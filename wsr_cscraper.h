
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cscraper.h - ::WSR::CScraper class header
// --------------------------------------------------------------------------
//
// WEB SCRAPER
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WSR_CSCRAPER
#define _WSR_CSCRAPER

#include "wsr_configuration.h"

#include "wsr_ctask.h"

namespace WSR {

DERIVE_ABSTRACT_EXCEPTION(::WORKFLOW::EWorker, EScraper);
  
class WSR_EXPORT_IMPORT CScraper :
  public ::WORKFLOW::CWorker<REFERENCE< ::WSR::CTask>, REFERENCE< ::WSR::CTask> > {
public:
  //! constructor
  CScraper();
  //! destructor
  virtual ~CScraper();

  //! copy constructor
  CScraper(const CScraper & tScraper);
  //! assignment operator
  CScraper & operator=(const CScraper & tScraper);
}; // class WSR_EXPORT_IMPORT CScraper

} // namespace WSR

#endif // #ifndef _WSR_CSCRAPER
