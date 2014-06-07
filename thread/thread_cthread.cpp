
/////////////////////////////////////////////////////////////////////////////
//
// thread_cthread.cpp - ::THREAD::CThread class source
//
/////////////////////////////////////////////////////////////////////////////

#include "thread_cthread.h"

#include "thread_cguard.h"
#include "thread_cmutex.h"

namespace THREAD {

/////////////////////////////////////////////////////////////////////////////
T_ULONG CThread::STATIC_GetCurrentThreadId() {
    T_ULONG uResult = 0;

#if (OS_FAMILY == OSF_WINDOWS)

    uResult = (T_ULONG)::GetCurrentThreadId();

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

    uResult = (T_ULONG)pthread_self();

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

    return (uResult);
} // STATIC_GetCurrentThreadId


/////////////////////////////////////////////////////////////////////////////
void CThread::__construct(T_BOOLEAN bRunning) {
  if (bRunning == true) {
    // clear running state
    m_bRunning = false;

#if (OS_FAMILY == OSF_WINDOWS)

    DWORD uThreadId;

    // create a new thread
    m_hThread = CreateThread(NULL, NULL, STATIC_Execute, this, NULL, &uThreadId);
    if (m_hThread == NULL) {

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

    // initialize thread attribute
    if (pthread_attr_init(&m_tAttribute) != 0) {
      EXCEPTION(THREAD, ::THREAD::CThread, __construct,
      MESSAGE("FATAL ERROR: unable to initialize thread attribute"));
      THROW(EThread, UNABLE_CONSTRUCT);
    }

    // set thread attribute
    if (pthread_attr_setdetachstate(&m_tAttribute, PTHREAD_CREATE_JOINABLE) != 0) {
      EXCEPTION(THREAD, ::THREAD::CThread, __construct,
      MESSAGE("FATAL ERROR: unable to set thread attribute"));
      THROW(EThread, UNABLE_CONSTRUCT);
    }

    // create a new thread
    if (pthread_create(&m_tThread, &m_tAttribute, STATIC_Execute, this) != 0) {

#elif (PA_FAMILY == PAF_AVR)

    if (1 != 0) {

#else
#error unsupported platform
#endif

      EXCEPTION(THREAD, ::THREAD::CThread, __construct,
      MESSAGE("FATAL ERROR: unable to create a new thread"));
      THROW(EThread, UNABLE_CONSTRUCT);
    }
  }

  // set or clear running state
  m_bRunning = bRunning;
} // __construct


/////////////////////////////////////////////////////////////////////////////
void CThread::__destruct() {
  if (m_bRunning == true) {

#if (OS_FAMILY == OSF_WINDOWS)

    // join thread
    if (WaitForSingleObject(m_hThread, INFINITE) == WAIT_FAILED) {

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

    // join thread
    if (pthread_join(m_tThread, NULL) != 0) {

#elif (PA_FAMILY == PAF_AVR)

    if (1 != 0) {

#else
#error unsupported platform
#endif

      EXCEPTION(THREAD, ::THREAD::CThread, __destruct,
      MESSAGE("FATAL ERROR: unable to join thread"));
      THROW(EThread, UNABLE_DESTRUCT);
    }

#if (OS_FAMILY == OSF_WINDOWS)

    // close handle
    CloseHandle(m_hThread);

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

    // destroy thread attribute
    pthread_attr_destroy(&m_tAttribute);

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

    // clear running state
    m_bRunning = false;
  }
} // __destruct


/////////////////////////////////////////////////////////////////////////////

#if (OS_FAMILY == OSF_WINDOWS)

DWORD WINAPI CThread::STATIC_Execute(LPVOID pArg) {

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX) | (PA_FAMILY == PAF_AVR))

void * CThread::STATIC_Execute(void * pArg) {

#else
#error unsupported platform
#endif

  CThread * pThread = static_cast<CThread *>(pArg);

  // construct
  IGNORE_EXCEPTION(pThread->Construct());

  // execute
  IGNORE_EXCEPTION(pThread->Execute());

  // destruct
  IGNORE_EXCEPTION(pThread->Destruct());

  return (0);
} // STATIC_Execute


/////////////////////////////////////////////////////////////////////////////
CThread::CThread() {
  __construct(false);
} // CThread


/////////////////////////////////////////////////////////////////////////////
CThread::~CThread() {
  __destruct();
} // ~CThread


/////////////////////////////////////////////////////////////////////////////
CThread::CThread(const CThread & tThread) :
  ::THREAD::CObject(tThread) {
  __construct(tThread.m_bRunning);
} // CThread


/////////////////////////////////////////////////////////////////////////////
CThread & CThread::operator=(const CThread & tThread) {
  __destruct();
  ::THREAD::CObject::operator=(tThread);
  __construct(tThread.m_bRunning);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
void CThread::Initialize() {
  ::THREAD::CObject::Initialize();
  __construct(true);
} // Initialize


/////////////////////////////////////////////////////////////////////////////
void CThread::Shutdown(T_BOOLEAN bImmediate) {
  THREADGUARD __tGuard(* this);
  ::THREAD::CObject::Interrupt();
  if (m_Synchronizator.IsValid() == true) {
    m_Synchronizator->Release();
  }
  __destruct();
  if (m_Synchronizator.IsValid() == true) {
    m_Synchronizator->Acquire();
  }
  ::THREAD::CObject::Shutdown(bImmediate);
} // Shutdown

} // namespace THREAD
