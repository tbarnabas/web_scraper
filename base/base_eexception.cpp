
/////////////////////////////////////////////////////////////////////////////
//
// base_eexception.cpp - ::BASE::EException class source
//
/////////////////////////////////////////////////////////////////////////////

#include "base_eexception.h"

namespace BASE {

/////////////////////////////////////////////////////////////////////////////
EException::EException(const T_TIME & tTime, const T_STRING & sModule, const T_STRING & sClassPath, const T_STRING & sMethod, T_ULONG uLevel, T_ULONG uCode, const T_STRING & sMessage) :
  CMessage(tTime, sModule, sClassPath, sMethod, uLevel, uCode, sMessage) {
} // EException


/////////////////////////////////////////////////////////////////////////////
EException::EException(const CMessage & tMessage) :
  CMessage(tMessage) {
} // EException


/////////////////////////////////////////////////////////////////////////////
EException::~EException() {
} // ~EException


/////////////////////////////////////////////////////////////////////////////
EException::EException(const EException & tException) :
  CMessage(tException) {
} // EException


/////////////////////////////////////////////////////////////////////////////
EException & EException::operator=(const EException & tException) {
  CMessage::operator=(tException);
  return (* this);
} // operator=

} // namespace BASE
