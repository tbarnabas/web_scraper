
/////////////////////////////////////////////////////////////////////////////
//
// wsr_capplication.cpp - ::WSR::CApplication class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_capplication.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
CApplication::CApplication(T_ULONG uScrapers, const T_STRING & sInput, const T_STRING & sOutput, T_ULONG uDepth) : 
  ::RESOURCE::CApplication(::BASE::CObject::BLOCKED) {
  // create a new queue
  m_Domains.Create(new ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> >());
  m_Domains->SetSynchronizator(REFERENCE< ::THREAD::CObject>().Create(new ::THREAD::CCondition()));

  // create a new queue
  m_Emails.Create(new ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> >());
  m_Emails->SetSynchronizator(REFERENCE< ::THREAD::CObject>().Create(new ::THREAD::CCondition()));
  
  // create a new reader
  m_Reader.Create(new ::WSR::CReader(uScrapers, sInput, uDepth, m_Domains));
  m_Reader->SetSynchronizator(m_Domains->GetSynchronizator());
  m_ObjectManager.Insert(m_ObjectManager.GetSize(), m_Reader.__ptr());

  // create a new writer
  m_Writer.Create(new ::WSR::CWriter(sOutput, m_Domains));
  m_Writer->SetSynchronizator(m_Domains->GetSynchronizator());
  m_ObjectManager.Insert(m_ObjectManager.GetSize(), m_Writer.__ptr());
} // CApplication


/////////////////////////////////////////////////////////////////////////////
CApplication::~CApplication() {
} // ~CApplication


/////////////////////////////////////////////////////////////////////////////
void CApplication::Run() {
  printf("HELLO\n");
  sleep(5);
} // Run

} // namespace WSR
