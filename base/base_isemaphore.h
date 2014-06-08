
/////////////////////////////////////////////////////////////////////////////
//
// base_isemaphore.h - ::BASE::ISemaphore interface header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _BASE_ISEMAPHORE
#define _BASE_ISEMAPHORE

#include "base_configuration.h"

#include "base_iobject.h"

namespace BASE {

class BASE_EXPORT_IMPORT ISemaphore :
  virtual public IObject {
public:
  //! destructor
  virtual ~ISemaphore() {}  
}; // class BASE_EXPORT_IMPORT ISemaphore

} // namespace BASE

#endif // #ifndef _BASE_ISEMAPHORE
