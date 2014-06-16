
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cscraper.cpp - ::WSR::CScraper class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_cscraper.h"

namespace WSR {

::DATASTRUCTURE::CMap<T_STRING, T_STRING> CScraper::STATIC_tLock;
::DATASTRUCTURE::CMap<T_STRING, T_STRING> CScraper::STATIC_tBufferedEmails;

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
T_ULONG CScraper::STATIC_uTIMEWAITSockets = 0;
  
/////////////////////////////////////////////////////////////////////////////
void CScraper::StoreEmail(const T_CHAR * pAddress) {
  m_Output << pAddress << ::std::endl;
  m_Output.flush();
} // StoreEmail
  

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
  
  tArray->Append(NULL, uExtentSize);

  ::DATASTRUCTURE::CArray<T_BYTE>::Copy((T_BYTE *)pContents, tArray->GetElements() + uOriginalSize - 1, uExtentSize); 

  (* tArray)[tArray->GetSize() - 1] = '\0';
  
  return (uExtentSize);
} // ReceiveData


/////////////////////////////////////////////////////////////////////////////
REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > CScraper::DownloadHomePage(const T_STRING & sAddress) {
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > tResult;
  T_STRING sDomain = sAddress;
  T_STRING sKey = sDomain;
    
  // create a new array
  tResult.Create(new ::DATASTRUCTURE::CArray<T_BYTE>(1));
  (* tResult)[tResult->GetSize() - 1] = '\0';
 
  CURL * pCURL= NULL;
  CURLcode tCURLResult;

  // initialize CURL session
  pCURL = curl_easy_init();
  if (pCURL != NULL) {
    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_uDownloadingScrapers = STATIC_uDownloadingScrapers + 1;
      while ((STATIC_tLock.Exists(sKey) == true) || (GetTIMEWAITSocketNumber() > STATIC_uTIMEWAITSockets)) {
//        printf("WSR::CScraper::DownloadHomePage() > waiting ...\n");
        STATIC_tLock.Wait();
      }
      STATIC_tLock.Insert(sKey, sKey);
    }
    
    // set CURL options
    curl_easy_setopt(pCURL, CURLOPT_URL, C_STR(sDomain));
    curl_easy_setopt(pCURL, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(pCURL, CURLOPT_USERAGENT, "wsr-agent/1.0");
    curl_easy_setopt(pCURL, CURLOPT_AUTOREFERER, 1);
    curl_easy_setopt(pCURL, CURLOPT_MAXREDIRS, 5);

    curl_easy_setopt(pCURL, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(pCURL, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(pCURL, CURLOPT_NOSIGNAL, 1);

    curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, ReceiveData);    
    curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, (void *)tResult.__ptr());
 
//    printf("WSR::CScraper::DownloadHomePage() > connecting to server (address=%s)\n", C_STR(sAddress)); 
    
    // perform CURL operation
    tCURLResult = curl_easy_perform(pCURL);
    
    // clean up CURL session
    curl_easy_cleanup(pCURL);

    // clean page
    for (T_ULONG i = 0; i < tResult->GetSize() - 1; i++) {
      if (((* tResult)[i] > 127) || ((* tResult)[i] == 0)) {
        (* tResult)[i] = 32;
      }
    }

//    printf("%s\n", tResult->GetElements());
    
    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_tLock.Erase(sKey);    
      STATIC_uDownloadingScrapers = STATIC_uDownloadingScrapers - 1;
      STATIC_uTotalDownloadTry = STATIC_uTotalDownloadTry + 1;
      if (tCURLResult == CURLE_OK) {
        STATIC_uAccessedHomePages = STATIC_uAccessedHomePages + 1;
//        printf("WSR::CScraper::DownloadHomePage() > page (address=%s) downloaded\n", C_STR(sAddress));
      } else {      
        STATIC_uUnreachableHomePages = STATIC_uUnreachableHomePages + 1;
//        printf("WSR::CScraper::DownloadHomePage() > ERROR: unable to connect (address=%s, error=%d)\n", C_STR(sAddress), tCURLResult); 
      }
      STATIC_tLock.Broadcast();
    }
  }

  return (tResult);
} // DownloadHomePage

  
/////////////////////////////////////////////////////////////////////////////
void CScraper::RecognizeEmails(::DATASTRUCTURE::CArray<T_BYTE> * pPage) {
  regex_t tEmail;
  
  // compile regular expression
  if (regcomp(&tEmail, "[a-z][a-z|0-9|_|-|\\.]+@[a-z][a-z|0-9|_|-]+(\\.[a-z][a-z|0-9|_|-]+)+", REG_ICASE | REG_EXTENDED) == 0) { 
    regmatch_t tMatch;
    
    T_BOOLEAN bMatch = true;
    T_ULONG uPosition = 0;
    T_CHAR pBuffer[1024];
    T_BOOLEAN bExists = false;
    
    while (bMatch == true) {
      if (regexec(&tEmail, (T_CHAR *)pPage->GetElements() + uPosition, 1, &tMatch, 0) == 0) {
        T_ULONG uLength = tMatch.rm_eo - tMatch.rm_so + 1;
        if (uLength > sizeof(pBuffer) - 1) {
          uLength = sizeof(pBuffer) - 1;
        }
        snprintf(pBuffer, uLength, "%s", pPage->GetElements() + uPosition + tMatch.rm_so);
        pBuffer[uLength] = '\0';
        {
          GUARD __tGuard(STATIC_tBufferedEmails);

          bExists = STATIC_tBufferedEmails.Exists(pBuffer);
          if (bExists == false) {
            STATIC_tBufferedEmails.Insert(pBuffer, pBuffer);
            if (STATIC_tBufferedEmails.GetSize() > 50000) {
              STATIC_tBufferedEmails.Clear();
            }
            StoreEmail(pBuffer);           
          }
        }
        
        {
          GUARD __tGuard(STATIC_tLock);
          if (bExists == false) {
            STATIC_uRecognizedEmails = STATIC_uRecognizedEmails + 1;
          }
        }

        uPosition = uPosition + tMatch.rm_eo;
      } else {
        bMatch = false;
      }
    }
  
    // free regular expression
    regfree(&tEmail);
  } 
} // RecognizeEmails


