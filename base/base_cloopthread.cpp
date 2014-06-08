
/////////////////////////////////////////////////////////////////////////////
//
// base_cloopthread.cpp - ::THREAD::CLoopThread class source
//
/////////////////////////////////////////////////////////////////////////////

#include "base_cloopthread.h"

#include "base_ccondition.h"
#include "base_cguard.h"

namespace BASE {

/////////////////////////////////////////////////////////////////////////////
void CLoopThread::__construct(IObject::modes mode, const T_TIME & tTimeOut) {
  m_Synch.Create(new CCondition());
  m_mode = mode;
  m_TimeOut = tTimeOut;
} // __construct


/////////////////////////////////////////////////////////////////////////////
void CLoopThread::Execute() {
  GUARD __tGuard(* this, IObject::WRITE, IObject::BLOCKED, false);

  do {
    // loop
    {
      GUARD __tGuard(* this, IObject::WRITE, IObject::BLOCKED, true);
      IGNORE_EXCEPTION(Loop());
    }

    // waiting for timeout or exit condition
    if (GetShutdown() == false) {
      if (m_mode == IObject::BLOCKED) {
        IGNORE_EXCEPTION(Wait(m_TimeOut));
      }
    }
  } while (GetShutdown() == false);
} // Execute


/////////////////////////////////////////////////////////////////////////////
CLoopThread::CLoopThread(IObject::modes mode, const T_TIME & tTimeOut) {
  __construct(mode, tTimeOut);
} // CLoopThread


/////////////////////////////////////////////////////////////////////////////
CLoopThread::~CLoopThread() {
} // ~CLoopThread


/////////////////////////////////////////////////////////////////////////////
CLoopThread::CLoopThread(const CLoopThread & tLoopThread) :
  CThread(tLoopThread) {
  __construct(tLoopThread.m_mode, tLoopThread.m_TimeOut);
} // CLoopThread


/////////////////////////////////////////////////////////////////////////////
CLoopThread & CLoopThread::operator=(const CLoopThread & tLoopThread) {
  CThread::operator=(tLoopThread);
  __construct(tLoopThread.m_mode, tLoopThread.m_TimeOut);
  return (* this);
} // operator=

} // namespace THREAD
