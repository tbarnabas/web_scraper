
/////////////////////////////////////////////////////////////////////////////
//
// thread_cguard.h - ::THREAD::CGuard<T> class header
// --------------------------------------------------------------------------
//
// THREAD
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _THREAD_CGUARD
#define _THREAD_CGUARD

#include "thread_configuration.h"

#include "thread_cobject.h"

namespace THREAD {

#define THREADGUARD ::THREAD::CGuard

class THREAD_EXPORT_IMPORT CGuard {
private:
  //! reference to object
  REFERENCE< ::THREAD::CObject> m_tObject;

  //! copy constructor
  CGuard(const CGuard & tGuard);
  //! assignment operator
  CGuard & operator=(const CGuard & tGuard);

public:
  //! constructor
  CGuard(::THREAD::CObject & tObject, ::BASE::CObject::operations operation = ::BASE::CObject::WRITE, ::BASE::CObject::synchronizations synchronization = ::BASE::CObject::BLOCKED);
  //! destructor
  virtual ~CGuard();
}; // class CGuard

} // namespace THREAD

#endif // #ifndef _THREAD_CGUARD
