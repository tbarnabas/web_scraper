
/////////////////////////////////////////////////////////////////////////////
//
// thread_cloopthread.cpp - ::THREAD::CLoopThread class source
//
/////////////////////////////////////////////////////////////////////////////

#include "thread_cloopthread.h"

#include "thread_ccondition.h"
#include "thread_cguard.h"

#include <unistd.h>
namespace THREAD {


/////////////////////////////////////////////////////////////////////////////
void CLoopThread::__construct(::BASE::CObject::synchronizations synchronization, const T_TIME & tTimeOut) {
  m_Synchronizator.Create(new ::THREAD::CCondition());
  m_Synchronization = synchronization;
  m_TimeOut = tTimeOut;
} // __construct


/////////////////////////////////////////////////////////////////////////////
void CLoopThread::Execute() {
  THREADGUARD __tGuard(* this);

  do {
    // release object
    ::THREAD::CObject::Release();

    // loop
    IGNORE_EXCEPTION(Loop());
printf("--1--\n");
    // acquire object
    ::THREAD::CObject::Acquire();
printf("--2--\n");

    // waiting for timeout or exit condition
    if (::THREAD::CObject::GetInterrupted() == false) {
printf("--3--\n");
      if (m_Synchronization == ::BASE::CObject::BLOCKED) {
printf("--4--\n");
        IGNORE_EXCEPTION(::THREAD::CObject::Wait(m_TimeOut));
      }
    }
  } while (::THREAD::CObject::GetInterrupted() == false);
printf("--5--\n");
  
  printf("THERAD %d EXITED\n", ::THREAD::CThread::STATIC_GetCurrentThreadId());
} // Execute


/////////////////////////////////////////////////////////////////////////////
CLoopThread::CLoopThread(::BASE::CObject::synchronizations synchronization, const T_TIME & tTimeOut) {
  __construct(synchronization, tTimeOut);
} // CLoopThread


/////////////////////////////////////////////////////////////////////////////
CLoopThread::~CLoopThread() {
} // ~CLoopThread


/////////////////////////////////////////////////////////////////////////////
CLoopThread::CLoopThread(const CLoopThread & tLoopThread) :
  ::THREAD::CThread(tLoopThread) {
  __construct(tLoopThread.m_Synchronization, tLoopThread.m_TimeOut);
} // CLoopThread


/////////////////////////////////////////////////////////////////////////////
CLoopThread & CLoopThread::operator=(const CLoopThread & tLoopThread) {
  ::THREAD::CThread::operator=(tLoopThread);
  __construct(tLoopThread.m_Synchronization, tLoopThread.m_TimeOut);
  return (* this);
} // operator=

} // namespace THREAD
