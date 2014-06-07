
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

#define GUARD ::BASE::CGuard

class BASE_EXPORT_IMPORT CGuard {
private:
  MEMBER__REFERENCE(IObject, Object);
  MEMBER(IObject::operations, operation);
  MEMBER(IObject::modes, mode);
  MEMBER(T_BOOLEAN, Inverse);
  
  //! copy constructor
  CGuard(const CGuard & tGuard);
  //! assignment operator
  CGuard & operator=(const CGuard & tGuard);

public:
  //! constructor
  CGuard(IObject & tObject, IObject::operations operation = IObject::WRITE, IObject::modes mode = IObject::BLOCKED, T_BOOLEAN bInverse = false);
  //! destructor
  virtual ~CGuard();
}; // class CGuard

} // namespace BASE

#endif // #ifndef _BASE_CGUARD
