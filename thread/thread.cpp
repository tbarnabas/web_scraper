
/////////////////////////////////////////////////////////////////////////////
//
// thread.cpp - module source
//
/////////////////////////////////////////////////////////////////////////////

#include "thread.h"

#if (DE_FAMILY == DEF_ARDUINO)
#include "thread_ccondition.cpp"
#include "thread_cguard.cpp"
#include "thread_cmutex.cpp"
#include "thread_cloopthread.cpp"
#include "thread_cobject.cpp"
#include "thread_cthread.cpp"
#endif // #if (DE_FAMILY == DEF_ARDUINO)

namespace THREAD {

T_ULONG GLOBAL_uCreated = 0;
T_ULONG GLOBAL_uInitialized = 0;

/////////////////////////////////////////////////////////////////////////////
void Create() {
  if (GLOBAL_uCreated == 0) {
    ::BASE::Create();
  }
  GLOBAL_uCreated = GLOBAL_uCreated + 1;
} // Create


/////////////////////////////////////////////////////////////////////////////
void Initialize() {
  Create();
  if (GLOBAL_uInitialized == 0) {
    ::BASE::Initialize();
  }
  GLOBAL_uInitialized = GLOBAL_uInitialized + 1;
} // Initialize


/////////////////////////////////////////////////////////////////////////////
void Shutdown(T_BOOLEAN bImmediate) {
  GLOBAL_uInitialized = GLOBAL_uInitialized - 1;
  if (GLOBAL_uInitialized == 0) {
    ::BASE::Shutdown(bImmediate);
  }
} // Shutdown


/////////////////////////////////////////////////////////////////////////////
void Destroy() {
  Shutdown(true);
  GLOBAL_uCreated = GLOBAL_uCreated - 1;
  if (GLOBAL_uCreated == 0) {
    ::BASE::Destroy();
  }
} // Destroy

} // namespace THREAD
