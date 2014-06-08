
/////////////////////////////////////////////////////////////////////////////
//
// base_icondition.h - ::BASE::ICondition interface header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _BASE_ICONDITION
#define _BASE_ICONDITION

#include "base_configuration.h"

#include "base_imutex.h"

namespace BASE {

class BASE_EXPORT_IMPORT ICondition :
  virtual public IObject { 
public:
  //! destructor
  virtual ~ICondition() {}  
}; // class BASE_EXPORT_IMPORT ICondition

} // namespace BASE

#endif // #ifndef _BASE_ICONDITION
