
/////////////////////////////////////////////////////////////////////////////
//
// wsr_configuration.h - configuration header
// --------------------------------------------------------------------------
//
// WEB SCRAPER
//
// CONFIGURATION
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WSR_CONFIGURATION
#define _WSR_CONFIGURATION

#include "wsr_include.h"
#include "wsr_macro.h"

/////////////////////////////////////////////////////////////////////////////
// DLL export/import

#define WSR_EXPORT_IMPORT

#if (OS_FAMILY == OSF_WINDOWS)

#if (!defined(WSR_STATIC) && !defined(WSR_EXPORT) && !defined(WSR_IMPORT))
#define WSR_STATIC
#endif // #if (!defined(WSR_STATIC) && !defined(WSR_EXPORT) && !defined(WSR_IMPORT))

#ifdef WSR_STATIC
#undef WSR_EXPORT_IMPORT
#define WSR_EXPORT_IMPORT
#define WSR_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef WSR_STATIC

#ifdef WSR_EXPORT
#undef WSR_EXPORT_IMPORT
#define WSR_EXPORT_IMPORT __declspec(dllexport)
#define WSR_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef WSR_EXPORT

#ifdef WSR_IMPORT
#undef WSR_EXPORT_IMPORT
#define WSR_EXPORT_IMPORT __declspec(dllimport)
#define WSR_EXPORT_IMPORT_TEMPLATE extern
#endif // #ifdef WSR_IMPORT

#endif // #if (OS_FAMILY == OSF_WINDOWS)

#endif // #ifndef _WSR_CONFIGURATION
