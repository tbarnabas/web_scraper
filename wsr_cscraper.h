
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
  MEMBER__REFERENCE(::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> >, Domains);
  MEMBER__REFERENCE(::BASE::IObject, DomainsProducers);
  MEMBER__REFERENCE(::BASE::IObject, DomainsConsumers);
  MEMBER__REFERENCE(::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> >, Emails);
  MEMBER__REFERENCE(::BASE::IObject, EmailsProducers);
  MEMBER__REFERENCE(::BASE::IObject, EmailsConsumers);

  //! copy constructor
  CScraper(const CScraper & tScraper);
  //! assignment operator
  CScraper & operator=(const CScraper & tScraper);

protected:
  //! loop
  virtual void Loop();

public:
  //! constructor
  CScraper(::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pDomains, ::BASE::IObject * DomainsProducers, ::BASE::IObject * DomainsConsumers, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pEmails, ::BASE::IObject * EmailsProducers, ::BASE::IObject * EmailsConsumers);
  //! destructor
  virtual ~CScraper();
}; // class WSR_EXPORT_IMPORT CScraper

} // namespace WSR

#endif // #ifndef _WSR_CSCRAPER
