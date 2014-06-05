
/////////////////////////////////////////////////////////////////////////////
//
// base_cipaddress.cpp - ::BASE::CIPAddress class source
//
/////////////////////////////////////////////////////////////////////////////

#include "base_cipaddress.h"

namespace BASE {

#define __BASE__CIPAddress__BUFFER_SIZE 32
#define __BASE__CIPAddress__FORMAT "%d.%d.%d.%d"

/////////////////////////////////////////////////////////////////////////////
void CIPAddress::__construct(const T_STRING & sIPAddress) {
  T_INT a = 0;
  T_INT b = 0;
  T_INT c = 0;
  T_INT d = 0;

#if (OS_FAMILY == OSF_WINDOWS)

  if (sscanf_s(C_STR(sIPAddress), __BASE__CIPAddress__FORMAT, &a, &b, &c, &d) == 0) {

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX) | (PA_FAMILY == PAF_AVR))

  if (sscanf(C_STR(sIPAddress), __BASE__CIPAddress__FORMAT, &a, &b, &c, &d) == 0) {

#else
#error unsupported platform
#endif

    m_uIPAddress = 0;
  } else {
    m_uIPAddress = ((a << 24) + (b << 16) + (c << 8) + d) & 0x00000000ffffffff;
  }
} // __construct


/////////////////////////////////////////////////////////////////////////////
CIPAddress::CIPAddress(T_UINT32 uIPAddress) :
  m_uIPAddress(uIPAddress) {
} // CIPAddress


/////////////////////////////////////////////////////////////////////////////
CIPAddress::CIPAddress(const T_STRING & sIPAddress) {
  __construct(sIPAddress);
} // CIPAddress


/////////////////////////////////////////////////////////////////////////////
CIPAddress::~CIPAddress() {
} // ~CIPAddress


/////////////////////////////////////////////////////////////////////////////
CIPAddress::CIPAddress(const CIPAddress & tIPAddress) : 
  m_uIPAddress(tIPAddress.m_uIPAddress) {
} // CIPAddress


/////////////////////////////////////////////////////////////////////////////
CIPAddress & CIPAddress::operator=(const CIPAddress & tIPAddress) {
  m_uIPAddress = tIPAddress.m_uIPAddress;
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
CIPAddress & CIPAddress::operator=(T_UINT32 uIPAddress) {
  m_uIPAddress = uIPAddress;
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
CIPAddress & CIPAddress::operator=(const T_STRING & sIPAddress) {
  __construct(sIPAddress);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
CIPAddress::operator T_UINT32() const {
  return (m_uIPAddress);
} // operator T_UINT32


/////////////////////////////////////////////////////////////////////////////
CIPAddress::operator T_STRING() const {
  T_CHAR pBuffer[__BASE__CIPAddress__BUFFER_SIZE];

#if (OS_FAMILY == OSF_WINDOWS)

  sprintf_s(pBuffer, __BASE__CIPAddress__BUFFER_SIZE, __BASE__CIPAddress__FORMAT, (T_INT)((m_uIPAddress & 0xff000000) >> 24), (T_INT)((m_uIPAddress & 0xff0000) >> 16), (T_INT)((m_uIPAddress & 0xff00) >> 8), (T_INT)(m_uIPAddress & 0xff));

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX) | (PA_FAMILY == PAF_AVR))

  snprintf(pBuffer, __BASE__CIPAddress__BUFFER_SIZE, __BASE__CIPAddress__FORMAT, (T_INT)((m_uIPAddress & 0xff000000) >> 24), (T_INT)((m_uIPAddress & 0xff0000) >> 16), (T_INT)((m_uIPAddress & 0xff00) >> 8), (T_INT)(m_uIPAddress & 0xff));

#else
#error unsupported platform
#endif

  return (pBuffer);
} // operator T_STRING


/////////////////////////////////////////////////////////////////////////////
T_BOOLEAN CIPAddress::IsValid() const {
  return (m_uIPAddress != 0);
} // IsValid

} // namespace BASE
