
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_ctextstream.cpp - ::DATASTRUCTURE::CTextStream class source
//
/////////////////////////////////////////////////////////////////////////////

#include "datastructure_ctextstream.h"

namespace DATASTRUCTURE {

/////////////////////////////////////////////////////////////////////////////
CTextStream::CTextStream() {
} // CTextStream


/////////////////////////////////////////////////////////////////////////////
CTextStream::~CTextStream() {
} // ~CTextStream


/////////////////////////////////////////////////////////////////////////////
CTextStream::CTextStream(const CTextStream & tTextStream) :
  ::DATASTRUCTURE::CStream(tTextStream) {
} // CTextStream


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator=(const CTextStream & tTextStream) {
  ::DATASTRUCTURE::CStream::operator=(tTextStream);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(::DATASTRUCTURE::CArray<T_BYTE> * pArray) {
  ::DATASTRUCTURE::CStream::operator<<(pArray);
  return (* this);
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(T_BOOLEAN bValue) {
  return ((* this) << ((bValue == true) ? T_STRING("true") : T_STRING("false")));
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(T_INT8 iValue) {
  return ((* this) << T_STRING((T_LONG)iValue));
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(T_UINT8 uValue) {
  return ((* this) << T_STRING((T_ULONG)uValue));
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(T_INT16 iValue) {
  return ((* this) << T_STRING((T_LONG)iValue));
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(T_UINT16 uValue) {
  return ((* this) << T_STRING((T_ULONG)uValue));
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(T_INT32 iValue) {
  return ((* this) << T_STRING((T_LONG)iValue));
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(T_UINT32 uValue) {
  return ((* this) << T_STRING((T_ULONG)uValue));
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(T_FLOAT fValue) {
  return ((* this) << T_STRING((T_DOUBLE)fValue));
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(T_DOUBLE dValue) {
  return ((* this) << T_STRING(dValue));
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(const T_CHAR * pValue) {
  return ((* this) << T_STRING(pValue));
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CTextStream & CTextStream::operator<<(const T_STRING & sValue) {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

  tElement.Create(new ::DATASTRUCTURE::CArray<T_BYTE>((T_BYTE *)C_STR(sValue), sValue.GetLength()));
  ::DATASTRUCTURE::CStream::Push(tElement);

  return (* this);  
} // operator<<


/////////////////////////////////////////////////////////////////////////////
REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > CTextStream::Write() {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tResult;

  T_ULONG uTotalSize = 0;
  for (::DATASTRUCTURE::CStream::iterator i = ::DATASTRUCTURE::CStream::Begin(); i != ::DATASTRUCTURE::CStream::End(); i++) {
    uTotalSize = uTotalSize + i->GetSize();
  }

  tResult.Create(new ::DATASTRUCTURE::CArray<T_BYTE>(uTotalSize));

  T_ULONG uPosition = 0;
  while (::DATASTRUCTURE::CStream::IsEmpty() == false) {
    REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement = ::DATASTRUCTURE::CStream::Pop();

    ::BASE::CArray<T_BYTE>::STATIC_Copy(tElement->GetElements(), tResult->GetElements() + uPosition, tElement->GetSize());
    uPosition = uPosition + tElement->GetSize();    
  }

  return (tResult);
} // Write

} // namespace DATASTRUCTURE
