
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cwriter.cpp - ::WSR::CWriter class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_cwriter.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
void CWriter::FlushBufferedEmails() {
  T_ULONG uSize = m_BufferedEmails.GetSize();
  
  while (m_BufferedEmails.IsEmpty() == false) {
    m_Output << m_BufferedEmails.RemoveElement(0)->GetAddress() << ::std::endl;
    m_Output.flush();
  }

  printf("WSR::CWriter::FlushBufferedEmails() > %d email(s) stored\n", uSize);
} // FlushBufferedEmails


/////////////////////////////////////////////////////////////////////////////
void CWriter::Loop() {
  REFERENCE<CTask> tTask;

  __DEQUEUE_EMAILS(tTask);

  if (tTask->GetAddress() != "QUIT") {
    m_BufferedEmails.Insert(tTask->GetAddress(), tTask);
    if (m_BufferedEmails.GetSize() > 1000) {
      FlushBufferedEmails();
    }
  } else {
    m_Shutdown = true;
    FlushBufferedEmails();
    printf("WSR::CWriter::Loop() > shutdown\n");
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
