
/////////////////////////////////////////////////////////////////////////////
//
// wsr_ctask.cpp - ::WSR::CTask class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_ctask.h"

namespace WSR {

/////////////////////////////////////////////////////////////////////////////
CTask::CTask(const T_STRING & sDomain, const T_STRING & sAddress, T_ULONG uDepth) :
  m_Domain(sDomain),
  m_Address(sAddress),
  m_Depth(uDepth) {
} // CTask


/////////////////////////////////////////////////////////////////////////////
CTask::~CTask() {
} // ~CTask


/////////////////////////////////////////////////////////////////////////////
CTask::CTask(const CTask & tTask) :
  ::BASE::CObject(tTask),
  m_Domain(tTask.m_Domain),
  m_Address(tTask.m_Address),
  m_Depth(tTask.m_Depth) {
} // CTask


/////////////////////////////////////////////////////////////////////////////
CTask & CTask::operator=(const CTask & tTask) {
  ::BASE::CObject::operator=(tTask);
  m_Domain = tTask.m_Domain;
  m_Address = tTask.m_Address;
  m_Depth = tTask.m_Depth;
  return (* this);
} // operator=

} // namespace WSR
