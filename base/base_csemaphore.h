
/////////////////////////////////////////////////////////////////////////////
//
// base_csemaphore.h - ::BASE::CSemaphore class header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

// PLATFORM DEPENDENT
// note: not implemented on PAF_AVR platforms

#pragma once
#ifndef _BASE_CSEMAPHORE
#define _BASE_CSEMAPHORE

#include "base_configuration.h"

#include "base_isemaphore.h"

#include "base_cobject.h"

namespace BASE {

DERIVE_ABSTRACT_EXCEPTION(EObject, ESemaphore);

class BASE_EXPORT_IMPORT CSemaphore :
  virtual public ISemaphore, 
  virtual public CObject {
protected:
  MEMBER(T_ULONG, Value);

#if ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  //! semaphore
  sem_t m_tSemaphore;

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

  //! construct
  void __construct(T_ULONG uValue);
  //! destruct
  void __destruct();

public:
  //! constructor
  CSemaphore(T_ULONG uValue);
  //! destructor
  virtual ~CSemaphore();

  //! copy constructor
  CSemaphore(const CSemaphore & tSemaphore);
  //! assignment operator
  CSemaphore & operator=(const CSemaphore & tSemaphore);

  //! acquire
  virtual void Acquire(IObject::operations operation = IObject::WRITE, IObject::modes mode = IObject::BLOCKED);
  //! release
  void Release();
}; // class BASE_EXPORT_IMPORT CSemaphore

} // namespace BASE

#endif // #ifndef _BASE_CSEMAPHORE
