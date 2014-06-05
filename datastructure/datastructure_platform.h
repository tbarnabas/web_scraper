
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_platform.h - platform header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
// PLATFORM
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE_PLATFORM
#define _DATASTRUCTURE_PLATFORM

/////////////////////////////////////////////////////////////////////////////
// enable standard template library (STL)

#if ((OS_FAMILY == OSF_WINDOWS) | (OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))
#if (!defined(ENABLE_STL))
#define ENABLE_STL 1
#endif // #if (!defined(ENABLE_STL))
#endif // #if ((OS_FAMILY == OSF_WINDOWS) | (OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

#endif // #ifndef _DATASTRUCTURE_PLATFORM
