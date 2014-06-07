
/////////////////////////////////////////////////////////////////////////////
//
// wsr_creader.h - ::WSR::CReader class header
// --------------------------------------------------------------------------
//
// WEB SCRAPER
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WSR_CREADER
#define _WSR_CREADER

#include "wsr_configuration.h"

#include "wsr_ctask.h"

namespace WSR {

DERIVE_EXCEPTION_BEGIN(::THREAD::ELoopThread, EReader)
  UNABLE_TO_OPEN
DERIVE_EXCEPTION_END(EReader);
  
class WSR_EXPORT_IMPORT CReader :
  public ::THREAD::CLoopThread {
private:
  MEMBER(T_ULONG, Scrapers);
  MEMBER(::std::ifstream, Input);
  MEMBER(T_ULONG, Depth);
  MEMBER__REFERENCE(::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> >, Domains);

  //! copy constructor
  CReader(const CReader & tReader);
  //! assignment operator
  CReader & operator=(const CReader & tReader);

protected:
  //! loop
  virtual void Loop();

public:
  //! constructor
  CReader(T_ULONG uScrapers, const T_STRING & sInput, T_ULONG uDepth, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pDomains);
  //! destructor
  virtual ~CReader();
}; // class WSR_EXPORT_IMPORT CReader

} // namespace WSR

#endif // #ifndef _WSR_CREADER
