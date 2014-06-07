
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cwriter.cpp - ::WSR::CWriter class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_cwriter.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
void CWriter::Loop() {
  if (m_Output.fail() == false) {
    THREADGUARD __tGuard(m_Emails);
    
    T_ULONG uSize = m_Emails->GetSize();
    
    // dequeue emails
    if (uSize > 0) {
      while (m_Emails->IsEmpty() == false) {
        REFERENCE< ::WSR::CTask> tTask = m_Emails->Pop();
        m_Output << tTask->GetAddress() << ::std::endl;
      }

      // flush output stream
      m_Output.flush();
      
      // send broadcast signal to producers (optional)
      m_Emails->Broadcast();

      printf("::WSR::CWriter::Loop() > %d email(s) stored\n", uSize);
    }
  } else {
    printf("::WSR::CWriter::Loop() FATAL ERROR > output error\n");
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CWriter::CWriter(const T_STRING & sOutput, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pEmails) :
  ::THREAD::CLoopThread(::BASE::CObject::BLOCKED),
  m_Output(sOutput, ::std::ios::app),
  m_Emails(pEmails) {
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
