
/////////////////////////////////////////////////////////////////////////////
//
// base_cguard.h - ::BASE::CGuard<T> class header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _BASE_CGUARD
#define _BASE_CGUARD

#include "base_configuration.h"

#include "base_iobject.h"

namespace BASE {

#define BASEGUARD ::BASE::CGuard

class BASE_EXPORT_IMPORT CGuard {
private:
  //! reference to object
  REFERENCE<IObject> m_tObject;

  //! copy constructor
  CGuard(const CGuard & tGuard);
  //! assignment operator
  CGuard & operator=(const CGuard & tGuard);

public:
  //! constructor
  CGuard(IObject & tObject, IObject::operations operation = IObject::WRITE, IObject::modes mode = IObject::BLOCKED);
  //! destructor
  virtual ~CGuard();
}; // class CGuard

} // namespace BASE

#endif // #ifndef _BASE_CGUARD
