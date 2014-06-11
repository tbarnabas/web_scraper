
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cscraper.cpp - ::WSR::CScraper class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_cscraper.h"

namespace WSR {

::DATASTRUCTURE::CMap<T_STRING, T_STRING> CScraper::STATIC_tLock;
T_ULONG CScraper::STATIC_uWaitingScrapers = 0;
T_ULONG CScraper::STATIC_uWorkingScrapers = 0;
T_ULONG CScraper::STATIC_uDownloadingScrapers = 0;
T_ULONG CScraper::STATIC_uRecognizingDomainScrapers = 0;
T_ULONG CScraper::STATIC_uRecognizingEmailScrapers = 0;

T_ULONG CScraper::STATIC_uTotalDownloadTry = 0;
T_ULONG CScraper::STATIC_uAccessedHomePages = 0;
T_ULONG CScraper::STATIC_uUnreachableHomePages = 0;

T_ULONG CScraper::STATIC_uEnqueuedDomains = 0;
T_ULONG CScraper::STATIC_uRecognizedDomains = 0;
T_ULONG CScraper::STATIC_uRecognizedEmails = 0;

  
/////////////////////////////////////////////////////////////////////////////
size_t CScraper::ReceiveData(void * pContents, size_t uSize, size_t uLength, void * pData) {
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > tArray = static_cast< ::DATASTRUCTURE::CArray<T_BYTE> *>(pData);
  T_ULONG uOriginalSize = tArray->GetSize();
  T_ULONG uExtentSize = uSize * uLength;
  
  tArray->Append(NULL, uExtentSize + 1);

  ::DATASTRUCTURE::CArray<T_BYTE>::Copy((T_BYTE *)pContents, tArray->GetElements() + uOriginalSize, uExtentSize); 
  
  return (uExtentSize);
} // ReceiveData


/////////////////////////////////////////////////////////////////////////////
REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > CScraper::DownloadHomePage(const T_STRING & sAddress) {
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > tResult;
  T_STRING sDomain = sAddress;
   
  // create a new array
  tResult.Create(new ::DATASTRUCTURE::CArray<T_BYTE>());

  CURL * pCURL= NULL;
  CURLcode tCURLResult;

  // initialize CURL session
  pCURL = curl_easy_init();
  if (pCURL != NULL) {
    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_uDownloadingScrapers = STATIC_uDownloadingScrapers + 1;
    }
    
    // set CURL options
    curl_easy_setopt(pCURL, CURLOPT_URL, C_STR(sDomain));
    curl_easy_setopt(pCURL, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(pCURL, CURLOPT_USERAGENT, "wsr-agent/1.0");
    curl_easy_setopt(pCURL, CURLOPT_AUTOREFERER, 1);
    curl_easy_setopt(pCURL, CURLOPT_MAXREDIRS, 5);

    curl_easy_setopt(pCURL, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(pCURL, CURLOPT_CONNECTTIMEOUT, 5);

    curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, ReceiveData);    
    curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, (void *)tResult.__ptr());
 
//    printf("WSR::CScraper::DownloadHomePage() > connecting to server (address=%s)\n", C_STR(sAddress)); 
    
    // perform CURL operation
    tCURLResult = curl_easy_perform(pCURL);
    
    // clean up CURL session
    curl_easy_cleanup(pCURL);
    
    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_uDownloadingScrapers = STATIC_uDownloadingScrapers - 1;
      STATIC_uTotalDownloadTry = STATIC_uTotalDownloadTry + 1;
      if (tCURLResult == CURLE_OK) {
        STATIC_uAccessedHomePages = STATIC_uAccessedHomePages + 1;
        printf("WSR::CScraper::DownloadHomePage() > page (address=%s) downloaded\n", C_STR(sAddress));
      } else {      
        STATIC_uUnreachableHomePages = STATIC_uUnreachableHomePages + 1;
        printf("WSR::CScraper::DownloadHomePage() > ERROR: unable to connect (address=%s, error=%d)\n", C_STR(sAddress), tCURLResult); 
      }
    }
  }

  
  return (tResult);
} // DownloadHomePage

  
/////////////////////////////////////////////////////////////////////////////
void CScraper::RecognizeEmails(::DATASTRUCTURE::CArray<T_BYTE> * pPage, ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pEmails) {
  DATASTRUCTURE::CMap<T_STRING, REFERENCE<CTask> > tEmails;

  regex_t tEmail;
  
  // compile regular expression
  if (regcomp(&tEmail, "[a-z][a-z0-9_-]*@[a-z][a-z0-9_-]*[\\.[a-z][a-z0-9_-]*]*", REG_ICASE | REG_EXTENDED) == 0) { 
    regmatch_t tMatch;
    
    T_BOOLEAN bMatch = true;
    T_ULONG uPosition = 0;
    T_CHAR pBuffer[256];
    
    while (bMatch == true) {
      if (regexec(&tEmail, (T_CHAR *)pPage->GetElements() + uPosition, 1, &tMatch, 0) == 0) {
        T_ULONG uLength = tMatch.rm_eo - tMatch.rm_so + 1;
        if (uLength > sizeof(pBuffer) - 1) {
          uLength = sizeof(pBuffer) - 1;
        }
        snprintf(pBuffer, uLength, "%s", pPage->GetElements() + uPosition + tMatch.rm_so);
        pBuffer[uLength] = '\0';
        tEmails.Insert(pBuffer, REFERENCE<CTask>().Create(new CTask(pBuffer, 0)));
        uPosition = uPosition + tMatch.rm_eo;
      } else {
        bMatch = false;
      }
    }
  
    // free regular expression
    regfree(&tEmail);
  }
  
  // collect result
  while (tEmails.IsEmpty() == false) {
    pEmails->Push(tEmails.RemoveElement(0));
    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_uRecognizedEmails = STATIC_uRecognizedEmails + 1;
    }
  }
} // RecognizeEmails


