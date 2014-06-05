
/////////////////////////////////////////////////////////////////////////////
//
// thread_cobject.h - ::THREAD::CObject class header
// --------------------------------------------------------------------------
//
// THREAD
//
/////////////////////////////////////////////////////////////////////////////

// PLATFORM DEPENDENT

#pragma once
#ifndef _THREAD_COBJECT
#define _THREAD_COBJECT

#include "thread_configuration.h"

namespace THREAD {

DERIVE_EXCEPTION_BEGIN(::BASE::EObject, EObject)
  INTERRUPTED
DERIVE_EXCEPTION_END(EObject);

class THREAD_EXPORT_IMPORT CObject :
  virtual public ::BASE::CObject {
private:
  //! construct
  void __construct();

public:
  MEMBER_GET_SET__REFERENCE(::THREAD::CObject, Synchronizator);
  MEMBER_GET(T_BOOLEAN, Interrupted);

public:
  //! constructor
  CObject();
  //! destructor
  virtual ~CObject();

  //! copy constructor
  CObject(const CObject & tObject);
  //! assignment operator
  CObject & operator=(const CObject & tObject);

  //! acquire
  virtual void Acquire(::BASE::CObject::operations operation = ::BASE::CObject::WRITE, ::BASE::CObject::synchronizations synchronization = ::BASE::CObject::BLOCKED);
  //! release
  virtual void Release();
  //! send wakeup signal to one of waiting threads
  virtual void Signal();
  //! send wakeup signal to all of waiting threads
  virtual void Broadcast();
  //! waiting for wakeup signal
  virtual void Wait(const T_TIME & tTimeOut = T_TIME());
  //! interrupt
  virtual void Interrupt();
}; // class THREAD_EXPORT_IMPORT CObject

} // namespace THREAD

#endif // #ifndef _THREAD_COBJECT
