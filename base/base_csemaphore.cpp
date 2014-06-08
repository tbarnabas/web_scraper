
/////////////////////////////////////////////////////////////////////////////
//
// base_csemaphore.cpp - ::BASE::CSemaphore class source
//
/////////////////////////////////////////////////////////////////////////////

#include "base_csemaphore.h"

namespace BASE {

/////////////////////////////////////////////////////////////////////////////
void CSemaphore::__construct(T_ULONG uValue) {
  m_Value = uValue;

#if ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  // initialize semaphore
  sem_init(&m_tSemaphore, 0, uValue);

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

} // __construct


/////////////////////////////////////////////////////////////////////////////
void CSemaphore::__destruct() {

#if ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  // destroy semaphore
  sem_destroy(&m_tSemaphore);

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

} // __destruct


/////////////////////////////////////////////////////////////////////////////
CSemaphore::CSemaphore(T_ULONG uValue) {
  __construct(uValue);
} // CSemaphore


/////////////////////////////////////////////////////////////////////////////
CSemaphore::~CSemaphore() {
  __destruct();
} // ~CSemaphore


/////////////////////////////////////////////////////////////////////////////
CSemaphore::CSemaphore(const CSemaphore & tSemaphore) :
  CObject(tSemaphore) {
  __construct(tSemaphore.m_Value);
} // CSemaphore


/////////////////////////////////////////////////////////////////////////////
CSemaphore & CSemaphore::operator=(const CSemaphore & tSemaphore) {
  __destruct();
  CObject::operator=(tSemaphore);
  __construct(tSemaphore.m_Value);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
void CSemaphore::Acquire(IObject::operations operation, IObject::modes mode) {
    
#if ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))
    
  // lock semaphore variable
  sem_wait(&m_tSemaphore);
    
#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

} // Acquire


/////////////////////////////////////////////////////////////////////////////
void CSemaphore::Release() {

#if ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  // unlock semaphore variable
  sem_post(&m_tSemaphore);

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif

} // Release

} // namespace BASE
