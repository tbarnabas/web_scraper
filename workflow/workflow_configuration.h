
/////////////////////////////////////////////////////////////////////////////
//
// workflow_configuration.h - configuration header
// --------------------------------------------------------------------------
//
// WORKFLOW
//
// CONFIGURATION
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WORKFLOW_CONFIGURATION
#define _WORKFLOW_CONFIGURATION

#include "workflow_include.h"
#include "workflow_macro.h"

/////////////////////////////////////////////////////////////////////////////
// export/import

#define WORKFLOW_EXPORT_IMPORT

#if (OS_FAMILY == OSF_WINDOWS)

#if (!defined(WORKFLOW_STATIC) && !defined(WORKFLOW_EXPORT) && !defined(WORKFLOW_IMPORT))
#define WORKFLOW_STATIC
#endif // #if (!defined(WORKFLOW_STATIC) && !defined(WORKFLOW_EXPORT) && !defined(WORKFLOW_IMPORT))

#ifdef WORKFLOW_STATIC
#undef WORKFLOW_EXPORT_IMPORT
#define WORKFLOW_EXPORT_IMPORT
#define WORKFLOW_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef WORKFLOW_STATIC

#ifdef WORKFLOW_EXPORT
#undef WORKFLOW_EXPORT_IMPORT
#define WORKFLOW_EXPORT_IMPORT __declspec(dllexport)
#define WORKFLOW_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef WORKFLOW_EXPORT

#ifdef WORKFLOW_IMPORT
#undef WORKFLOW_EXPORT_IMPORT
#define WORKFLOW_EXPORT_IMPORT __declspec(dllimport)
#define WORKFLOW_EXPORT_IMPORT_TEMPLATE extern
#endif // #ifdef WORKFLOW_IMPORT

#endif // #if (OS_FAMILY == OSF_WINDOWS)

#endif // #ifndef _WORKFLOW_CONFIGURATION
