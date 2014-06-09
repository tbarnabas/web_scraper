
/////////////////////////////////////////////////////////////////////////////
//
// base_cstring.cpp - ::BASE::CString class source
//
/////////////////////////////////////////////////////////////////////////////

#include "base_cstring.h"

namespace BASE {

const T_ULONG CString::npos = (T_ULONG)-1;

#define __BASE__CString__BUFFER_SIZE 128

/////////////////////////////////////////////////////////////////////////////
T_INT CString::__strcmp(const T_CHAR * pLeft, const T_CHAR * pRight) {
  T_INT iResult = 0;

  if ((pLeft != NULL) && (pRight != NULL)) {
    T_ULONG i = 0;

    while (((pLeft[i] != 0) && (pRight[i] != 0)) && (iResult == 0)) {
      if (pLeft[i] > pRight[i]) {
        iResult = 1;
      } else {
        if (pLeft[i] < pRight[i]) {
          iResult = -1;
        } else {
          i = i + 1;
        }
      }
    }

    if ((pLeft[i] == 0) && (pRight[i] != 0)) {
      iResult = -1;
    }
    if ((pLeft[i] != 0) && (pRight[i] == 0)) {
      iResult = 1;
    }
  } else {
    if (pLeft == NULL) {
      iResult = iResult - 1;
    }
    if (pRight == NULL) {
      iResult = iResult + 1;
    }
  }

  return (iResult);
} // __strcmp


/////////////////////////////////////////////////////////////////////////////
void CString::__strcpy(T_CHAR * pDestination, const T_CHAR * pSource, T_ULONG uLength) {
  for (T_ULONG i = 0; i < uLength; i++) {
    pDestination[i] = pSource[i];
  }
} // __strcpy


/////////////////////////////////////////////////////////////////////////////
T_ULONG CString::__strlen(const T_CHAR * pString) {
  T_ULONG uResult = 0;

  if (pString != NULL) {
    while (pString[uResult] != 0) {
      uResult = uResult + 1;
    }
  }

  return (uResult);
} // __strlen


/////////////////////////////////////////////////////////////////////////////
T_ULONG CString::__strstr(const T_CHAR * pLeft, const T_CHAR * pRight) {
  T_ULONG uResult = npos;
 
  T_ULONG i = 0; 
  while ((uResult == npos) && (pLeft[i] != 0)) {
    T_ULONG j = 0;
    
    while ((uResult == npos) && (pLeft[i + j] != 0) && (pRight[j] != 0)) {
      if (pLeft[i + j] != pRight[j]) {
        break;
      } else {
        j = j + 1;
      }
    }
    if (pRight[j] == 0) {
      uResult = i;
    }    
    
    i = i + 1;
  }

  return (uResult);
} // __strstr


/////////////////////////////////////////////////////////////////////////////
void CString::__construct(const T_CHAR * pString, T_ULONG uLength) {
  if (pString != NULL) {
    m_Length = uLength;
    m_String = (T_CHAR *)malloc((m_Length + 1) * sizeof(T_CHAR));
    __strcpy(m_String, pString, m_Length);
    m_String[m_Length] = 0;
  } else {
    m_Length = 0;
    m_String = NULL;
  }
} // __construct


/////////////////////////////////////////////////////////////////////////////
void CString::__destruct() {
  if (m_String != NULL) {
    free(m_String);
    m_String = NULL;
    m_Length = 0;
  }
} // __destruct


/////////////////////////////////////////////////////////////////////////////
CString::CString(const T_CHAR * pString, T_ULONG uLength) {
  __construct(pString, (uLength == 0) ? __strlen(pString) : uLength);
} // CString


/////////////////////////////////////////////////////////////////////////////
CString::CString(T_CHAR cChar) {
  __construct(&cChar, 1);
} // CString


#if ((OS_FAMILY == OSF_WINDOWS) | (OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

#if (OS_FAMILY == OSF_WINDOWS) 
#define __BASE__CString__SPRINTF sprintf_s
#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))
#define __BASE__CString__SPRINTF sprintf
#else
#error unsupported platform
#endif

/////////////////////////////////////////////////////////////////////////////
CString::CString(T_ULONG uValue) {
  __T_CHAR pBuffer[__BASE__CString__BUFFER_SIZE];

  __BASE__CString__SPRINTF(pBuffer, "%d", uValue);
  __construct(pBuffer, __strlen(pBuffer));
} // CString


/////////////////////////////////////////////////////////////////////////////
CString::CString(T_LONG iValue) {
  __T_CHAR pBuffer[__BASE__CString__BUFFER_SIZE];

  __BASE__CString__SPRINTF(pBuffer, "%d", iValue);
  __construct(pBuffer, __strlen(pBuffer));
} // CString



/////////////////////////////////////////////////////////////////////////////
CString::CString(T_DOUBLE dValue) {
  __T_CHAR pBuffer[__BASE__CString__BUFFER_SIZE];

  __BASE__CString__SPRINTF(pBuffer, "%f", dValue);
  __construct(pBuffer, __strlen(pBuffer));
} // CString

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif


/////////////////////////////////////////////////////////////////////////////
CString::~CString() {
  __destruct();
} // ~CString


/////////////////////////////////////////////////////////////////////////////
CString::CString(const CString & tString) {
  __construct(tString.m_String, tString.m_Length);
} // CString


/////////////////////////////////////////////////////////////////////////////
CString & CString::operator=(const CString & tString) {
  __destruct();
  __construct(tString.m_String, tString.m_Length);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
CString & CString::operator=(const T_CHAR * pString) {
  (* this) = CString(pString);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
T_CHAR & CString::operator[](T_ULONG uIndex) const {
  return (m_String[uIndex]);
} // operator[]


/////////////////////////////////////////////////////////////////////////////
T_CHAR & CString::operator[](T_INT uIndex) const {
  return (m_String[(T_ULONG)uIndex]);
} // operator[]


/////////////////////////////////////////////////////////////////////////////
CString::operator const T_CHAR * () const {
  return (m_String);
} // operator const T_CHAR *


/////////////////////////////////////////////////////////////////////////////
T_BOOLEAN CString::operator==(const CString & tString) const {
  T_BOOLEAN bResult = false;

  if (m_Length == tString.m_Length) {
    if (__strcmp(m_String, tString.m_String) == 0) {
      bResult = true;
    }
  }

  return (bResult);
} // operator==


/////////////////////////////////////////////////////////////////////////////
T_BOOLEAN CString::operator==(const T_CHAR * pString) const {
  return ((* this) == CString(pString));
} // operator==


/////////////////////////////////////////////////////////////////////////////
T_BOOLEAN CString::operator!=(const CString & tString) const {
  return (!((* this) == tString));
} // operator!=


/////////////////////////////////////////////////////////////////////////////
T_BOOLEAN CString::operator!=(const T_CHAR * pString) const {
  return ((* this) != CString(pString));
} // operator!=


/////////////////////////////////////////////////////////////////////////////
T_BOOLEAN CString::operator<(const CString & tString) const {
  T_BOOLEAN bResult = false;

  if (__strcmp(m_String, tString.m_String) == -1) {
    bResult = true;
  }

  return (bResult);
} // operator<


/////////////////////////////////////////////////////////////////////////////
CString CString::operator+(const CString & tString) const {
  CString sResult((T_CHAR *)NULL);

  sResult.m_Length = m_Length + tString.m_Length;
  if (sResult.m_Length != 0) {
    sResult.m_String = (T_CHAR *)malloc((sResult.m_Length + 1) * sizeof(T_CHAR));
    __strcpy(sResult.m_String, m_String, m_Length);
    __strcpy(sResult.m_String + m_Length, tString.m_String, tString.m_Length);
    sResult.m_String[sResult.m_Length] = '\0';
  }

  return (sResult);
} // operator+


/////////////////////////////////////////////////////////////////////////////
CString CString::operator+(const T_CHAR * pString) const {
  return ((* this) + CString(pString));
} // operator+


/////////////////////////////////////////////////////////////////////////////
CString CString::operator+(T_CHAR cChar) const {
  return ((* this) + CString(cChar));
} // operator+


#if ((OS_FAMILY == OSF_WINDOWS) | (OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

/////////////////////////////////////////////////////////////////////////////
CString CString::operator+(T_ULONG uValue) const {
  return ((* this) + CString(uValue));
} // operator+


/////////////////////////////////////////////////////////////////////////////
CString CString::operator+(T_LONG iValue) const {
  return ((* this) + CString(iValue));
} // operator+


/////////////////////////////////////////////////////////////////////////////
CString CString::operator+(T_DOUBLE dValue) const {
  return ((* this) + CString(dValue));
} // operator+

#elif (PA_FAMILY == PAF_AVR)

#else
#error unsupported platform
#endif


/////////////////////////////////////////////////////////////////////////////
T_BOOLEAN CString::IsValid() const {
  return (m_String != NULL);
} // IsValid


/////////////////////////////////////////////////////////////////////////////
T_BOOLEAN CString::IsLower(T_ULONG uIndex) const {
  T_BOOLEAN bResult = false;

  if ((m_String[uIndex] >= 97) && (m_String[uIndex] <= 122)) {
    bResult = true;
  }

  return (bResult);
} // IsLower


/////////////////////////////////////////////////////////////////////////////
T_BOOLEAN CString::IsUpper(T_ULONG uIndex) const {
  T_BOOLEAN bResult = false;

  if ((m_String[uIndex] >= 65) && (m_String[uIndex] <= 90)) {
    bResult = true;
  }

  return (bResult);
} // IsUpper


/////////////////////////////////////////////////////////////////////////////
T_BOOLEAN CString::IsNumeric(T_ULONG uIndex) const {
  T_BOOLEAN bResult = false;

  if ((m_String[uIndex] >= 48) && (m_String[uIndex] <= 57)) {
    bResult = true;
  }

  return (bResult);
} // IsNumeric


/////////////////////////////////////////////////////////////////////////////
CString CString::ToLower() const {
  CString sResult(* this);

  for (T_ULONG i = 0; i < sResult.m_Length; i++) {
    if ((sResult.m_String[i] >= 65) && (sResult.m_String[i] <= 90)) {
      sResult.m_String[i] = sResult.m_String[i] + 32;
    }
  }

  return (sResult);
} // ToLower


/////////////////////////////////////////////////////////////////////////////
CString CString::ToUpper() const {
  CString sResult(* this);

  for (T_ULONG i = 0; i < sResult.m_Length; i++) {
    if ((sResult.m_String[i] >= 97) && (sResult.m_String[i] <= 122)) {
      sResult.m_String[i] = sResult.m_String[i] - 32;
    }
  }

  return (sResult);
} // ToLower


/////////////////////////////////////////////////////////////////////////////
CString CString::SubString(T_ULONG uIndex, T_ULONG uLength) const {
  return (CString(&operator[](uIndex), uLength));
} // SubString


/////////////////////////////////////////////////////////////////////////////
T_ULONG CString::Find(const T_STRING & sValue) const {
  return (__strstr(m_String, C_STR(sValue)));
} // Find

} // namespace BASE
