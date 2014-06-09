
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

DERIVE_EXCEPTION_BEGIN(::BASE::ELoopThread, EScraper)
  UNABLE_TO_DOWNLOAD_HOME_PAGE
DERIVE_EXCEPTION_END(EScraper)

class WSR_EXPORT_IMPORT CScraper :
  public ::BASE::CLoopThread {
private:
  MEMBER__REFERENCE(::DATASTRUCTURE::CQueue<REFERENCE<CTask> >, Domains);
  MEMBER__REFERENCE(::BASE::IObject, DomainsProducers);
  MEMBER__REFERENCE(::BASE::IObject, DomainsConsumers);
  MEMBER__REFERENCE(::DATASTRUCTURE::CQueue<REFERENCE<CTask> >, Emails);
  MEMBER__REFERENCE(::BASE::IObject, EmailsProducers);
  MEMBER__REFERENCE(::BASE::IObject, EmailsConsumers);

  //! copy constructor
  CScraper(const CScraper & tScraper);
  //! assignment operator
  CScraper & operator=(const CScraper & tScraper);

protected:
  //! modular power (based on repeated sqaring on binary form algorithm)
  T_ULONG Modular(T_ULONG uBase, T_ULONG uExponent, T_ULONG uDivisor);
  //! match pattern (based on Rabin-Karp algorithm)
  T_BOOLEAN RabinKarp(const T_BYTE * pBuffer, T_ULONG uBufferSize, const T_BYTE * pPattern, T_ULONG uPatternSize, T_ULONG & uIndex);
  //! recognize string contents
  REFERENCE< ::DATASTRUCTURE::CQueue<T_STRING> > RecognizeStringContents(const T_CHAR * pBuffer, T_ULONG uBufferSize, ::DATASTRUCTURE::CQueue<T_STRING> * pPatterns);

  //! receive data
  static size_t ReceiveData(void * pContents, size_t uSize, size_t uLength, void * pData);
  //! download home page
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > DownloadHomePage(const T_STRING & sAddress);
  //! recognize emails
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > RecognizeEmails(::DATASTRUCTURE::CArray<T_BYTE> * pPage);
  //! recognize domains
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > RecognizeDomains(::DATASTRUCTURE::CArray<T_BYTE> * pPage);

  //! loop
  virtual void Loop();
  
public:
  //! constructor
  CScraper(::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pDomains, ::BASE::IObject * DomainsProducers, ::BASE::IObject * DomainsConsumers, ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pEmails, ::BASE::IObject * EmailsProducers, ::BASE::IObject * EmailsConsumers);
  //! destructor
  virtual ~CScraper();
}; // class WSR_EXPORT_IMPORT CScraper

} // namespace WSR

#endif // #ifndef _WSR_CSCRAPER
