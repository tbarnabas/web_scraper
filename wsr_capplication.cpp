
/////////////////////////////////////////////////////////////////////////////
//
// wsr_capplication.cpp - ::WSR::CApplication class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_capplication.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
CApplication::CApplication(T_ULONG uScraper, const T_STRING & sInput, const T_STRING & sOutput, T_ULONG uDepth) : 
  ::RESOURCE::CApplication(::BASE::CObject::BLOCKED, T_TIME(1)),
  m_Scraper(uScraper),
  m_Input(sInput),
  m_Output(sOutput),
  m_Depth(uDepth) {

  m_Reader.Create(new ::WSR::CReader());
  m_ObjectManager.Insert(m_ObjectManager.GetSize(), m_Reader.__ptr());
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
