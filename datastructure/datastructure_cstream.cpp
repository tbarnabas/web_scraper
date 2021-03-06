
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_cstream.cpp - ::DATASTRUCTURE::CStream class source
//
/////////////////////////////////////////////////////////////////////////////

#include "datastructure_cstream.h"

namespace DATASTRUCTURE {

/////////////////////////////////////////////////////////////////////////////
CStream::CStream() { 
} // CStream


/////////////////////////////////////////////////////////////////////////////
CStream::~CStream() {
} // ~CStream


/////////////////////////////////////////////////////////////////////////////
CStream::CStream(const CStream & tStream) :
  ::DATASTRUCTURE::CQueue<REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > >(tStream) {
} // CStream


/////////////////////////////////////////////////////////////////////////////
CStream & CStream::operator=(const CStream & tStream) {
  ::DATASTRUCTURE::CQueue<REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > >::operator=(tStream);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
CStream & CStream::operator<<(::DATASTRUCTURE::CArray<T_BYTE> * pArray) {
  ::DATASTRUCTURE::CQueue<REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > >::Push(pArray);
  return (* this);
} // operator<<

} // namespace DATASTRUCTURE
