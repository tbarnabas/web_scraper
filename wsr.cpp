
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
    ::DATASTRUCTURE::Create();
    ::RESOURCE::Create();
    curl_global_init(CURL_GLOBAL_ALL);
    GLOBAL_bCreated = true;
  }
} // Create


/////////////////////////////////////////////////////////////////////////////
void Initialize() {
  Create();
  if (GLOBAL_bInitialized == false)  {
    ::BASE::Initialize();
    ::DATASTRUCTURE::Initialize();
    ::RESOURCE::Initialize();
    GLOBAL_bInitialized = true;
  }
} // Initialize


/////////////////////////////////////////////////////////////////////////////
void Shutdown(T_BOOLEAN bImmediate) {
  if (GLOBAL_bInitialized == true)  {
    ::RESOURCE::Shutdown(bImmediate);
    ::DATASTRUCTURE::Shutdown(bImmediate);
    ::BASE::Shutdown(bImmediate);
    GLOBAL_bInitialized = false;
  }
} // Shutdown


/////////////////////////////////////////////////////////////////////////////
void Destroy() {
  Shutdown(true);
  if (GLOBAL_bCreated == true)  {
    curl_global_cleanup();
    ::RESOURCE::Destroy();
    ::DATASTRUCTURE::Destroy();
    ::BASE::Destroy();
    GLOBAL_bCreated = false;
  }
} // Destroy

} // namespace WSR
