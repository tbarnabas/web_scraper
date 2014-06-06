
/////////////////////////////////////////////////////////////////////////////
//
// wsr.h - module header
// --------------------------------------------------------------------------
//
// WEB SCRAPER
//
// MODULE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WSR
#define _WSR

#include "wsr_configuration.h"

#include "wsr_capplication.h"
#include "wsr_creader.h"
#include "wsr_cscraper.h"
#include "wsr_ctask.h"
#include "wsr_cwriter.h"

namespace WSR {

//! create module
void Create();
//! initialize module
void Initialize();

//! shutdown module
void Shutdown(T_BOOLEAN bImmediate = false);
//! destroy module
void Destroy();

} // namespace WSR

#endif // ifndef _WSR
