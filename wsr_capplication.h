
/////////////////////////////////////////////////////////////////////////////
//
// wsr_capplication.h - ::WSR::CApplication class header
// --------------------------------------------------------------------------
//
// WEB SCARPER
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WSR_CAPPLICATION
#define _WSR_CAPPLICATION

#include "wsr_configuration.h"

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
  MEMBER_GET(T_ULONG, Scarper);
  MEMBER_GET(T_STRING, Input);
  MEMBER_GET(T_STRING, Output);
  MEMBER_GET(T_ULONG, Depth);

public:
  //! constructor
  CApplication(T_ULONG uScarper, T_STRING sInput, T_STRING sOutput, T_ULONG uDepth);
  //! destructor
  virtual ~CApplication();

  //! initialize
  virtual void Initialize();
}; // class WSR_EXPORT_IMPORT CApplication

} // namespace WSR

#endif // #ifndef _WSR_CAPPLICATION
