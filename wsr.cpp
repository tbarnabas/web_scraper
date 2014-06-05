
/////////////////////////////////////////////////////////////////////////////
//
// wsr.cpp - module source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr.h"

namespace WSR {

T_BOOLEAN GLOBAL_bCreated = false;
T_BOOLEAN GLOBAL_bInitialized = false;

/////////////////////////////////////////////////////////////////////////////
void Create() {
  if (GLOBAL_bCreated == false) {
    ::BASE::Create();
    ::THREAD::Create();
    ::DATASTRUCTURE::Create();
    GLOBAL_bCreated = true;
  }
} // Create


/////////////////////////////////////////////////////////////////////////////
void Initialize() {
  Create();
  if (GLOBAL_bInitialized == false)  {
    ::BASE::Initialize();
    ::THREAD::Initialize();
    ::DATASTRUCTURE::Initialize();
    GLOBAL_bInitialized = true;
  }
} // Initialize


/////////////////////////////////////////////////////////////////////////////
void Shutdown(T_BOOLEAN bImmediate) {
  if (GLOBAL_bInitialized == true)  {
    ::DATASTRUCTURE::Shutdown(bImmediate);
    ::THREAD::Shutdown(bImmediate);
    ::BASE::Shutdown(bImmediate);
    GLOBAL_bInitialized = false;
  }
} // Shutdown


/////////////////////////////////////////////////////////////////////////////
void Destroy() {
  Shutdown(true);
  if (GLOBAL_bCreated == true)  {
    ::DATASTRUCTURE::Destroy();
    ::THREAD::Destroy();
    ::BASE::Destroy();
    GLOBAL_bCreated = false;
  }
} // Destroy

} // namespace WSR
