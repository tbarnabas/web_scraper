
/////////////////////////////////////////////////////////////////////////////
//
// wsr_capplication.h - ::WSR::CApplication class header
// --------------------------------------------------------------------------
//
// WEB SCRAPER
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WSR_CAPPLICATION
#define _WSR_CAPPLICATION

#include "wsr_configuration.h"

#include "wsr_creader.h"
#include "wsr_cwriter.h"

namespace WSR {

DERIVE_ABSTRACT_EXCEPTION(::RESOURCE::EApplication, EApplication);
  
class WSR_EXPORT_IMPORT CApplication :
  public ::RESOURCE::CApplication {
private:
  //! copy constructor
  CApplication(const CApplication & tApplication);
  //! assignment operator
  CApplication & operator=(const CApplication & tApplication);

public:
  MEMBER_GET(T_ULONG, Scraper);
  MEMBER_GET(T_STRING, Input);
  MEMBER_GET(T_STRING, Output);
  MEMBER_GET(T_ULONG, Depth);
  
  MEMBER(REFERENCE< ::WSR::CReader>, Reader);
  MEMBER(REFERENCE< ::WSR::CWriter>, Writer);

public:
  //! constructor
  CApplication(T_ULONG uScraper, const T_STRING & sInput, const T_STRING & sOutput, T_ULONG uDepth);
  //! destructor
  virtual ~CApplication();

    //! run
  virtual void Run();
}; // class WSR_EXPORT_IMPORT CApplication

} // namespace WSR

#endif // #ifndef _WSR_CAPPLICATION
