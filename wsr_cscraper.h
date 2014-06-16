
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
  UNABLE_TO_DOWNLOAD_HOME_PAGE,
  UNABLE_TO_OPEN_OUTPUT
DERIVE_EXCEPTION_END(EScraper)

class WSR_EXPORT_IMPORT CScraper :
  public ::BASE::CLoopThread {
public:
  //! context
  static ::DATASTRUCTURE::CMap<T_STRING, T_STRING> STATIC_tLock;
  static ::DATASTRUCTURE::CMap<T_STRING, T_STRING> STATIC_tBufferedEmails;

  static T_ULONG STATIC_uRunningScrapers;
  static T_ULONG STATIC_uWaitingScrapers;
  static T_ULONG STATIC_uWorkingScrapers;
  static T_ULONG STATIC_uDownloadingScrapers;
  static T_ULONG STATIC_uRecognizingDomainScrapers;
  static T_ULONG STATIC_uRecognizingEmailScrapers;

  static T_ULONG STATIC_uTotalDownloadTry;
  static T_ULONG STATIC_uAccessedHomePages;
  static T_ULONG STATIC_uUnreachableHomePages ;

  static T_ULONG STATIC_uEnqueuedDomains;
  static T_ULONG STATIC_uRecognizedDomains;
  static T_ULONG STATIC_uRecognizedEmails;

  static T_ULONG STATIC_uMaximumTIMEWAITSockets;
  static T_ULONG STATIC_uCurrentTIMEWAITSockets;
  static T_ULONG STATIC_uTIMEWAITSockets;
  
private:
  MEMBER(::std::ofstream, Output);
  MEMBER__REFERENCE(::DATASTRUCTURE::CQueue<REFERENCE<CTask> >, Domains);
  MEMBER__REFERENCE(::BASE::IObject, DomainsProducers);
  MEMBER__REFERENCE(::BASE::IObject, DomainsConsumers);

  //! copy constructor
  CScraper(const CScraper & tScraper);
  //! assignment operator
  CScraper & operator=(const CScraper & tScraper);

protected:
  //! store email task
  void StoreEmail(const T_CHAR * pAddress);
  
  //! get TIME_WAIT socket number
  T_ULONG GetTIMEWAITSocketNumber();
  //! receive data
  static size_t ReceiveData(void * pContents, size_t uSize, size_t uLength, void * pData);
  //! download home page
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > DownloadHomePage(const T_STRING & sAddress);
  //! recognize emails
  void RecognizeEmails(::DATASTRUCTURE::CArray<T_BYTE> * pPage);
  //! recognize domains
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > RecognizeDomains(::DATASTRUCTURE::CArray<T_BYTE> * pPage, const T_STRING & sDomain, T_ULONG uDepth);
  //! process domain
  void Process(CTask * pDomain);
  //! loop
  virtual void Loop();
  
public:
  //! constructor
  CScraper(const T_STRING & sOutput, ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pDomains, ::BASE::IObject * DomainsProducers, ::BASE::IObject * DomainsConsumers);
  //! destructor
  virtual ~CScraper();
}; // class WSR_EXPORT_IMPORT CScraper

} // namespace WSR

#endif // #ifndef _WSR_CSCRAPER
