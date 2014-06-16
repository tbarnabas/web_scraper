
/////////////////////////////////////////////////////////////////////////////
//
// wsr_creader.cpp - ::WSR::CReader class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_creader.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
void CReader::Loop() {
  REFERENCE<CTask> tTask;
  
  if (m_Input.eof() == false) {  
    ::std::string sDomain;
    ::std::getline(m_Input, sDomain);
  
    if (m_Input.fail() == false) {
      tTask.Create(new ::WSR::CTask(sDomain.c_str(), sDomain.c_str(), m_Depth));
    }
  } else {
    if (m_Scrapers > 0) {
      tTask.Create(new ::WSR::CTask("QUIT", "QUIT", 0));
      m_Scrapers = m_Scrapers - 1;
    } else {
      m_Shutdown = true;
    }
  }

  if (tTask.IsValid() == true) {
    __WSR__ENQUEUE_DOMAINS(tTask);
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CReader::CReader(T_ULONG uScrapers, const T_STRING & sInput, T_ULONG uDepth, ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pDomains, ::BASE::IObject * DomainsProducers, ::BASE::IObject * DomainsConsumers) :
  ::BASE::CLoopThread(::BASE::IObject::NON_BLOCKED),
  m_Scrapers(uScrapers),
  m_Input(sInput, ::std::ios::in),
  m_Depth(uDepth),
  m_Domains(pDomains),
  m_DomainsProducers(DomainsProducers),
  m_DomainsConsumers(DomainsConsumers) {
  // check existence of input
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
