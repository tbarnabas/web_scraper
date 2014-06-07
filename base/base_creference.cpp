
/////////////////////////////////////////////////////////////////////////////
//
// base_creference.cpp - ::BASE::CReference<T> class source
//
/////////////////////////////////////////////////////////////////////////////

#include "base_creference.h"

#include "base_cmutex.h"

namespace BASE {

CMutex * GLOBAL_CReference__pMutex = NULL;

/////////////////////////////////////////////////////////////////////////////
void CReference__Create() {
  if (GLOBAL_CReference__pMutex == NULL) {
    GLOBAL_CReference__pMutex = new ::BASE::CMutex();
  }
} // CReference__Create


/////////////////////////////////////////////////////////////////////////////
void CReference__Destroy() {
  if (GLOBAL_CReference__pMutex != NULL) {
    delete GLOBAL_CReference__pMutex;
    GLOBAL_CReference__pMutex = NULL;
  }
} // CReference__Destroy


/////////////////////////////////////////////////////////////////////////////
void CReference__tMutex__Acquire() {
  GLOBAL_CReference__pMutex->Acquire();
} // CReference__tMutex__Acquire


/////////////////////////////////////////////////////////////////////////////
void CReference__tMutex__Release() {
  GLOBAL_CReference__pMutex->Release();
} // CReference__tMutex__Release
        
} // namespace BASE
