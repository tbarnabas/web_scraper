
/////////////////////////////////////////////////////////////////////////////
//
// wsr_creader.cpp - ::WSR::CReader class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_creader.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
void CReader::Loop() {
  
  printf("LOOP\n");
} // Loop


/////////////////////////////////////////////////////////////////////////////
CReader::CReader(const T_STRING & sInput) :
  ::THREAD::CLoopThread(::BASE::CObject::BLOCKED, T_TIME(1, 0)) {
  // create a new file
  m_Input.Create(new ::RESOURCE::CFile(sInput, ::RESOURCE::CFile::READ));
} // CReader


/////////////////////////////////////////////////////////////////////////////
CReader::~CReader() {
} // ~CReader


/////////////////////////////////////////////////////////////////////////////
CReader::CReader(const CReader & tReader) :
  ::THREAD::CLoopThread(tReader) {
} // CReader


/////////////////////////////////////////////////////////////////////////////
CReader & CReader::operator=(const CReader & tReader) {
  ::THREAD::CLoopThread::operator=(tReader);
  return (* this);
} // operator=

} // namespace WSR
