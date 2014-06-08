
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cscraper.cpp - ::WSR::CScraper class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_cscraper.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
void CScraper::Loop() {
  REFERENCE< ::WSR::CTask> tTask;
  
  m_DomainsConsumers->Acquire();

  // dequeue domain
  {
    GUARD __tGuard(m_Domains);
    tTask = m_Domains->Pop();
  }

  m_DomainsProducers->Release();
  
  printf("%d processing ...\n", GetThreadId());

  m_EmailsProducers->Acquire();

  // dequeue email
  {
    GUARD __tGuard(m_Emails);
    m_Emails->Push(tTask);
  }

  m_EmailsConsumers->Release();
} // Loop


/////////////////////////////////////////////////////////////////////////////
CScraper::CScraper(::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pDomains, ::BASE::IObject * DomainsProducers, ::BASE::IObject * DomainsConsumers, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pEmails, ::BASE::IObject * EmailsProducers, ::BASE::IObject * EmailsConsumers) :
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
