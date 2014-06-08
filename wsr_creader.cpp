
/////////////////////////////////////////////////////////////////////////////
//
// wsr_creader.cpp - ::WSR::CReader class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_creader.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
void CReader::Execute() {
  while ((GetShutdown() == false) && (m_Input.eof() == false) && (m_Input.fail() == false)) {
    T_ULONG uNumber = 0;

    GUARD __tGuard(m_Domains);

    // waiting for queue
    while ((GetShutdown() == false) && (m_Domains->GetSize() == m_Scrapers)) {
      m_Domains->Wait();
    }

    // enqueue domains
    while ((m_Domains->GetSize() < m_Scrapers) && (m_Input.eof() == false) && (m_Input.fail() == false)) {
      ::std::string sDomain;
      
      // read domain from input
      ::std::getline(m_Input, sDomain);
      if (m_Input.fail() == false) {
        // enqueue domain
        m_Domains->Push(REFERENCE< ::WSR::CTask>().Create(new ::WSR::CTask(sDomain.c_str(), m_Depth)));
        uNumber = uNumber + 1;        

        // send wakeup signal to a consumers
        m_Domains->Signal();    
      }
    }

    if (uNumber > 0) {
      printf("::WSR::CReader::Loop() > %d domain(s) are waiting in the queue for processing ..\n", m_Domains->GetSize());
    }
  }
  
  printf("::WSR::CReader::Loop() > all input domains processed\n");
} // Execute


/////////////////////////////////////////////////////////////////////////////
CReader::CReader(T_ULONG uScrapers, const T_STRING & sInput, T_ULONG uDepth, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pDomains) :
  m_Scrapers(uScrapers),
  m_Input(sInput, ::std::ios::in),
  m_Depth(uDepth),
  m_Domains(pDomains) {
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
