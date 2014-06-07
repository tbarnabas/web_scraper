
/////////////////////////////////////////////////////////////////////////////
//
// wsr_creader.cpp - ::WSR::CReader class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_creader.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
void CReader::Loop() {
  if (m_Input.eof() == false) {
    THREADGUARD __tGuard(m_Domains);
    
    T_ULONG uNumber = 0;
    
    // enqueue domains
    while ((m_Domains->GetSize() < m_Scrapers) && (m_Input.eof() == false) && (m_Input.fail() == false)) {
      ::std::string sDomain;
      ::std::getline(m_Input, sDomain);
      if (m_Input.fail() == false) {
        m_Domains->Push(REFERENCE< ::WSR::CTask>().Create(new ::WSR::CTask(sDomain.c_str(), m_Depth)));
        uNumber = uNumber + 1;        
      }
    }

    if (uNumber > 0) {
      printf("::WSR::CReader::Loop() > %d domain(s) enqueued\n", uNumber);
    }
    
    // broadcast wakeup signal to all consumers
    m_Domains->Broadcast();    
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CReader::CReader(T_ULONG uScrapers, const T_STRING & sInput, T_ULONG uDepth, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pDomains) :
  ::THREAD::CLoopThread(::BASE::CObject::BLOCKED),
  m_Scrapers(uScrapers),
  m_Input(sInput, ::std::ios::in),
  m_Depth(uDepth),
  m_Domains(pDomains) {
  if (m_Input.fail() == true) {
    EXCEPTION(WSR, ::WSR::CReader, CReader,
    MESSAGE("unable to open"));
    THROW(EReader, UNABLE_TO_OPEN);
  }
} // CReader


/////////////////////////////////////////////////////////////////////////////
CReader::~CReader() {
} // ~CReader

} // namespace WSR
