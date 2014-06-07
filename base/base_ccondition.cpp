
/////////////////////////////////////////////////////////////////////////////
//
// base_ccondition.cpp - ::BASE::CCondition class source
//
/////////////////////////////////////////////////////////////////////////////

#include "base_ccondition.h"

#include "base_cguard.h"

namespace BASE {

/////////////////////////////////////////////////////////////////////////////
void CCondition::__construct() {

#if (OS_FAMILY == OSF_WINDOWS)

  // initialize condition
  m_tCondition.m_uWait = 0;
  m_tCondition.m_uReleased = 0;
  m_tCondition.m_uGeneration = 0;

  // create event
  m_tCondition.m_hEvent = CreateEvent(NULL, true, false, NULL);

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  // initialize condition
  pthread_cond_init(&m_tCondition, NULL);

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

} // __construct


/////////////////////////////////////////////////////////////////////////////
void CCondition::__destruct() {

#if (OS_FAMILY == OSF_WINDOWS)

  // destroy event
  CloseHandle(m_tCondition.m_hEvent);

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  // destroy condition
  pthread_cond_destroy(&m_tCondition);

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

} // __destruct


/////////////////////////////////////////////////////////////////////////////
CCondition::CCondition() {
  __construct();
} // CCondition


/////////////////////////////////////////////////////////////////////////////
CCondition::~CCondition() {
  __destruct();
} // ~CCondition


/////////////////////////////////////////////////////////////////////////////
CCondition::CCondition(const CCondition & tCondition) :
  CMutex(tCondition) {
  __construct();
} // CCondition


/////////////////////////////////////////////////////////////////////////////
CCondition & CCondition::operator=(const CCondition & tCondition) {
  __destruct();
  CMutex::operator=(tCondition);
  __construct();
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
void CCondition::Signal() {

#if (OS_FAMILY == OSF_WINDOWS)

  THREADGUARD __tGuard(m_tCondition.m_tMutex);

  if (m_tCondition.m_uWait > m_tCondition.m_uReleased) {
    // send wakeup signal
    SetEvent(m_tCondition.m_hEvent);

    // increment number of released threads
    m_tCondition.m_uReleased = m_tCondition.m_uReleased + 1;

    // increment generation id
    m_tCondition.m_uGeneration = m_tCondition.m_uGeneration + 1;
  }

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  // send wakeup signal to one of waiting threads
  pthread_cond_signal(&m_tCondition);

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

} // Signal


/////////////////////////////////////////////////////////////////////////////
void CCondition::Broadcast() {

#if (OS_FAMILY == OSF_WINDOWS)

  THREADGUARD __tGuard(m_tCondition.m_tMutex);

  if (m_tCondition.m_uWait > 0) {
    // send wakeup signal
    SetEvent(m_tCondition.m_hEvent);

    // set number of released threads
    m_tCondition.m_uReleased = m_tCondition.m_uWait;

    // increment generation id
    m_tCondition.m_uGeneration = m_tCondition.m_uGeneration + 1;
  }

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  // send wakeup signal to all of waiting threads
  pthread_cond_broadcast(&m_tCondition);

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

} // Broadcast


/////////////////////////////////////////////////////////////////////////////
void CCondition::Wait(const T_TIME & tTimeOut) {
  T_BOOLEAN bTimedOut = false;
  T_BOOLEAN bShutdown = false;

  // acquire object
  Acquire();

#if (OS_FAMILY == OSF_WINDOWS)

  T_UINT uGeneration = 0;
  T_BOOLEAN bExit = false;
  T_BOOLEAN bReset = false;

  {
    THREADGUARD __tGuard(m_tCondition.m_tMutex);

    // increment number of waiting threads
    m_tCondition.m_uWait = m_tCondition.m_uWait + 1;

    // get generation id
    uGeneration = m_tCondition.m_uGeneration;
  }

  // release object
  Release();

  while ((bExit == false) && (bTimedOut == false) && (bShutdown == false)) {
    DWORD uResult = 0;

    // waiting for events
    if (tTimeOut.IsValid() == true) {
      uResult = WaitForSingleObject(m_tCondition.m_hEvent, T_TIME_TO_MSEC(tTimeOut));
    } else {
      uResult = WaitForSingleObject(m_tCondition.m_hEvent, INFINITE);
    }

    // check timed out state
    if (uResult == WAIT_TIMEOUT) {
      bTimedOut = true;
    }

    // check interrupted state
    bShutdown = GetShutdown();

    // check exit condition
    if ((bTimedOut == false) && (bShutdown == false)) {
      THREADGUARD __tGuard(m_tCondition.m_tMutex);

      if ((m_tCondition.m_uReleased > 0) && (m_tCondition.m_uGeneration != uGeneration)) {
        bExit = true;
      }
    }
  }

  // acquire object
  Acquire();

  {
    THREADGUARD __tGuard(m_tCondition.m_tMutex);

    // decrement number of waiting threads
    m_tCondition.m_uWait = m_tCondition.m_uWait - 1;

    // decrement number of released threads
    if (m_tCondition.m_uReleased > 0) {
      m_tCondition.m_uReleased = m_tCondition.m_uReleased - 1;
    }

    if (m_tCondition.m_uReleased == 0) {
      bReset = true;
    }
  }

  if (bReset == true) {
    ResetEvent(m_tCondition.m_hEvent);
  }

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  // waiting for wakeup signal
  if (tTimeOut.IsValid() == false) {
    pthread_cond_wait(&m_tCondition, &m_tMutex);
  } else {
    timespec t;

    // add current time and timeout
    T_TIME _tTimeOut = tTimeOut + T_TIME::GetCurrent();

    t.tv_sec = _tTimeOut.GetDelaySec();
    t.tv_nsec = _tTimeOut.GetDelayUSec() * 1000;
    if (pthread_cond_timedwait(&m_tCondition, &m_tMutex, &t) == ETIMEDOUT) {
      bTimedOut = true;
    }

    // check interrupted state
    bShutdown = GetShutdown();
  }

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

  // release object
  Release();

  if (bShutdown == true) {
    EXCEPTION(BASE, ::BASE::CCondition, Wait,
    MESSAGE("WARNING: shutting down .."));
    THROW(ECondition, TIMED_OUT);
  }

  if (bTimedOut == true) {
    EXCEPTION(BASE, ::BASE::CCondition, Wait,
    MESSAGE("WARNING: timed out"));
    THROW(ECondition, TIMED_OUT);
  }
} // Wait


/////////////////////////////////////////////////////////////////////////////
void CCondition::Shutdown(T_BOOLEAN bImmediate) {
  CMutex::Shutdown(bImmediate);
  Broadcast();
} // Shutdown

} // namespace THREAD
