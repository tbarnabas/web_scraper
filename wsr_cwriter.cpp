
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cwriter.cpp - ::WSR::CWriter class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_cwriter.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
void CWriter::Loop() {
  REFERENCE<CTask> tTask;
  
  m_EmailsConsumers->Acquire();
  {
    GUARD __tGuard(m_Emails);
    tTask = m_Emails->Pop();
  }
  m_EmailsProducers->Release();

  if (tTask->GetAddress() != "QUIT") {
    m_Output << tTask->GetAddress() << ::std::endl;
    m_Output.flush();
  
    printf("WSR::CWriter::Loop() > email (address=%s) stored\n", C_STR(tTask->GetAddress()));
  } else {
    m_Shutdown = true;
    printf("WSR::CWriter::Loop() > shutting down\n");    
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CWriter::CWriter(const T_STRING & sOutput, ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pEmails, ::BASE::IObject * EmailsProducers, ::BASE::IObject * EmailsConsumers) :
  ::BASE::CLoopThread(::BASE::IObject::NON_BLOCKED),
  m_Output(sOutput, ::std::ios::app),
  m_Emails(pEmails),
  m_EmailsProducers(EmailsProducers),
  m_EmailsConsumers(EmailsConsumers) {  
  if (m_Output.fail() == true) {
    EXCEPTION(WSR, ::WSR::CWriter, CWriter,
    MESSAGE("unable to open"));
    THROW(EWriter, UNABLE_TO_OPEN);
  }
} // CWriter


/////////////////////////////////////////////////////////////////////////////
CWriter::~CWriter() {
} // ~CWriter

} // namespace WSR
