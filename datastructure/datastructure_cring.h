
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_cring.h - ::DATASTRUCTURE::CRing<T> class header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE_CRING
#define _DATASTRUCTURE_CRING

#include "datastructure_configuration.h"

#include "datastructure_carray.h"

namespace DATASTRUCTURE {

DERIVE_EXCEPTION_BEGIN(::DATASTRUCTURE::EArray, ERing)
  IS_EMPTY
DERIVE_EXCEPTION_END(ERing);

template <class T>
class CRing :
  public ::DATASTRUCTURE::CArray<T> {
private:
  //! size
  T_ULONG m_uSize;
  //! position to tail
  T_ULONG m_uTail;

  //! construct
  void __construct();

public:
  //! constructor
  CRing(const T * pElements = NULL, __T_ULONG uSize = 0);
  //! destructor
  virtual ~CRing();

  //! copy constructor
  CRing(const CRing<T> & tRing);
  //! assignment operator
  CRing<T> & operator=(const CRing<T> & tRing);

  //! if ring is empty then return true
  T_BOOLEAN IsEmpty() const;
  //! if ring is full then return true
  T_BOOLEAN IsFull() const;
  //! get size
  T_ULONG GetSize() const;

  //! clear elements
  void Clear();

  //! last element
  const T & Last() const;

  //! push element
  void Push(const T & tElement);
}; // class CRing

} // namespace DATASTRUCTURE


/////////////////////////////////////////////////////////////////////////////
//
// ::DATASTRUCTURE::CRing<T> class source
//
/////////////////////////////////////////////////////////////////////////////

namespace DATASTRUCTURE {

/////////////////////////////////////////////////////////////////////////////
template <class T>
void CRing<T>::__construct() {
  m_uSize = 0;
  m_uTail = 0;
} // __construct


/////////////////////////////////////////////////////////////////////////////
template <class T>
CRing<T>::CRing(const T * pElements, __T_ULONG uSize) :
  ::DATASTRUCTURE::CArray<T>(pElements, uSize) {
  __construct();
} // CRing


/////////////////////////////////////////////////////////////////////////////
template <class T>
CRing<T>::~CRing() {
} // ~CRing


/////////////////////////////////////////////////////////////////////////////
template <class T>
CRing<T>::CRing(const CRing<T> & tRing) :
  ::DATASTRUCTURE::CArray<T>(tRing),
  m_uSize(tRing.m_uSize),
  m_uTail(tRing.m_uTail) {
} // CRing


/////////////////////////////////////////////////////////////////////////////
template <class T>
CRing<T> & CRing<T>::operator=(const CRing<T> & tRing) {
  ::DATASTRUCTURE::CArray<T>::operator=(tRing);
  m_uSize = tRing.m_uSize;
  m_uTail = tRing.m_uTail;
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
template <class T>
T_BOOLEAN CRing<T>::IsEmpty() const {
  return (m_uSize == 0);
} // IsEmpty


/////////////////////////////////////////////////////////////////////////////
template <class T>
T_BOOLEAN CRing<T>::IsFull() const {
  return (m_uSize == ::DATASTRUCTURE::CArray<T>::GetSize());
} // IsFull


/////////////////////////////////////////////////////////////////////////////
template <class T>
T_ULONG CRing<T>::GetSize() const {
  return (m_uSize);
} // GetSize


/////////////////////////////////////////////////////////////////////////////
template <class T>
void CRing<T>::Clear() {
  __construct();
} // Clear


/////////////////////////////////////////////////////////////////////////////
template <class T>
const T & CRing<T>::Last() const {
  if (IsEmpty() == true) {
    EXCEPTION(MODULES, ::DATASTRUCTURE::CRing, Last,
    MESSAGE("is empty"));
    THROW(ERing, IS_EMPTY);
  }

  return (::DATASTRUCTURE::CArray<T>::operator[](m_uTail));
} // Last


/////////////////////////////////////////////////////////////////////////////
template <class T>
void CRing<T>::Push(const T & tElement) {
  ::DATASTRUCTURE::CArray<T>::operator[](m_uTail) = tElement;

  m_uTail = (m_uTail + 1) % ::DATASTRUCTURE::CArray<T>::GetSize();
  if (m_uSize < ::DATASTRUCTURE::CArray<T>::GetSize()) {
    m_uSize = m_uSize + 1;
  }
} // Push

} // namespace DATASTRUCTURE

#endif // #ifndef _DATASTRUCTURE_CRING
