
/////////////////////////////////////////////////////////////////////////////
//
// thread_configuration.h - configuration header
// --------------------------------------------------------------------------
//
// THREAD
//
// CONFIGURATION
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _THREAD_CONFIGURATION
#define _THREAD_CONFIGURATION

#include "thread_include.h"
#include "thread_macro.h"

/////////////////////////////////////////////////////////////////////////////
// export/import

#define THREAD_EXPORT_IMPORT

#if (OS_FAMILY == OSF_WINDOWS)

#if (!defined(THREAD_STATIC) && !defined(THREAD_EXPORT) && !defined(THREAD_IMPORT))
#define THREAD_STATIC
#endif // #if (!defined(THREAD_STATIC) && !defined(THREAD_EXPORT) && !defined(THREAD_IMPORT))

#ifdef THREAD_STATIC
#undef THREAD_EXPORT_IMPORT
#define THREAD_EXPORT_IMPORT
#define THREAD_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef THREAD_STATIC

#ifdef THREAD_EXPORT
#undef THREAD_EXPORT_IMPORT
#define THREAD_EXPORT_IMPORT __declspec(dllexport)
#define THREAD_EXPORT_IMPORT_TEMPLATE
#endif // #ifdef THREAD_EXPORT

#ifdef THREAD_IMPORT
#undef THREAD_EXPORT_IMPORT
#define THREAD_EXPORT_IMPORT __declspec(dllimport)
#define THREAD_EXPORT_IMPORT_TEMPLATE extern
#endif // #ifdef THREAD_IMPORT

#endif // #if (OS_FAMILY == OSF_WINDOWS)

#endif // #ifndef _THREAD_CONFIGURATION
