
/////////////////////////////////////////////////////////////////////////////
//
// wsr_ctask.cpp - ::WSR::CTask class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_ctask.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
CTask::CTask(const T_STRING & sAddress, T_ULONG uDepth, T_ULONG uDownload) :
  m_Address(sAddress),
  m_Depth(uDepth),
  m_Download(uDownload) {
} // CTask


/////////////////////////////////////////////////////////////////////////////
CTask::~CTask() {
} // ~CTask


/////////////////////////////////////////////////////////////////////////////
CTask::CTask(const CTask & tTask) :
  ::BASE::CObject(tTask),
  m_Address(tTask.m_Address),
  m_Depth(tTask.m_Depth),
  m_Download(tTask.m_Download) {
} // CTask


/////////////////////////////////////////////////////////////////////////////
CTask & CTask::operator=(const CTask & tTask) {
  ::BASE::CObject::operator=(tTask);
  m_Address = tTask.m_Address;
  m_Depth = tTask.m_Depth;
  m_Download = tTask.m_Download;
  return (* this);
} // operator=

} // namespace WSR
