
/////////////////////////////////////////////////////////////////////////////
//
// base_ithread.h - ::BASE::IThread interface header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _BASE_ITHREAD
#define _BASE_ITHREAD

#include "base_configuration.h"

#include "base_iobject.h"

namespace BASE {

class BASE_EXPORT_IMPORT IThread :
  virtual public IObject {
protected:
  //! construct
  virtual void Construct() = 0;
  //! destruct
  virtual void Destruct() = 0;
  //! execute
  virtual void Execute() = 0;
  
public:
  //! destructor
  virtual ~IThread() {}  
}; // class BASE_EXPORT_IMPORT IThread

} // namespace BASE

#endif // #ifndef _BASE_ITHREAD
