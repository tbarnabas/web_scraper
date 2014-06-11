
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cscraper.cpp - ::WSR::CScraper class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_cscraper.h"

namespace WSR {

::DATASTRUCTURE::CMap<T_STRING, T_STRING> CScraper::STATIC_tLockedDomains;
  
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
  
  CURL * pCURL= NULL;
  CURLcode tCURLResult;
  
  // create a new array
  tResult.Create(new ::DATASTRUCTURE::CArray<T_BYTE>());
  
  // initialize CURL session
  pCURL = curl_easy_init();
  if (pCURL != NULL) {
    // set CURL options
    curl_easy_setopt(pCURL, CURLOPT_URL, C_STR(sDomain));
    curl_easy_setopt(pCURL, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(pCURL, CURLOPT_USERAGENT, "wsr-agent/1.0");
    curl_easy_setopt(pCURL, CURLOPT_CONNECTTIMEOUT, 5);

    curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, ReceiveData);    
    curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, (void *)tResult.__ptr());
 
    printf("CScraper::DownloadHomePage() > connecting to server (address=%s)\n", C_STR(sAddress)); 
    
    // perform CURL operation
    tCURLResult = curl_easy_perform(pCURL);
    
    // clean up CURL session
    curl_easy_cleanup(pCURL);
    
    if (tCURLResult != CURLE_OK) {
      printf("CScraper::DownloadHomePage() > ERROR: unable to connect (address=%s)\n", C_STR(sAddress)); 
      tResult = NULL;
    } else {
      printf("CScraper::DownloadHomePage() > page (address=%s) downloaded\n", C_STR(sAddress));
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
  if (tPage.IsValid() == true) {
    RecognizeEmails(tPage, tResult);

    // recognize domains
    if (pDomain->GetDepth() > 0) {
      REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tDomains;
      
      tDomains = RecognizeDomains(tPage, pDomain->GetDepth() - 1);  

      if (tDomains->GetSize() != 0) {
        printf("CScraper::Loop() > %d domain(s) recognized\n", tDomains->GetSize());
      }

      // process domains
      while (tDomains->IsEmpty() == false) {
        IGNORE_EXCEPTION(tResult = Process(tDomains->Pop(), tResult));
      }
    }
  }

  return (tResult);
} // Process
  
  
/////////////////////////////////////////////////////////////////////////////
void CScraper::Loop() {
  REFERENCE<CTask> tDomain;
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tEmails;
  
  // dequeue domain
  m_DomainsConsumers->Acquire();
  {
    GUARD __tGuard(m_Domains);
    tDomain = m_Domains->Pop();
  }
  m_DomainsProducers->Release();

  // process domain
  tEmails = Process(tDomain);
  
  if (tEmails.IsValid() == true) {
    if (tEmails->GetSize() != 0) {
      printf("CScraper::Loop() > %d email(s) recognized\n", tEmails->GetSize());
    }

    // enqueue emails
    while (tEmails->IsEmpty() == false) {
      m_EmailsProducers->Acquire();
      {
        GUARD __tGuard(m_Emails);
        m_Emails->Push(tEmails->Pop());
      }
      m_EmailsConsumers->Release();
    }
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
