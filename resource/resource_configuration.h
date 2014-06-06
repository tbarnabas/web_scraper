
/////////////////////////////////////////////////////////////////////////////
//
// resource_configuration.h - configuration header
// --------------------------------------------------------------------------
//
// RESOURCE
//
// CONFIGURATION
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _RESOURCE_CONFIGURATION
#define _RESOURCE_CONFIGURATION

#include "resource_include.h"
#include "resource_macro.h"

/////////////////////////////////////////////////////////////////////////////
// export/import

#define RESOURCE_EXPORT_IMPORT

#if (OS_FAMILY == OSF_WINDOWS)

#if (!defined(RESOURCE_STATIC) && !defined(RESOURCE_EXPORT) && !defined(RESOURCE_IMPORT))
#define RESOURCE_STATIC
#endif // #if (!defined(RESOURCE_STATIC) && !defined(RESOURCE_EXPORT) && !defined(RESOURCE_IMPORT))

#ifdef RESOURCE_STATIC
#undef RESOURCE_EXPORT_IMPORT
#define RESOURCE_EXPORT_IMPORT
#define RESOURCE_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef RESOURCE_STATIC

#ifdef RESOURCE_EXPORT
#undef RESOURCE_EXPORT_IMPORT
#define RESOURCE_EXPORT_IMPORT __declspec(dllexport)
#define RESOURCE_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef RESOURCE_EXPORT

#ifdef RESOURCE_IMPORT
#undef RESOURCE_EXPORT_IMPORT
#define RESOURCE_EXPORT_IMPORT __declspec(dllimport)
#define RESOURCE_EXPORT_IMPORT_TEMPLATE extern
#endif // #ifdef RESOURCE_IMPORT

#endif // #if (OS_FAMILY == OSF_WINDOWS)

#endif // #ifndef _RESOURCE_CONFIGURATION
