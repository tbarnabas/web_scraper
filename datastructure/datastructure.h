
/////////////////////////////////////////////////////////////////////////////
//
// datastructure.h - module header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
// MODULE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE
#define _DATASTRUCTURE

#include "datastructure_configuration.h"

#include "datastructure_carray.h"
#include "datastructure_clist.h"
#include "datastructure_cmap.h"
#include "datastructure_cpuffer.h"
#include "datastructure_cqueue.h"
#include "datastructure_crawstream.h"
#include "datastructure_cring.h"
#include "datastructure_cstack.h"
#include "datastructure_cstream.h"
#include "datastructure_ctextstream.h"

#if (DE_FAMILY == DEF_ARDUINO)
#include "datastructure.cpp"
#endif // #if (DE_FAMILY == DEF_ARDUINO)

namespace DATASTRUCTURE {

//! create module
void Create();
//! initialize module
void Initialize();
//! shutdown module
void Shutdown(T_BOOLEAN bImmediate = false);
//! destroy module
void Destroy();

} // namespace DATASTRUCTURE

#endif // #ifndef _DATASTRUCTURE
