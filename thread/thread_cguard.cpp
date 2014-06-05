
/////////////////////////////////////////////////////////////////////////////
//
// thread_cguard.cpp - ::THREAD::CGuard class source
//
/////////////////////////////////////////////////////////////////////////////

#include "thread_cguard.h"

namespace THREAD {

/////////////////////////////////////////////////////////////////////////////
CGuard::CGuard(::THREAD::CObject & tObject, ::BASE::CObject::operations operation, ::BASE::CObject::synchronizations synchronization) {
  m_tObject = &tObject;

  // acquire object
  if (m_tObject.IsValid() == true) {
    m_tObject->Acquire(operation, synchronization);
  }
} // CGuard


/////////////////////////////////////////////////////////////////////////////
CGuard::~CGuard() {
  if (m_tObject.IsValid() == true) {
    m_tObject->Release();
    m_tObject = NULL;
  }
} // ~CGuard

} // namespace THREAD
