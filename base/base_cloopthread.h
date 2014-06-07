
/////////////////////////////////////////////////////////////////////////////
//
// base_cloopthread.h - ::THREAD::CLoopThread class header
// --------------------------------------------------------------------------
//
// THREAD
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _THREAD_CLOOPTHREAD
#define _THREAD_CLOOPTHREAD

#include "base_configuration.h"

#include "base_cthread.h"

namespace THREAD {

DERIVE_ABSTRACT_EXCEPTION(::THREAD::EThread, ELoopThread);

class THREAD_EXPORT_IMPORT CLoopThread :
  public ::THREAD::CThread {
public:
  MEMBER_GET(::BASE::CObject::synchronizations, Synchronization);
  MEMBER_GET(T_TIME, TimeOut);

private:
  //! construct
  void __construct(::BASE::CObject::synchronizations synchronization, const T_TIME & tTimeOut);

protected:
  //! execute
  virtual void Execute();
  //! loop
  virtual void Loop() = 0;

public:
  //! constructor
  CLoopThread(::BASE::CObject::synchronizations synchronization = ::BASE::CObject::NON_BLOCKED, const T_TIME & tTimeOut = T_TIME());
  //! destructor
  virtual ~CLoopThread();

  //! copy constructor
  CLoopThread(const CLoopThread & tLoopThread);
  //! assignment operator
  CLoopThread & operator=(const CLoopThread & tLoopThread);
}; // class THREAD_EXPORT_IMPORT CLoopThread

} // namespace THREAD

#endif // #ifndef _THREAD_CLOOPTHREAD
