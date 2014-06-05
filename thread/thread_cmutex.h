
/////////////////////////////////////////////////////////////////////////////
//
// thread_cmutex.h - ::THREAD::CMutex class header
// --------------------------------------------------------------------------
//
// THREAD
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _THREAD_CMUTEX
#define _THREAD_CMUTEX

#include "thread_configuration.h"

#include "thread_cobject.h"

namespace THREAD {

DERIVE_EXCEPTION_BEGIN(::THREAD::EObject, EMutex)
  IS_LOCKED
DERIVE_EXCEPTION_END(EMutex);

class THREAD_EXPORT_IMPORT CMutex :
  public ::BASE::CMutex,
  virtual public ::THREAD::CObject {
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
  virtual void Acquire(::BASE::CObject::operations operation = ::BASE::CObject::WRITE, ::BASE::CObject::synchronizations synchronization = ::BASE::CObject::BLOCKED);
  //! release
  virtual void Release();
}; // class THREAD_EXPORT_IMPORT CMutex

} // namespace THREAD

#endif // #ifndef _THREAD_CMUTEX
