
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
  
  {
    GUARD __tGuard(m_Domains);
    
    // waiting for task
    while ((m_Domains->IsEmpty() == true) && (GetShutdown() == false)) {
      m_Domains->Wait();
    }
    
    // pop task from queue
    tTask = m_Domains->Pop();

    // send wakeup signal to producer
    m_Reader->Signal();
  }
  
  if (tTask.IsValid() == true) {
    printf("%d processing ...\n", GetThreadId());
  }

  {
    GUARD __tGuard(m_Emails);
    
    // push task into queue
    m_Emails->Push(tTask);
    
    // send wakeup signal to consumer
    m_Writer->Signal();
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CScraper::CScraper(::BASE::IObject * pReader, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pDomains, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pEmails, ::BASE::IObject * pWriter) :
  ::BASE::CLoopThread(::BASE::IObject::NON_BLOCKED),
  m_Reader(pReader),
  m_Domains(pDomains),
  m_Emails(pEmails),
  m_Writer(pWriter) {
} // CScraper


/////////////////////////////////////////////////////////////////////////////
CScraper::~CScraper() {
} // ~CScraper

} // namespace WSR
