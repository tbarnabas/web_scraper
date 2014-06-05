
/////////////////////////////////////////////////////////////////////////////
//
// datastructure.cpp - module source
//
/////////////////////////////////////////////////////////////////////////////

#include "datastructure.h"

namespace DATASTRUCTURE {

T_ULONG GLOBAL_uCreated = 0;
T_ULONG GLOBAL_uInitialized = 0;

/////////////////////////////////////////////////////////////////////////////
void Create() {
  if (GLOBAL_uCreated == 0) {
    ::BASE::Create();
    ::THREAD::Create();
  }
  GLOBAL_uCreated = GLOBAL_uCreated + 1;
} // Create


/////////////////////////////////////////////////////////////////////////////
void Initialize() {
  Create();
  if (GLOBAL_uInitialized == 0) {
    ::BASE::Initialize();
    ::THREAD::Initialize();
  }
  GLOBAL_uInitialized = GLOBAL_uInitialized + 1;
} // Initialize


/////////////////////////////////////////////////////////////////////////////
void Shutdown(T_BOOLEAN bImmediate) {
  GLOBAL_uInitialized = GLOBAL_uInitialized - 1;
  if (GLOBAL_uInitialized == 0) {
    ::THREAD::Shutdown(bImmediate);
    ::BASE::Shutdown(bImmediate);
  }
} // Shutdown


/////////////////////////////////////////////////////////////////////////////
void Destroy() {
  Shutdown(true);
  GLOBAL_uCreated = GLOBAL_uCreated - 1;
  if (GLOBAL_uCreated == 0) {
    ::THREAD::Destroy();
    ::BASE::Destroy();
  }
} // Destroy

} // namespace DATASTRUCTURE
