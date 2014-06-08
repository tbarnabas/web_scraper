
/////////////////////////////////////////////////////////////////////////////
//
// base_cguard.cpp - ::BASE::CGuard class source
//
/////////////////////////////////////////////////////////////////////////////

#include "base_cguard.h"

namespace BASE {

/////////////////////////////////////////////////////////////////////////////
CGuard::CGuard(IObject & tObject, IObject::operations operation, IObject::modes mode, T_BOOLEAN bInverse) :
  m_operation(operation),
  m_mode(mode),
  m_Inverse(bInverse) {
  m_Object = &tObject;
  if (m_Object.IsValid() == true) {
    if (m_Inverse == false) {
      m_Object->Acquire(m_operation, m_mode);
    } else {
      m_Object->Release();
    }
  }
} // CGuard


/////////////////////////////////////////////////////////////////////////////
CGuard::~CGuard() {
  if (m_Object.IsValid() == true) {
    if (m_Inverse == false) {
      m_Object->Release();
    } else {
      m_Object->Acquire(m_operation, m_mode);
    }
  }
} // ~CGuard

} // namespace BASE
