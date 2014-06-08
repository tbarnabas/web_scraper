
/////////////////////////////////////////////////////////////////////////////
//
// resource_capplication.cpp - ::RESOURCE::CApplication class source
//
/////////////////////////////////////////////////////////////////////////////

#include "resource_capplication.h"

namespace RESOURCE {

/////////////////////////////////////////////////////////////////////////////
void CApplication::Loop() {
  GUARD __tGuard(m_ObjectManager);
  m_ObjectManager.Maintain();
} // Loop


/////////////////////////////////////////////////////////////////////////////
CApplication::CApplication(::BASE::IObject::modes mode, const T_TIME & tTimeOut) :
  ::BASE::CLoopThread(mode, tTimeOut) {
  m_ObjectManager.SetSynch(REFERENCE< ::BASE::IObject>().Create(new ::BASE::CMutex()));
} // CApplication


/////////////////////////////////////////////////////////////////////////////
CApplication::~CApplication() {
} // ~CApplication


/////////////////////////////////////////////////////////////////////////////
void CApplication::Initialize() {
  {
    GUARD __tGuard(m_ObjectManager);
    m_ObjectManager.Initialize();
  }
  ::BASE::CLoopThread::Initialize();
} // Initialize


/////////////////////////////////////////////////////////////////////////////
void CApplication::Shutdown(T_BOOLEAN bImmediate) {
  ::BASE::CLoopThread::Shutdown(bImmediate);
  {
    GUARD __tGuard(m_ObjectManager);
    m_ObjectManager.Shutdown(bImmediate);
  }
} // Shutdown


/////////////////////////////////////////////////////////////////////////////
void CApplication::Maintain() {
  ::BASE::CLoopThread::Maintain();
  {
    GUARD __tGuard(m_ObjectManager);
    m_ObjectManager.Maintain();
  }
} // Maintain

} // namespace RESOURCE
