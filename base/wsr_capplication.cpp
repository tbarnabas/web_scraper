
/////////////////////////////////////////////////////////////////////////////
//
// wsr_capplication.cpp - ::WSR::CApplication class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_capplication.h"

#include "wsr_cscraper.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
CApplication::CApplication(T_ULONG uScrapers, const T_STRING & sInput, const T_STRING & sOutput, T_ULONG uDepth) : 
  ::RESOURCE::CApplication(::BASE::IObject::BLOCKED, T_TIME(1)) {
  REFERENCE< ::BASE::IObject> tDomainsProducers;
  REFERENCE< ::BASE::IObject> tDomainsConsumers;
  REFERENCE<CScraper> tScraper;
  REFERENCE< ::BASE::IObject> tEmailsProducers;
  REFERENCE< ::BASE::IObject> tEmailsConsumers;
  
  // create a new queue
  m_Domains.Create(new ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> >());
  m_Domains->SetSynch(REFERENCE< ::BASE::IObject>().Create(new ::BASE::CMutex()));
  tDomainsProducers.Create(new ::BASE::CSemaphore(uScrapers));
  tDomainsConsumers.Create(new ::BASE::CSemaphore(0));
  
  // create a new queue
  m_Emails.Create(new ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> >());
  m_Emails->SetSynch(REFERENCE< ::BASE::IObject>().Create(new ::BASE::CMutex()));
  tEmailsProducers.Create(new ::BASE::CSemaphore(uScrapers));
  tEmailsConsumers.Create(new ::BASE::CSemaphore(0));
  
  // create a new reader
  m_Reader.Create(new CReader(uScrapers, sInput, uDepth, m_Domains, tDomainsProducers, tDomainsConsumers));
  m_ObjectManager.Insert(m_ObjectManager.GetSize(), m_Reader.__ptr());

  // create a new writer
  m_Writer.Create(new CWriter(sOutput, m_Emails, tEmailsProducers, tEmailsConsumers));
  m_ObjectManager.Insert(m_ObjectManager.GetSize(), m_Writer.__ptr());

  // create new scrapers
  for (T_ULONG i = 0; i < uScrapers; i++) {
    tScraper.Create(new CScraper(m_Domains, tDomainsProducers, tDomainsConsumers, m_Emails, tEmailsProducers, tEmailsConsumers));
    m_ObjectManager.Insert(m_ObjectManager.GetSize(), tScraper.__ptr());
  }
} // CApplication


/////////////////////////////////////////////////////////////////////////////
CApplication::~CApplication() {
} // ~CApplication


/////////////////////////////////////////////////////////////////////////////
void CApplication::Run() {
  GUARD __tGuard(CScraper::STATIC_tLock);
  while ((CScraper::STATIC_uWorkingScrapers != 0) || (CScraper::STATIC_uTotalDownloadTry == 0)) {
    CScraper::STATIC_tLock.Wait();
  }

  printf("WSR::CApplication::Run() > shutting down ..\n"); 
} // Run

} // namespace WSR
