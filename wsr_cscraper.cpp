
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
    THREADGUARD __tGuard(m_Domains);
    
    // waiting for task
    while ((m_Domains->IsEmpty() == true) && (::THREAD::CObject::GetInterrupted() == false)) {
      m_Domains->Wait();
    }
    
    // pop task from queue
    tTask = m_Domains->Pop();

    // send wakeup signal to producer
    m_Reader->Signal();
  }
  
  if (tTask.IsValid() == true) {
    printf("%d processing ...\n", ::THREAD::CThread::STATIC_GetCurrentThreadId());
  }

  {
    THREADGUARD __tGuard(m_Emails);
    
    // push task into queue
    m_Emails->Push(tTask);
    
    // send wakeup signal to consumer
    m_Writer->Signal();
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CScraper::CScraper(::THREAD::CObject * pReader, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pDomains, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pEmails, ::THREAD::CObject * pWriter) :
  ::THREAD::CLoopThread(::BASE::CObject::NON_BLOCKED),
  m_Reader(pReader),
  m_Domains(pDomains),
  m_Emails(pEmails),
  m_Writer(pWriter) {
} // CScraper


/////////////////////////////////////////////////////////////////////////////
CScraper::~CScraper() {
} // ~CScraper

} // namespace WSR
