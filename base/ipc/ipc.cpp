
/////////////////////////////////////////////////////////////////////////////
//
// ipc.cpp - module source
//
/////////////////////////////////////////////////////////////////////////////

#include "ipc.h"

//#if (DE_FAMILY == DEF_ARDUINO)
//#include "ipc_crfm12b.cpp"
//#include "ipc_cspi.cpp"
//#endif // #if (DE_FAMILY == DEF_ARDUINO)

namespace IPC {

T_ULONG GLOBAL_uCreated = 0;
T_ULONG GLOBAL_uInitialized = 0;

/////////////////////////////////////////////////////////////////////////////
void Create() {
  if (GLOBAL_uCreated == 0) {
    ::BASE::Create();
    ::DATASTRUCTURE::Create();
  }
  GLOBAL_uCreated = GLOBAL_uCreated + 1;
} // Create


/////////////////////////////////////////////////////////////////////////////
void Initialize() {
  Create();
  if (GLOBAL_uInitialized == 0) {
    ::BASE::Initialize();
    ::DATASTRUCTURE::Initialize();
  }
  GLOBAL_uInitialized = GLOBAL_uInitialized + 1;
} // Initialize


/////////////////////////////////////////////////////////////////////////////
void Shutdown(T_BOOLEAN bImmediate) {
  GLOBAL_uInitialized = GLOBAL_uInitialized - 1;
  if (GLOBAL_uInitialized == 0) {
    ::DATASTRUCTURE::Shutdown(bImmediate);
    ::BASE::Shutdown(bImmediate);
  }
} // Shutdown


/////////////////////////////////////////////////////////////////////////////
void Destroy() {
  Shutdown(true);
  GLOBAL_uCreated = GLOBAL_uCreated - 1;
  if (GLOBAL_uCreated == 0) {
    ::DATASTRUCTURE::Destroy();
    ::BASE::Destroy();
  }
} // Destroy

} // namespace IPC
