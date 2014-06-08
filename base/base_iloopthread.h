
/////////////////////////////////////////////////////////////////////////////
//
// base_iloopthread.h - ::BASE::ILoopThread interface header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _BASE_ILOOPTHREAD
#define _BASE_ILOOPTHREAD

#include "base_configuration.h"

#include "base_ithread.h"

namespace BASE {

class BASE_EXPORT_IMPORT ILoopThread :
  virtual public IThread {
protected:
  //! loop
  virtual void Loop() = 0;
  
public:
  //! destructor
  virtual ~ILoopThread() {}  
}; // class BASE_EXPORT_IMPORT ILoopThread

} // namespace BASE

#endif // #ifndef _BASE_ILOOPTHREAD
