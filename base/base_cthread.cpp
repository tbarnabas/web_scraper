
/////////////////////////////////////////////////////////////////////////////
//
// base_cthread.cpp - ::BASE::CThread class source
//
/////////////////////////////////////////////////////////////////////////////

#include "base_cthread.h"

#include "base_cguard.h"
#include "base_cmutex.h"

namespace BASE {

/////////////////////////////////////////////////////////////////////////////
T_ULONG CThread::GetThreadId() {
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
} // GetThreadId


/////////////////////////////////////////////////////////////////////////////
void CThread::__construct(T_BOOLEAN bRunning) {
  if (bRunning == true) {
    // clear running state
    m_bRunning = false;

#if (OS_FAMILY == OSF_WINDOWS)

    DWORD uThreadId;

    // create a new thread
    m_hThread = CreateThread(NULL, NULL, __execute, this, NULL, &uThreadId);
    if (m_hThread == NULL) {

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

    // initialize thread attribute
    if (pthread_attr_init(&m_tAttribute) != 0) {
      EXCEPTION(BASE, ::BASE::CThread, __construct,
      MESSAGE("FATAL ERROR: unable to initialize thread attribute"));
      THROW(EThread, UNABLE_CONSTRUCT);
    }

    // set thread attribute
    if (pthread_attr_setdetachstate(&m_tAttribute, PTHREAD_CREATE_JOINABLE) != 0) {
      EXCEPTION(BASE, ::BASE::CThread, __construct,
      MESSAGE("FATAL ERROR: unable to set thread attribute"));
      THROW(EThread, UNABLE_CONSTRUCT);
    }

    // create a new thread
    if (pthread_create(&m_tThread, &m_tAttribute, __execute, this) != 0) {

#elif (PA_FAMILY == PAF_AVR)

    if (1 != 0) {

#else
#error unsupported platform
#endif

      EXCEPTION(BASE, ::BASE::CThread, __construct,
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

      EXCEPTION(BASE, ::BASE::CThread, __destruct,
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

DWORD WINAPI CThread::__execute(LPVOID pArg) {

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX) | (PA_FAMILY == PAF_AVR))

void * CThread::__execute(void * pArg) {

#else
#error unsupported platform
#endif

  CThread * pThread = static_cast<CThread *>(pArg);

  // run thread
  IGNORE_EXCEPTION(
    pThread->Construct();
    pThread->Execute();
    pThread->Destruct());

  return (0);
} // __execute


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
  CObject(tThread) {
  __construct(tThread.m_bRunning);
} // CThread


/////////////////////////////////////////////////////////////////////////////
CThread & CThread::operator=(const CThread & tThread) {
  __destruct();
  CObject::operator=(tThread);
  __construct(tThread.m_bRunning);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
void CThread::Initialize() {
  CObject::Initialize();
  __construct(true);
} // Initialize


/////////////////////////////////////////////////////////////////////////////
void CThread::Shutdown(T_BOOLEAN bImmediate) {
  GUARD __tGuard(* this, IObject::WRITE, IObject::BLOCKED, false);
  CObject::Shutdown(bImmediate);
  {
    GUARD __tGuard(* this, IObject::WRITE, IObject::BLOCKED, true);
    __destruct();
  }
  CObject::Shutdown(bImmediate);
} // Shutdown

} // namespace THREAD