/////////////////////////////////////////////////////////////////////////////
REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > CScraper::RecognizeDomains(::DATASTRUCTURE::CArray<T_BYTE> * pPage, T_ULONG uDepth) {
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tResult;
  DATASTRUCTURE::CMap<T_STRING, REFERENCE<CTask> > tDomains;

  // create a new queue
  tResult.Create(new ::DATASTRUCTURE::CQueue<REFERENCE<CTask> >());
  
  regex_t tDomain;
  
  // compile regular expression
  if (regcomp(&tDomain, "http[s]*://[a-z][a-z0-9_-]*\\.[a-z][a-z0-9_-]*[\\.[a-z][a-z0-9_-]*]*", REG_ICASE | REG_EXTENDED) == 0) { 
    regmatch_t tMatch;
    
    T_BOOLEAN bMatch = true;
    T_ULONG uPosition = 0;
    T_CHAR pBuffer[256];
    
    while (bMatch == true) {
      if (regexec(&tDomain, (T_CHAR *)pPage->GetElements() + uPosition, 1, &tMatch, 0) == 0) {
        T_ULONG uLength = tMatch.rm_eo - tMatch.rm_so + 1;
        if (uLength > sizeof(pBuffer) - 1) {
          uLength = sizeof(pBuffer) - 1;
        }
        snprintf(pBuffer, uLength, "%s", pPage->GetElements() + uPosition + tMatch.rm_so);
        pBuffer[uLength] = '\0';
        tDomains.Insert(pBuffer, REFERENCE<CTask>().Create(new CTask(pBuffer, uDepth)));
        uPosition = uPosition + tMatch.rm_eo;
      } else {
        bMatch = false;
      }
    }
  
    // free regular expression
    regfree(&tDomain);
  }
  
  // collect result
  while (tDomains.IsEmpty() == false) {
    tResult->Push(tDomains.RemoveElement(0));
    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_uRecognizedDomains = STATIC_uRecognizedDomains + 1;
    }
  }
  
  return (tResult);
} // RecognizeDomains


