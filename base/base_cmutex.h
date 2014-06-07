
/////////////////////////////////////////////////////////////////////////////
//
// base_cmutex.h - ::BASE::CMutex class header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

// PLATFORM DEPENDENT
// note: not implemented on PAF_AVR platforms

#pragma once
#ifndef _BASE_CMUTEX
#define _BASE_CMUTEX

#include "base_configuration.h"

#include "base_cobject.h"

namespace BASE {

class BASE_EXPORT_IMPORT CMutex : 
  virtual public CObject {
protected:

#if (OS_FAMILY == OSF_WINDOWS)

  //! critical section
  CRITICAL_SECTION m_tCriticalSection;

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  //! mutex attribute
  pthread_mutexattr_t m_tAttribute;
  //! mutex
  pthread_mutex_t m_tMutex;

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
  CMutex();
  //! destructor
  virtual ~CMutex();

  //! copy constructor
  CMutex(const CMutex & tMutex);
  //! assignment operator
  CMutex & operator=(const CMutex & tMutex);

  //! acquire
  virtual void Acquire(IObject::operations operation = IObject::WRITE, IObject::modes mode = IObject::BLOCKED);
  //! release
  void Release();
}; // class BASE_EXPORT_IMPORT CMutex

} // namespace BASE

#endif // #ifndef _BASE_CMUTEX
