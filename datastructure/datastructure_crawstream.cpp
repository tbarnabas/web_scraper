
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_crawstream.cpp - ::DATASTRUCTURE::CRawStream class source
//
/////////////////////////////////////////////////////////////////////////////

#include "datastructure_crawstream.h"

namespace DATASTRUCTURE {

/////////////////////////////////////////////////////////////////////////////
CRawStream::CRawStream() {
} // CRawStream


/////////////////////////////////////////////////////////////////////////////
CRawStream::~CRawStream() {
} // ~CRawStream


/////////////////////////////////////////////////////////////////////////////
CRawStream::CRawStream(const CRawStream & tRawStream) :
  ::DATASTRUCTURE::CStream(tRawStream) {
} // CRawStream


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator=(const CRawStream & tRawStream) {
  ::DATASTRUCTURE::CStream::operator=(tRawStream);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(::DATASTRUCTURE::CArray<T_BYTE> * pArray) {
  ::DATASTRUCTURE::CStream::operator<<(pArray);
  return (* this);
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(T_BOOLEAN bValue) {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

  tElement.Create(new ::DATASTRUCTURE::CArray<T_BYTE>((T_BYTE *)&bValue, sizeof(T_BOOLEAN)));
  ::DATASTRUCTURE::CStream::operator<<(tElement);

  return (* this);  
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(T_INT8 iValue) {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

  tElement.Create(new ::DATASTRUCTURE::CArray<T_BYTE>((T_BYTE *)&iValue, sizeof(T_INT8)));
  ::DATASTRUCTURE::CStream::operator<<(tElement);

  return (* this);  
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(T_UINT8 uValue) {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

  tElement.Create(new ::DATASTRUCTURE::CArray<T_BYTE>((T_BYTE *)&uValue, sizeof(T_UINT8)));
  ::DATASTRUCTURE::CStream::operator<<(tElement);

  return (* this);  
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(T_INT16 iValue) {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

  tElement.Create(new ::DATASTRUCTURE::CArray<T_BYTE>((T_BYTE *)&iValue, sizeof(T_INT16)));
  ::DATASTRUCTURE::CStream::operator<<(tElement);

  return (* this);  
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(T_UINT16 uValue) {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

  tElement.Create(new ::DATASTRUCTURE::CArray<T_BYTE>((T_BYTE *)&uValue, sizeof(T_UINT16)));
  ::DATASTRUCTURE::CStream::operator<<(tElement);

  return (* this);  
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(T_INT32 iValue) {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

  tElement.Create(new ::DATASTRUCTURE::CArray<T_BYTE>((T_BYTE *)&iValue, sizeof(T_INT32)));
  ::DATASTRUCTURE::CStream::operator<<(tElement);

  return (* this);  
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(T_UINT32 uValue) {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

  tElement.Create(new ::DATASTRUCTURE::CArray<T_BYTE>((T_BYTE *)&uValue, sizeof(T_UINT32)));
  ::DATASTRUCTURE::CStream::operator<<(tElement);

  return (* this);  
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(T_FLOAT fValue) {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

  tElement.Create(new ::DATASTRUCTURE::CArray<T_BYTE>((T_BYTE *)&fValue, sizeof(T_FLOAT)));
  ::DATASTRUCTURE::CStream::operator<<(tElement);

  return (* this);  
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(T_DOUBLE dValue) {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

  tElement.Create(new ::DATASTRUCTURE::CArray<T_BYTE>((T_BYTE *)&dValue, sizeof(T_DOUBLE)));
  ::DATASTRUCTURE::CStream::operator<<(tElement);

  return (* this);  
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(const T_CHAR * pValue) {
  return ((* this) << T_STRING(pValue));
} // operator<<


/////////////////////////////////////////////////////////////////////////////
CRawStream & CRawStream::operator<<(const T_STRING & sValue) {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

  tElement.Create(new ::DATASTRUCTURE::CArray<T_BYTE>((T_BYTE *)C_STR(sValue), sValue.GetLength() + 1));
  ::DATASTRUCTURE::CStream::operator<<(tElement);

  return (* this);  
} // operator<<


/////////////////////////////////////////////////////////////////////////////
REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > CRawStream::Write() {
  REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tResult;

  if (::DATASTRUCTURE::CStream::GetSize() > 0) {
    REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > tElement;

    T_ULONG uTotalSize = 0;
    T_ULONG uSize = ::DATASTRUCTURE::CStream::GetSize();
    for (::DATASTRUCTURE::CStream::iterator i = ::DATASTRUCTURE::CStream::Begin(); i != ::DATASTRUCTURE::CStream::End(); i++) {
      (* this) << (T_DWORD)(i->GetSize());
      (* this) << (* i);
      uTotalSize = uTotalSize + i->GetSize();
    }

    tResult.Create(new ::DATASTRUCTURE::CArray<T_BYTE>(uTotalSize));

    T_ULONG uPosition = 0;
    while (::DATASTRUCTURE::CStream::IsEmpty() == false) {
      tElement = ::DATASTRUCTURE::CStream::Pop();

      ::BASE::CArray<T_BYTE>::STATIC_Copy(tElement->GetElements(), tResult->GetElements() + uPosition, tElement->GetSize());
      uPosition = uPosition + tElement->GetSize();    
    }
  }

  return (tResult);
} // Write

} // namespace DATASTRUCTURE
