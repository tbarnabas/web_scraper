
/////////////////////////////////////////////////////////////////////////////
//
// resource_capplication.cpp - ::RESOURCE::CApplication class source
//
/////////////////////////////////////////////////////////////////////////////

#include "resource_capplication.h"

namespace RESOURCE {

/////////////////////////////////////////////////////////////////////////////
void CApplication::Loop() {
  THREADGUARD __tGuard(m_tObjectManager);
  m_tObjectManager.Maintain();
} // Loop


/////////////////////////////////////////////////////////////////////////////
CApplication::CApplication(::BASE::CObject::synchronizations synchronization, const T_TIME & tTimeOut) :
  ::THREAD::CLoopThread(synchronization, tTimeOut) {
  REFERENCE< ::THREAD::CObject> tSynchronizator;
  tSynchronizator.Create(new ::THREAD::CMutex());
  m_tObjectManager.SetSynchronizator(tSynchronizator);
} // CApplication


/////////////////////////////////////////////////////////////////////////////
CApplication::~CApplication() {
} // ~CApplication


/////////////////////////////////////////////////////////////////////////////
void CApplication::Initialize() {
  {
    THREADGUARD __tGuard(m_tObjectManager);
    m_tObjectManager.Initialize();
  }
  ::THREAD::CLoopThread::Initialize();
} // Initialize


/////////////////////////////////////////////////////////////////////////////
void CApplication::Shutdown(T_BOOLEAN bImmediate) {
  ::THREAD::CLoopThread::Shutdown(bImmediate);
  {
    THREADGUARD __tGuard(m_tObjectManager);
    m_tObjectManager.Shutdown(bImmediate);
  }
} // Shutdown


/////////////////////////////////////////////////////////////////////////////
void CApplication::Maintain() {
  ::THREAD::CLoopThread::Maintain();
  {
    THREADGUARD __tGuard(m_tObjectManager);
    m_tObjectManager.Maintain();
  }
} // Maintain

} // namespace RESOURCE
