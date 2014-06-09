
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
  
  printf("CScraper::ReceiveData() > extent %d -> %d\n", uOriginalSize, tArray->GetSize());  
/*
printf("--4--\n");  
  (* tArray)[tArray->GetSize() - 1] = 0;
  
  ::BASE::Dump(tArray);
  */
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
  
  tResult.Create(new ::DATASTRUCTURE::CArray<T_BYTE>());
  
  pCURL = curl_easy_init();
  if (pCURL != NULL) {
    curl_easy_setopt(pCURL, CURLOPT_URL, C_STR(sDomain));
    curl_easy_setopt(pCURL, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(pCURL, CURLOPT_USERAGENT, "wsr-agent/1.0");

    curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, ReceiveData);    
    curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, (void *)tResult.__ptr());
    
    tCURLResult = curl_easy_perform(pCURL);
    
    curl_easy_cleanup(pCURL);
    
    if (tCURLResult != CURLE_OK) {
      EXCEPTION(WSR, ::WSR::CScraper, DownloadHomePage,
      MESSAGE("unable to download home page"));
      THROW(EScraper, UNABLE_TO_DOWNLOAD_HOME_PAGE);
    }
  }
  
  ::BASE::Dump(tResult);
  
  return (tResult);
} // DownloadHomePage

  
/////////////////////////////////////////////////////////////////////////////
void CScraper::Process(CTask * pTask) {
  // download home page
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > tPage = DownloadHomePage(pTask->GetAddress());
} // Process


/////////////////////////////////////////////////////////////////////////////
void CScraper::Loop() {
  REFERENCE<CTask> tTask;
  
  m_DomainsConsumers->Acquire();

  // dequeue domain
  {
    GUARD __tGuard(m_Domains);
    tTask = m_Domains->Pop();
  }

  m_DomainsProducers->Release();
  
  IGNORE_EXCEPTION(Process(tTask));

  m_EmailsProducers->Acquire();

  // dequeue email
  {
    GUARD __tGuard(m_Emails);
    m_Emails->Push(tTask);
  }

  m_EmailsConsumers->Release();
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
