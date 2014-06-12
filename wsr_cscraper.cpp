
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cscraper.cpp - ::WSR::CScraper class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_cscraper.h"

namespace WSR {

::DATASTRUCTURE::CMap<T_STRING, T_STRING> CScraper::STATIC_tLock;
T_ULONG CScraper::STATIC_uRunningScrapers = 0;
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

T_ULONG CScraper::STATIC_uMaximumTIMEWAITSockets = 0;
T_ULONG CScraper::STATIC_uCurrentTIMEWAITSockets = 0;
  
/////////////////////////////////////////////////////////////////////////////
T_ULONG CScraper::GetTIMEWAITSocketNumber() {
  FILE * pPipe = NULL;
  pPipe = popen("netstat -na | grep tcp | grep TIME_WAIT | wc -l", "r");
  if (pPipe != NULL) {
    __T_INT uNumber = 0;
    if (fscanf(pPipe, "%d", &uNumber) == 1) {
      STATIC_uCurrentTIMEWAITSockets = uNumber;
      if (STATIC_uCurrentTIMEWAITSockets > STATIC_uMaximumTIMEWAITSockets) {
        STATIC_uMaximumTIMEWAITSockets = STATIC_uCurrentTIMEWAITSockets;
      }
    }    
    pclose(pPipe);
  }
  
  return (STATIC_uCurrentTIMEWAITSockets);
} // GetTIMEWAITSocketNumber


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
      while ((STATIC_tLock.Exists(sDomain) == true) || (GetTIMEWAITSocketNumber() > m_TIMEWAITSockets)) {
//        printf("WSR::CScraper::DownloadHomePage() > waiting ...\n");
        STATIC_tLock.Wait();
      }
      STATIC_tLock.Insert(sDomain, sDomain);
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
      STATIC_tLock.Erase(sDomain);    
      STATIC_uDownloadingScrapers = STATIC_uDownloadingScrapers - 1;
      STATIC_uTotalDownloadTry = STATIC_uTotalDownloadTry + 1;
      if (tCURLResult == CURLE_OK) {
        STATIC_uAccessedHomePages = STATIC_uAccessedHomePages + 1;
        printf("WSR::CScraper::DownloadHomePage() > page (address=%s) downloaded\n", C_STR(sAddress));
      } else {      
        STATIC_uUnreachableHomePages = STATIC_uUnreachableHomePages + 1;
        printf("WSR::CScraper::DownloadHomePage() > ERROR: unable to connect (address=%s, error=%d)\n", C_STR(sAddress), tCURLResult); 
      }
      STATIC_tLock.Broadcast();
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

#define __CScraper__Loop__STATUS_LOG \
  printf("WSR::CScraper::Loop() > -----------------------\n"); \
  printf("WSR::CScraper::Loop() > WORKER   : running=%d, wait=%d, work=%d, down=%d, rec_dom=%d, rec_email=%d\n", STATIC_uRunningScrapers, STATIC_uWaitingScrapers, STATIC_uWorkingScrapers, STATIC_uDownloadingScrapers, STATIC_uRecognizingDomainScrapers, STATIC_uRecognizingEmailScrapers); \
  printf("WSR::CScraper::Loop() > RESULT   : in_dom=%d, rec_dom=%d, rec_email=%d\n", STATIC_uEnqueuedDomains, STATIC_uRecognizedDomains, STATIC_uRecognizedEmails); \
  printf("WSR::CScraper::Loop() > DOWNLOAD : total=%d, access=%d, unreach=%d\n", STATIC_uTotalDownloadTry, STATIC_uAccessedHomePages, STATIC_uUnreachableHomePages); \
  printf("WSR::CScraper::Loop() > NETWORK  : max_tw=%d, curr_tw=%d, limit_tw=%d\n", STATIC_uMaximumTIMEWAITSockets, STATIC_uCurrentTIMEWAITSockets, m_TIMEWAITSockets); 
  
void CScraper::Loop() {
  REFERENCE<CTask> tDomain;
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tEmails;
  
  {
    GUARD __tGuard(STATIC_tLock);
    STATIC_uWaitingScrapers = STATIC_uWaitingScrapers + 1;
    STATIC_tLock.Broadcast();
    __CScraper__Loop__STATUS_LOG; 
  }

  __DEQUEUE_DOMAINS(tDomain);

  {
    GUARD __tGuard(STATIC_tLock);
    STATIC_uWaitingScrapers = STATIC_uWaitingScrapers - 1;
    STATIC_uWorkingScrapers = STATIC_uWorkingScrapers + 1;
    STATIC_tLock.Broadcast();
    __CScraper__Loop__STATUS_LOG; 
  } 

  if (tDomain->GetAddress() != "QUIT") {
    printf("WSR::CScraper::Loop() > processing domain (address=%s) ..\n", C_STR(tDomain->GetAddress())); 

    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_uEnqueuedDomains = STATIC_uEnqueuedDomains + 1;
      STATIC_tLock.Broadcast();
    __CScraper__Loop__STATUS_LOG; 
    } 

    // process domain
    IGNORE_EXCEPTION(tEmails = Process(tDomain));
    
    if (tEmails.IsValid() == true) {
      while (tEmails->IsEmpty() == false) {
        __ENQUEUE_EMAILS(tEmails->Pop());
      }
    }
  } else {
    GUARD __tGuard(STATIC_tLock);
    m_Shutdown = true;
    printf("WSR::CScraper::Loop() > shutting down\n");        
  }

  {
    GUARD __tGuard(STATIC_tLock);
    STATIC_uWorkingScrapers = STATIC_uWorkingScrapers - 1;
    if (m_Shutdown == true) {
      STATIC_uRunningScrapers = STATIC_uRunningScrapers - 1;
      if (STATIC_uRunningScrapers == 0) {
        __ENQUEUE_EMAILS(tDomain);
      }
    }
    STATIC_tLock.Broadcast();
    __CScraper__Loop__STATUS_LOG; 
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CScraper::CScraper(T_ULONG uTIMEWAITSockets, ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pDomains, ::BASE::IObject * DomainsProducers, ::BASE::IObject * DomainsConsumers, ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pEmails, ::BASE::IObject * EmailsProducers, ::BASE::IObject * EmailsConsumers) :
  ::BASE::CLoopThread(::BASE::IObject::NON_BLOCKED),
  m_TIMEWAITSockets(uTIMEWAITSockets),
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
