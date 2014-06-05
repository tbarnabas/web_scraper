
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_carray.h - ::DATASTRUCTURE::CArray<T, L> class header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE_CARRAY
#define _DATASTRUCTURE_CARRAY

#include "datastructure_configuration.h"

namespace DATASTRUCTURE {

DERIVE_ABSTRACT_EXCEPTION(::THREAD::EObject, EArray)

template <class T, class L = T_ULONG>
class CArray :
  public ::BASE::CArray<T, L>,
  virtual public ::THREAD::CObject {
public:
  //! constructor
  CArray(const T * pElements = NULL, L uSize = 0);
  //! constructor
  CArray(L uSize);
  //! destructor
  virtual ~CArray();

  //! copy constructor
  CArray(const CArray<T, L> & tArray);
  //! assignment operator
  CArray<T, L> & operator=(const CArray<T, L> & tArray);
}; // class CArray

} // namespace DATASTRUCTURE


/////////////////////////////////////////////////////////////////////////////
//
// ::DATASTRUCTURE::CArray<T, L> class source
//
/////////////////////////////////////////////////////////////////////////////

namespace DATASTRUCTURE {

/////////////////////////////////////////////////////////////////////////////
template <class T, class L>
CArray<T, L>::CArray(const T * pElements, L uSize) :
  ::BASE::CArray<T, L>(pElements, uSize) {
} // CArray


/////////////////////////////////////////////////////////////////////////////
template <class T, class L>
CArray<T, L>::CArray(L uSize) :
  ::BASE::CArray<T, L>(NULL, uSize) {
} // CArray


/////////////////////////////////////////////////////////////////////////////
template <class T, class L>
CArray<T, L>::~CArray() {
} // ~CArray


/////////////////////////////////////////////////////////////////////////////
template <class T, class L>
CArray<T, L>::CArray(const CArray<T, L> & tArray) :
  ::BASE::CArray<T, L>(tArray),
  ::THREAD::CObject(tArray) {
} // CArray


/////////////////////////////////////////////////////////////////////////////
template <class T, class L>
CArray<T, L> & CArray<T, L>::operator=(const CArray<T, L> & tArray) {
  ::BASE::CArray<T, L>::operator=(tArray);
  ::THREAD::CObject::operator=(tArray);
  return (* this);
} // operator=

} // namespace DATASTRUCTURE

#endif // #ifndef _DATASTRUCTURE_CARRAY
