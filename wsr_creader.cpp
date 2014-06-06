
/////////////////////////////////////////////////////////////////////////////
//
// wsr_creader.cpp - ::WSR::CReader class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_creader.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
void CReader::Loop() {
  if ((m_Input.eof() == false) && (m_Input.fail() == false)) {
    THREADGUARD __tGuard(m_Domains);
  
    while ((m_Domains->GetSize() < m_Scrapers) && (m_Input.eof() == false) && (m_Input.fail() == false)) {
      ::std::string sDomain;
      ::std::getline(m_Input, sDomain);
      if (m_Input.fail() == false) {
        REFERENCE< ::WSR::CTask> tTask;
        m_Domains->Push(REFERENCE< ::WSR::CTask>().Create(new ::WSR::CTask(sDomain.c_str(), m_Depth)));
      }
    }
    
    m_Domains->Broadcast();
    
    printf("::WSR::CReader::Loop() > %d domain(s) are waiting in the queue for processing ..\n", m_Domains->GetSize());
  } else {
    ::THREAD::CLoopThread::m_TimeOut = T_TIME(5, 0);
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CReader::CReader(T_ULONG uScrapers, const T_STRING & sInput, T_ULONG uDepth, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pDomains) :
  ::THREAD::CLoopThread(::BASE::CObject::BLOCKED),
  m_Scrapers(uScrapers),
  m_Input(sInput, ::std::ios::in),
  m_Depth(uDepth),
  m_Domains(pDomains) {
} // CReader


/////////////////////////////////////////////////////////////////////////////
CReader::~CReader() {
} // ~CReader

} // namespace WSR
