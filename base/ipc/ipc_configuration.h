
/////////////////////////////////////////////////////////////////////////////
//
// ipc_configuration.h - configuration header
// --------------------------------------------------------------------------
//
// INTER PROCESS COMMUNICATION
//
// CONFIGURATION
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _IPC_CONFIGURATION
#define _IPC_CONFIGURATION

#include "ipc_include.h"
#include "ipc_macro.h"

/////////////////////////////////////////////////////////////////////////////
// export/import

#define IPC_EXPORT_IMPORT

#if (OS_FAMILY == OSF_WINDOWS)

#if (!defined(IPC_STATIC) && !defined(IPC_EXPORT) && !defined(IPC_IMPORT))
#define IPC_STATIC
#endif // #if (!defined(IPC_STATIC) && !defined(IPC_EXPORT) && !defined(IPC_IMPORT))

#ifdef IPC_STATIC
#undef IPC_EXPORT_IMPORT
#define IPC_EXPORT_IMPORT
#define IPC_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef IPC_STATIC

#ifdef IPC_EXPORT
#undef IPC_EXPORT_IMPORT
#define IPC_EXPORT_IMPORT __declspec(dllexport)
#define IPC_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef IPC_EXPORT

#ifdef IPC_IMPORT
#undef IPC_EXPORT_IMPORT
#define IPC_EXPORT_IMPORT __declspec(dllimport)
#define IPC_EXPORT_IMPORT_TEMPLATE extern
#endif // #ifdef IPC_IMPORT

#endif // #if (OS_FAMILY == OSF_WINDOWS)

#endif // #ifndef _IPC_CONFIGURATION
