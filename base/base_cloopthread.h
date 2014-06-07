
/////////////////////////////////////////////////////////////////////////////
//
// base_cloopthread.h - ::BASE::CLoopThread class header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _BASE_CLOOPTHREAD
#define _BASE_CLOOPTHREAD

#include "base_configuration.h"

#include "base_iloopthread.h"

#include "base_cthread.h"

namespace BASE {

DERIVE_ABSTRACT_EXCEPTION(EThread, ELoopThread);

class BASE_EXPORT_IMPORT CLoopThread :
  virtual public ILoopThread,
  public CThread {
public:
  MEMBER_GET(IObject::modes, mode);
  MEMBER_GET(T_TIME, TimeOut);

private:
  //! construct
  void __construct(IObject::modes mode, const T_TIME & tTimeOut);

protected:
  //! execute
  virtual void Execute();

public:
  //! constructor
  CLoopThread(IObject::modes mode = IObject::NON_BLOCKED, const T_TIME & tTimeOut = T_TIME());
  //! destructor
  virtual ~CLoopThread();

  //! copy constructor
  CLoopThread(const CLoopThread & tLoopThread);
  //! assignment operator
  CLoopThread & operator=(const CLoopThread & tLoopThread);
}; // class BASE_EXPORT_IMPORT CLoopThread

} // namespace BASE

#endif // #ifndef _BASE_CLOOPTHREAD
