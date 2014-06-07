
/////////////////////////////////////////////////////////////////////////////
//
// thread_cobject.cpp - ::THREAD::CObject class source
//
/////////////////////////////////////////////////////////////////////////////

#include "thread_cobject.h"

namespace THREAD {

/////////////////////////////////////////////////////////////////////////////
void CObject::__construct() {
  m_Interrupted = false; // unique per instance
} // __construct


/////////////////////////////////////////////////////////////////////////////
CObject::CObject() {
  __construct();
} // CObject


/////////////////////////////////////////////////////////////////////////////
CObject::~CObject() {
} // ~CObject


/////////////////////////////////////////////////////////////////////////////
CObject::CObject(const CObject & tObject) :
  ::BASE::CObject(tObject) {
  __construct();
} // CObject


/////////////////////////////////////////////////////////////////////////////
CObject & CObject::operator=(const CObject & tObject) {
  ::BASE::CObject::operator=(tObject);
  __construct();
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
void CObject::Acquire(::BASE::CObject::operations operation, ::BASE::CObject::synchronizations synchronization) {
  if (m_Synchronizator.IsValid() == true) {
    m_Synchronizator->Acquire(operation, synchronization);
  }
} // Acquire


/////////////////////////////////////////////////////////////////////////////
void CObject::Release() {
  if (m_Synchronizator.IsValid() == true) {
    m_Synchronizator->Release();
  }
} // Release


/////////////////////////////////////////////////////////////////////////////
void CObject::Signal() {
  if (m_Synchronizator.IsValid() == true) {
    m_Synchronizator->Signal();
  }
} // Signal


/////////////////////////////////////////////////////////////////////////////
void CObject::Broadcast() {
  if (m_Synchronizator.IsValid() == true) {
    m_Synchronizator->Broadcast();
  }
} // Broadcast


/////////////////////////////////////////////////////////////////////////////
void CObject::Wait(const T_TIME & tTimeOut) {
  if (m_Synchronizator.IsValid() == true) {
    m_Synchronizator->Wait(tTimeOut);
  }
} // Wait


/////////////////////////////////////////////////////////////////////////////
void CObject::Interrupt() {
printf("%p INTERRUPTED\n", this);
  if (m_Synchronizator.IsValid() == true) {
    m_Synchronizator->Interrupt();
  }
  m_Interrupted = true;
} // Interrupt

} // namespace THREAD
