
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

DERIVE_ABSTRACT_EXCEPTION(::BASE::ELoopThread, EScraper);
  
class WSR_EXPORT_IMPORT CScraper :
  public ::BASE::CLoopThread {
private:
  MEMBER__REFERENCE(::BASE::IObject, Reader);
  MEMBER__REFERENCE(::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> >, Domains);
  MEMBER__REFERENCE(::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> >, Emails);
  MEMBER__REFERENCE(::BASE::IObject, Writer);

  //! copy constructor
  CScraper(const CScraper & tScraper);
  //! assignment operator
  CScraper & operator=(const CScraper & tScraper);

protected:
  //! loop
  virtual void Loop();

public:
  //! constructor
  CScraper(::BASE::IObject * pReader, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pDomains, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pEmails, ::BASE::IObject * pWriter);
  //! destructor
  virtual ~CScraper();
}; // class WSR_EXPORT_IMPORT CScraper

} // namespace WSR

#endif // #ifndef _WSR_CSCRAPER
