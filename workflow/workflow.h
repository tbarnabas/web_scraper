
/////////////////////////////////////////////////////////////////////////////
//
// workflow.h - module header
// --------------------------------------------------------------------------
//
// WORKFLOW
//
// MODULE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WORKFLOW
#define _WORKFLOW

#include "workflow_configuration.h"

#include "workflow_cworker.h"

namespace WORKFLOW {

//! create module
void Create();
//! initialize module
void Initialize();
//! shutdown module
void Shutdown(T_BOOLEAN bImmediate = false);
//! destroy module
void Destroy();

} // namespace WORKFLOW

#endif // #ifndef _WORKFLOW
