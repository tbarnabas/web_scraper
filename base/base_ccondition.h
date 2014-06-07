
/////////////////////////////////////////////////////////////////////////////
//
// base_ccondition.h - ::BASE::CCondition class header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

// PLATFORM DEPENDENT
// note: not implemented on PAF_AVR platforms

#pragma once
#ifndef _BASE_CCONDITION
#define _BASE_CCONDITION

#include "base_configuration.h"

#include "base_icondition.h"

#include "base_cmutex.h"

namespace BASE {

DERIVE_EXCEPTION_BEGIN(EMutex, ECondition)
  TIMED_OUT
DERIVE_EXCEPTION_END(ECondition);

class BASE_EXPORT_IMPORT CCondition :
  virtual public ICondition,
  virtual public CMutex {
private:

#if (OS_FAMILY == OSF_WINDOWS)

  typedef struct {
    //! mutex
    CMutex m_tMutex;
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
  //! shutdown
  virtual void Shutdown(T_BOOLEAN bImmediate = false);
}; // class BASE_EXPORT_IMPORT CCondition

} // namespace BASE

#endif // #ifndef _BASE_CCONDITION
