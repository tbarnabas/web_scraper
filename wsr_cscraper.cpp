
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cscraper.cpp - ::WSR::CScraper class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_cscraper.h"

namespace WSR {

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
  
  if (sDomain.Find("http") == false) {
    sDomain = T_STRING("http://") + sDomain;
  }
  
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

    curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, ReceiveData);    
    curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, (void *)tResult.__ptr());
    
    // perform CURL operation
    tCURLResult = curl_easy_perform(pCURL);
    
    // clean up CURL session
    curl_easy_cleanup(pCURL);
    
    if (tCURLResult != CURLE_OK) {
      EXCEPTION(WSR, ::WSR::CScraper, DownloadHomePage,
      MESSAGE("unable to download home page"));
      THROW(EScraper, UNABLE_TO_DOWNLOAD_HOME_PAGE);
    }
  }
  
  return (tResult);
} // DownloadHomePage

  
/////////////////////////////////////////////////////////////////////////////
REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > CScraper::RecognizeEmails(::DATASTRUCTURE::CArray<T_BYTE> * pPage) {
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tResult;

  // create a new queue
  tResult.Create(new ::DATASTRUCTURE::CQueue<REFERENCE<CTask> >());
  
  regex_t tEmail;
  
  // compile regular expression
  if (regcomp(&tEmail, "[a-z][a-z0-9_]*@[a-z][a-z0-9_]*[.[a-z][a-z0-9_]*]*", REG_ICASE | REG_EXTENDED) == 0) { 
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
        tResult->Push(REFERENCE<CTask>().Create(new CTask(pBuffer, 0)));
        uPosition = uPosition + tMatch.rm_eo;
      } else {
        bMatch = false;
      }
    }
  
    // free regular expression
    regfree(&tEmail);
  }
  
  return (tResult);
} // RecognizeEmails


/////////////////////////////////////////////////////////////////////////////
REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > CScraper::RecognizeDomains(::DATASTRUCTURE::CArray<T_BYTE> * pPage) {
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tResult;
  
  // create a new queue
  tResult.Create(new ::DATASTRUCTURE::CQueue<REFERENCE<CTask> >());
  
  
  return (tResult);
} // RecognizeDomains


/////////////////////////////////////////////////////////////////////////////
void CScraper::Loop() {
  REFERENCE<CTask> tDomain;
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > tPage;
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tEmails;
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tDomains;
  
  // dequeue domain
  m_DomainsConsumers->Acquire();
  {
    GUARD __tGuard(m_Domains);
    tDomain = m_Domains->Pop();
  }
  m_DomainsProducers->Release();
  
  // download home page
  tPage = DownloadHomePage(tDomain->GetAddress());

//  ::BASE::Dump(tPage);
  
  printf("CScraper::Loop() > page (address=%s) downloaded\n", C_STR(tDomain->GetAddress()));

  // recognize emails
  tEmails = RecognizeEmails(tPage);

  printf("CScraper::Loop() > %d email(s) recognized\n", tEmails->GetSize());

  // enqueue emails
  while (tEmails->IsEmpty() == false) {
    m_EmailsProducers->Acquire();
    {
      GUARD __tGuard(m_Emails);
      m_Emails->Push(tEmails->Pop());
    }
    m_EmailsConsumers->Release();
  }
  
  // recognize domains
  tDomains = RecognizeDomains(tPage);

  printf("CScraper::Loop() > %d domain(s) recognized\n", tDomains->GetSize());

  // enqueue emails
  while (tDomains->IsEmpty() == false) {
    m_DomainsProducers->Acquire();
    {
      GUARD __tGuard(m_Domains);
      m_Domains->Push(tDomains->Pop());
    }
    m_DomainsConsumers->Release();
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
