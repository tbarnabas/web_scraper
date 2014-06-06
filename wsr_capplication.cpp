
/////////////////////////////////////////////////////////////////////////////
//
// wsr_capplication.cpp - ::WSR::CApplication class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_capplication.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
CApplication::CApplication(T_ULONG uScraper, const T_STRING & sInput, const T_STRING & sOutput, T_ULONG uDepth) : 
  m_Scraper(uScraper),
  m_Input(sInput),
  m_Output(sOutput),
  m_Depth(uDepth) {
} // CApplication


/////////////////////////////////////////////////////////////////////////////
CApplication::~CApplication() {
} // ~CApplication


/////////////////////////////////////////////////////////////////////////////
void CApplication::Initialize() {
  // create a new reader
  m_Reader.Create(new ::WSR::CReader());
  m_ObjectManager.Insert(m_ObjectManager.GetSize(), m_Reader.__ptr());

  // initialize application
  ::RESOURCE::CApplication::Initialize();
} // Initialize


/////////////////////////////////////////////////////////////////////////////
void CApplication::Run() {
  printf("HELLO\n");
  sleep(3000);
} // Run

} // namespace WSR