/////////////////////////////////////////////////////////////////////////////

#define __CScraper__RecognizeDomains__REGEXP \
  "[http[s]*://]{0,1}" \
  "[a-z][a-z|0-9|_|-]*(\\.[a-z][a-z|0-9|_|-]*)*" \
  "(\\:[0-9]+){0,1}" \
  "(/[a-z|0-9|_|-|\\.]*)*" \
  "(\\?[a-z|0-9|_|-|=|&]*){0,1}"

REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > CScraper::RecognizeDomains(::DATASTRUCTURE::CArray<T_BYTE> * pPage, const T_STRING & sDomain, T_ULONG uDepth) {
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tResult;
  DATASTRUCTURE::CMap<T_STRING, REFERENCE<CTask> > tDomains;

  // create a new queue
  tResult.Create(new ::DATASTRUCTURE::CQueue<REFERENCE<CTask> >());
  
  regex_t tDomain;
  
  if (regcomp(&tDomain, __CScraper__RecognizeDomains__REGEXP, REG_ICASE | REG_EXTENDED) == 0) {    
    regmatch_t tMatch;
    
    T_BOOLEAN bMatch = true;
    T_ULONG uPosition = 0;
    T_CHAR pBuffer[4096];
    
    while (bMatch == true) {
      if (regexec(&tDomain, (T_CHAR *)pPage->GetElements() + uPosition, 1, &tMatch, 0) == 0) {
        T_ULONG uLength = tMatch.rm_eo - tMatch.rm_so + 1;
        if (uLength > sizeof(pBuffer) - 1) {
          uLength = sizeof(pBuffer) - 1;
        }
        snprintf(pBuffer, uLength, "%s", pPage->GetElements() + uPosition + tMatch.rm_so);
        pBuffer[uLength] = '\0';

        T_STRING sAddress = pBuffer;
  
        if (sAddress.SubString(0, 4) != "http") {
          if (sAddress.SubString(0, 2) != "//") {
            if (sAddress.SubString(0, 1) != "/") {
              sAddress = sDomain + "/" + sAddress;
            } else {
              sAddress = sDomain + sAddress;
            }
          } else {
            sAddress = sAddress.SubString(2);
          }
        }
  
        if (tDomains.Exists(sAddress) == false) {
          tDomains.Insert(sAddress, REFERENCE<CTask>().Create(new CTask(sAddress, sAddress, uDepth)));
//          printf("WSR::CScraper::RecognizeDomains() > domain (address=%s) recognized\n", C_STR(sAddress));
        }
        
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
void CScraper::Process(CTask * pDomain) {
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > tPage;
  
  // download home page
  tPage = DownloadHomePage(pDomain->GetAddress());

  // recognize emails
  if (tPage->IsValid() == true) {
    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_uRecognizingEmailScrapers = STATIC_uRecognizingEmailScrapers + 1;
    }
    RecognizeEmails(tPage);
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
      tDomains = RecognizeDomains(tPage, pDomain->GetAddress(), pDomain->GetDepth() - 1);  
      {
        GUARD __tGuard(STATIC_tLock);
        STATIC_uRecognizingDomainScrapers = STATIC_uRecognizingDomainScrapers - 1;
      }

      // process domains
      while (tDomains->IsEmpty() == false) {
        Process(tDomains->Pop());
      }
    }
  }
} // Process
  
  
/////////////////////////////////////////////////////////////////////////////
void CScraper::Loop() {
  REFERENCE<CTask> tDomain;
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tEmails;
  
  {
    GUARD __tGuard(STATIC_tLock);
    STATIC_uWaitingScrapers = STATIC_uWaitingScrapers + 1;
    STATIC_tLock.Broadcast();
  }

  __WSR__DEQUEUE_DOMAINS(tDomain);

  {
    GUARD __tGuard(STATIC_tLock);
    STATIC_uWaitingScrapers = STATIC_uWaitingScrapers - 1;
    STATIC_uWorkingScrapers = STATIC_uWorkingScrapers + 1;
    STATIC_tLock.Broadcast();
  } 

  if (tDomain->GetAddress() != "QUIT") {
//    printf("WSR::CScraper::Loop() > processing domain (address=%s) ..\n", C_STR(tDomain->GetAddress())); 

    {
      GUARD __tGuard(STATIC_tLock);
      STATIC_uEnqueuedDomains = STATIC_uEnqueuedDomains + 1;
      STATIC_tLock.Broadcast();
    } 

    // process domain
    IGNORE_EXCEPTION(Process(tDomain));
  } else {
    m_Shutdown = true;
  }

  {
    GUARD __tGuard(STATIC_tLock);
    STATIC_uWorkingScrapers = STATIC_uWorkingScrapers - 1;
    if (m_Shutdown == true) {
      STATIC_uRunningScrapers = STATIC_uRunningScrapers - 1;
    }
    STATIC_tLock.Broadcast();
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CScraper::CScraper(const T_STRING & sOutput, ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pDomains, ::BASE::IObject * DomainsProducers, ::BASE::IObject * DomainsConsumers) :
  ::BASE::CLoopThread(::BASE::IObject::NON_BLOCKED),
  m_Output(sOutput, ::std::ios::app),
  m_Domains(pDomains),
  m_DomainsProducers(DomainsProducers),
  m_DomainsConsumers(DomainsConsumers) {
  if (m_Output.fail() == true) {
    EXCEPTION(WSR, ::WSR::CScraper, CScraper,
    MESSAGE("unable to open output"));
    THROW(EScraper, UNABLE_TO_OPEN_OUTPUT);
  }
} // CScraper


/////////////////////////////////////////////////////////////////////////////
CScraper::~CScraper() {
} // ~CScraper

} // namespace WSR
