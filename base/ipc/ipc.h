
/////////////////////////////////////////////////////////////////////////////
//
// ipc.h - module header
// --------------------------------------------------------------------------
//
// INTER PROCESS COMMUNICATION
//
// MODULE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _IPC
#define _IPC

#include "ipc_configuration.h"

#include "ipv4/ipc_ipv4_csocket.h"

//#include "ipc_cpacket.h"
//#include "ipc_crfm12b.h"
//#include "ipc_csocket.h"
//#include "ipc_cspi.h"

//#if (DE_FAMILY == DEF_ARDUINO)
//#include "ipc.cpp"
//#endif // #if (DE_FAMILY == DEF_ARDUINO)

namespace IPC {

//! create module
void Create();
//! initialize module
void Initialize();
//! shutdown module
void Shutdown(T_BOOLEAN bImmediate = false);
//! destroy module
void Destroy();

} // namespace IPC

#endif // #ifndef _IPC
