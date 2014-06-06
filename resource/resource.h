
/////////////////////////////////////////////////////////////////////////////
//
// resource.h - module header
// --------------------------------------------------------------------------
//
// RESOURCE
//
// MODULE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _RESOURCE
#define _RESOURCE

#include "resource_configuration.h"

#include "resource_capplication.h"
#include "resource_cfile.h"
#include "resource_cobjectmanager.h"
#include "resource_cservice.h"

#if (DE_FAMILY == DEF_ARDUINO)
#include "resource.cpp"
#endif // #if (DE_FAMILY == DEF_ARDUINO)

namespace RESOURCE {

//! create module
void Create();
//! initialize module
void Initialize();
//! shutdown module
void Shutdown(T_BOOLEAN bImmediate = false);
//! destroy module
void Destroy();

} // namespace RESOURCE

#endif // #ifndef _RESOURCE
