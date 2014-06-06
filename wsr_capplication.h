
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

public:
  //! constructor
  CApplication(T_ULONG uScraper, const T_STRING & sInput, const T_STRING & sOutput, T_ULONG uDepth);
  //! destructor
  virtual ~CApplication();

  //! initialize
  virtual void Initialize();
}; // class WSR_EXPORT_IMPORT CApplication

} // namespace WSR

#endif // #ifndef _WSR_CAPPLICATION
