
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
    T_ULONG uSize = 0;
  
    // read domain from input
    ::std::string sDomain;
    ::std::getline(m_Input, sDomain);
  
    if (m_Input.fail() == false) {
      m_DomainsProducers->Acquire();
    
      // enqueue domain
      {
        GUARD __tGuard(m_Domains); 
        m_Domains->Push(REFERENCE< ::WSR::CTask>().Create(new ::WSR::CTask(sDomain.c_str(), m_Depth)));
        uSize = m_Domains->GetSize();
      }

      m_DomainsConsumers->Release();

      printf("::WSR::CReader::Loop() > %d domain(s) are waiting in the queue for processing ..\n", uSize);
    }
  } else {
    m_Shutdown = true;
    printf("::WSR::CReader::Loop() > all input domains processed\n");
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CReader::CReader(const T_STRING & sInput, T_ULONG uDepth, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pDomains, ::BASE::IObject * DomainsProducers, ::BASE::IObject * DomainsConsumers) :
  ::BASE::CLoopThread(::BASE::IObject::NON_BLOCKED),
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
