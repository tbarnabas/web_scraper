
/////////////////////////////////////////////////////////////////////////////
//
// thread_ccondition.h - ::THREAD::CCondition class header
// --------------------------------------------------------------------------
//
// THREAD
//
/////////////////////////////////////////////////////////////////////////////

// PLATFORM DEPENDENT
// note: not implemented on PAF_AVR platforms

#pragma once
#ifndef _THREAD_CCONDITION
#define _THREAD_CCONDITION

#include "thread_configuration.h"

#include "thread_cmutex.h"

namespace THREAD {

DERIVE_EXCEPTION_BEGIN(::THREAD::EMutex, ECondition)
  TIMED_OUT
DERIVE_EXCEPTION_END(ECondition);

class THREAD_EXPORT_IMPORT CCondition :
  virtual public ::THREAD::CMutex {
private:

#if (OS_FAMILY == OSF_WINDOWS)

  typedef struct {
    //! mutex
    ::THREAD::CMutex m_tMutex;
    //! number of waiting threads
    T_UINT m_uWait;
    //! number of released threads
    T_UINT m_uReleased;
    //! generation id
    T_UINT m_uGeneration;
    //! event handle
    HANDLE m_hEvent;
  } thread_cond_t;

  //! condition variable
  thread_cond_t m_tCondition;

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  //! condition variable
  pthread_cond_t m_tCondition;

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

  //! construct
  void __construct();
  //! destruct
  void __destruct();

public:
  //! constructor
  CCondition();
  //! destructor
  virtual ~CCondition();

  //! copy constructor
  CCondition(const CCondition & tCondition);
  //! assignment operator
  CCondition & operator=(const CCondition & tCondition);

  //! send wakeup signal to one of waiting threads
  virtual void Signal();
  //! send wakeup signal to all of waiting threads
  virtual void Broadcast();
  //! waiting for wakeup signal
  virtual void Wait(const T_TIME & tTimeOut = T_TIME());
  //! interrupt
  virtual void Interrupt();
}; // class THREAD_EXPORT_IMPORT CCondition

} // namespace THREAD

#endif // #ifndef _THREAD_CCONDITION
