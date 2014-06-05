
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_configuration.h - configuration header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
// CONFIGURATION
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE_CONFIGURATION
#define _DATASTRUCTURE_CONFIGURATION

#include "datastructure_include.h"
#include "datastructure_macro.h"

/////////////////////////////////////////////////////////////////////////////
// export/import

#define DATASTRUCTURE_EXPORT_IMPORT

#if (OS_FAMILY == OSF_WINDOWS)

#if (!defined(DATASTRUCTURE_STATIC) && !defined(DATASTRUCTURE_EXPORT) && !defined(DATASTRUCTURE_IMPORT))
#define DATASTRUCTURE_STATIC
#endif // #if (!defined(DATASTRUCTURE_STATIC) && !defined(DATASTRUCTURE_EXPORT) && !defined(DATASTRUCTURE_IMPORT))

#ifdef DATASTRUCTURE_STATIC
#undef DATASTRUCTURE_EXPORT_IMPORT
#define DATASTRUCTURE_EXPORT_IMPORT
#define DATASTRUCTURE_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef DATASTRUCTURE_STATIC

#ifdef DATASTRUCTURE_EXPORT
#undef DATASTRUCTURE_EXPORT_IMPORT
#define DATASTRUCTURE_EXPORT_IMPORT __declspec(dllexport)
#define DATASTRUCTURE_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef DATASTRUCTURE_EXPORT

#ifdef DATASTRUCTURE_IMPORT
#undef DATASTRUCTURE_EXPORT_IMPORT
#define DATASTRUCTURE_EXPORT_IMPORT __declspec(dllimport)
#define DATASTRUCTURE_EXPORT_IMPORT_TEMPLATE extern
#endif // #ifdef DATASTRUCTURE_IMPORT

#endif // #if (OS_FAMILY == OSF_WINDOWS)

#endif // #ifndef _DATASTRUCTURE_CONFIGURATION
