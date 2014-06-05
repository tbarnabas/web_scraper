
/////////////////////////////////////////////////////////////////////////////
//
// thread.h - module header
// --------------------------------------------------------------------------
//
// THREAD
//
// MODULE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _THREAD
#define _THREAD

#include "thread_configuration.h"

#include "thread_ccondition.h"
#include "thread_cguard.h"
#include "thread_cmutex.h"
#include "thread_cloopthread.h"
#include "thread_cobject.h"
#include "thread_cthread.h"

#if (DE_FAMILY == DEF_ARDUINO)
#include "thread.cpp"
#endif // #if (DE_FAMILY == DEF_ARDUINO)

namespace THREAD {

//! create module
void Create();
//! initialize module
void Initialize();
//! shutdown module
void Shutdown(T_BOOLEAN bImmediate = false);
//! destroy module
void Destroy();

} // namespace THREAD

#endif // #ifndef _THREAD
