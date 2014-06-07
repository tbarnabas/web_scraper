
/////////////////////////////////////////////////////////////////////////////
//
// base_cguard.cpp - ::BASE::CGuard class source
//
/////////////////////////////////////////////////////////////////////////////

#include "base_cguard.h"

namespace BASE {

/////////////////////////////////////////////////////////////////////////////
CGuard::CGuard(IObject & tObject, IObject::operations operation, IObject::modes mode) {
  m_tObject = &tObject;

  // acquire object
  if (m_tObject.IsValid() == true) {
    m_tObject->Acquire(operation, mode);
  }
} // CGuard


/////////////////////////////////////////////////////////////////////////////
CGuard::~CGuard() {
  if (m_tObject.IsValid() == true) {
    m_tObject->Release();
    m_tObject = NULL;
  }
} // ~CGuard

} // namespace BASE