/////////////////////////////////////////////////////////////////////////////
REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > CScraper::Process(CTask * pDomain, ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pEmails) {
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tResult(pEmails);
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > tPage;
  
  // create a new queue
  if (tResult.IsValid() == false) {
    tResult.Create(new ::DATASTRUCTURE::CQueue<REFERENCE<CTask> >());
  }

  // download home page
  tPage = DownloadHomePage(pDomain->GetAddress());

  // recognize emails
  if (tPage->IsValid() == true) {
    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_uRecognizingEmailScrapers = STATIC_uRecognizingEmailScrapers + 1;
    }
    RecognizeEmails(tPage, tResult);
    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_uRecognizingEmailScrapers = STATIC_uRecognizingEmailScrapers - 1;
    }

    // recognize domains
    if (pDomain->GetDepth() > 0) {
      REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tDomains;
      
      {
        GUARD __tGuard(STATIC_tLock);
        STATIC_uRecognizingDomainScrapers = STATIC_uRecognizingDomainScrapers + 1;
      }
      tDomains = RecognizeDomains(tPage, pDomain->GetDepth() - 1);  
      {
        GUARD __tGuard(STATIC_tLock);
        STATIC_uRecognizingDomainScrapers = STATIC_uRecognizingDomainScrapers - 1;
      }

      // process domains
      while (tDomains->IsEmpty() == false) {
        tResult = Process(tDomains->Pop(), tResult);
      }
    }
  }

  return (tResult);
} // Process
  
  
/////////////////////////////////////////////////////////////////////////////
void CScraper::Loop() {
  REFERENCE<CTask> tDomain;
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tEmails;
  
  {
    GUARD __tGuard(STATIC_tLock);
    STATIC_uWaitingScrapers = STATIC_uWaitingScrapers + 1;
    STATIC_tLock.Broadcast();
    printf("WSR::CScraper::Loop() > STATE: wait=%d, work=%d, down=%d rec_dom=%d, rec_email=%d | RESULT: in_dom=%d, rec_dom=%d, rec_email=%d | DOWNLOAD: total=%d, access=%d, error=%d\n", STATIC_uWaitingScrapers, STATIC_uWorkingScrapers, STATIC_uDownloadingScrapers, STATIC_uRecognizingDomainScrapers, STATIC_uRecognizingEmailScrapers, STATIC_uEnqueuedDomains, STATIC_uRecognizedDomains, STATIC_uRecognizedEmails, STATIC_uTotalDownloadTry, STATIC_uAccessedHomePages, STATIC_uUnreachableHomePages); 
  }

  // dequeue domain
  m_DomainsConsumers->Acquire();
  {
    GUARD __tGuard(m_Domains);
    tDomain = m_Domains->Pop();
  }
  m_DomainsProducers->Release();

  {
    GUARD __tGuard(STATIC_tLock);
    STATIC_uWaitingScrapers = STATIC_uWaitingScrapers - 1;
    STATIC_uWorkingScrapers = STATIC_uWorkingScrapers + 1;
    STATIC_tLock.Broadcast();
    printf("WSR::CScraper::Loop() > STATE: wait=%d, work=%d, down=%d rec_dom=%d, rec_email=%d | RESULT: in_dom=%d, rec_dom=%d, rec_email=%d | DOWNLOAD: total=%d, access=%d, error=%d\n", STATIC_uWaitingScrapers, STATIC_uWorkingScrapers, STATIC_uDownloadingScrapers, STATIC_uRecognizingDomainScrapers, STATIC_uRecognizingEmailScrapers, STATIC_uEnqueuedDomains, STATIC_uRecognizedDomains, STATIC_uRecognizedEmails, STATIC_uTotalDownloadTry, STATIC_uAccessedHomePages, STATIC_uUnreachableHomePages); 
  } 

  if (tDomain->GetAddress() != "QUIT") {
    printf("WSR::CScraper::Loop() > processing domain (address=%s) ..\n", C_STR(tDomain->GetAddress())); 

    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_uEnqueuedDomains = STATIC_uEnqueuedDomains + 1;
      STATIC_tLock.Broadcast();
      printf("WSR::CScraper::Loop() > STATE: wait=%d, work=%d, down=%d rec_dom=%d, rec_email=%d | RESULT: in_dom=%d, rec_dom=%d, rec_email=%d | DOWNLOAD: total=%d, access=%d, error=%d\n", STATIC_uWaitingScrapers, STATIC_uWorkingScrapers, STATIC_uDownloadingScrapers, STATIC_uRecognizingDomainScrapers, STATIC_uRecognizingEmailScrapers, STATIC_uEnqueuedDomains, STATIC_uRecognizedDomains, STATIC_uRecognizedEmails, STATIC_uTotalDownloadTry, STATIC_uAccessedHomePages, STATIC_uUnreachableHomePages); 
    } 

    // process domain
    IGNORE_EXCEPTION(tEmails = Process(tDomain));
    
    if (tEmails.IsValid() == true) {
      while (tEmails->IsEmpty() == false) {
        m_EmailsProducers->Acquire();
        {
          GUARD __tGuard(m_Emails);
          m_Emails->Push(tEmails->Pop());
        }
        m_EmailsConsumers->Release();
      }
    }
  } else {
    m_Shutdown = true;
    printf("WSR::CScraper::Loop() > shutting down\n");    
  }

  {
    GUARD __tGuard(STATIC_tLock);
    STATIC_uWorkingScrapers = STATIC_uWorkingScrapers - 1;
    if ((m_Shutdown == true) && (STATIC_uWorkingScrapers == 0)) {
       m_EmailsProducers->Acquire();
      {
        GUARD __tGuard(m_Emails);
        m_Emails->Push(tDomain);
      }
      m_EmailsConsumers->Release();
    }
    STATIC_tLock.Broadcast();
    printf("WSR::CScraper::Loop() > STATE: wait=%d, work=%d, down=%d rec_dom=%d, rec_email=%d | RESULT: in_dom=%d, rec_dom=%d, rec_email=%d | DOWNLOAD: total=%d, access=%d, error=%d\n", STATIC_uWaitingScrapers, STATIC_uWorkingScrapers, STATIC_uDownloadingScrapers, STATIC_uRecognizingDomainScrapers, STATIC_uRecognizingEmailScrapers, STATIC_uEnqueuedDomains, STATIC_uRecognizedDomains, STATIC_uRecognizedEmails, STATIC_uTotalDownloadTry, STATIC_uAccessedHomePages, STATIC_uUnreachableHomePages); 
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CScraper::CScraper(::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pDomains, ::BASE::IObject * DomainsProducers, ::BASE::IObject * DomainsConsumers, ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pEmails, ::BASE::IObject * EmailsProducers, ::BASE::IObject * EmailsConsumers) :
  ::BASE::CLoopThread(::BASE::IObject::NON_BLOCKED),
  m_Domains(pDomains),
  m_DomainsProducers(DomainsProducers),
  m_DomainsConsumers(DomainsConsumers),
  m_Emails(pEmails),
  m_EmailsProducers(EmailsProducers),
  m_EmailsConsumers(EmailsConsumers) {
} // CScraper


/////////////////////////////////////////////////////////////////////////////
CScraper::~CScraper() {
} // ~CScraper

} // namespace WSR
