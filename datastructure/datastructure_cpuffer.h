
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_cpuffer.h - ::DATASTRUCTURE::CPuffer<T> class header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE_CPUFFER
#define _DATASTRUCTURE_CPUFFER

#include "datastructure_configuration.h"

#include "datastructure_carray.h"

namespace DATASTRUCTURE {

DERIVE_EXCEPTION_BEGIN(::DATASTRUCTURE::EArray, EPuffer)
  IS_EMPTY
DERIVE_EXCEPTION_END(EPuffer);

template <class T>
class CPuffer :
  public ::DATASTRUCTURE::CArray<T> {
public:
  MEMBER_GET_SET(T_ULONG, Position);

public:
  //! constructor
  CPuffer(const T * pElements = NULL, T_ULONG uSize = 0);
  //! destructor
  virtual ~CPuffer();

  //! copy constructor
  CPuffer(const CPuffer<T> & tPuffer);
  //! assignment operator
  CPuffer<T> & operator=(const CPuffer<T> & tPuffer);

  //! if puffer is empty then return true
  T_BOOLEAN IsEmpty() const;

  //! peek element
  T & Peek() const;
  //! next element
  T & Next();
}; // class CPuffer

} // namespace DATASTRUCTURE


/////////////////////////////////////////////////////////////////////////////
//
// ::DATASTRUCTURE::CPuffer<T> class source
//
/////////////////////////////////////////////////////////////////////////////

namespace DATASTRUCTURE {

/////////////////////////////////////////////////////////////////////////////
template <class T>
CPuffer<T>::CPuffer(const T * pElements, T_ULONG uSize) :
  ::DATASTRUCTURE::CArray<T>(pElements, uSize) {
  m_Position = 0;
} // CPuffer


/////////////////////////////////////////////////////////////////////////////
template <class T>
CPuffer<T>::~CPuffer() {
} // ~CPuffer


/////////////////////////////////////////////////////////////////////////////
template <class T>
CPuffer<T>::CPuffer(const CPuffer<T> & tPuffer) :
  ::DATASTRUCTURE::CArray<T>(tPuffer),
  m_Position(tPuffer.m_Position) {
} // CPuffer


/////////////////////////////////////////////////////////////////////////////
template <class T>
CPuffer<T> & CPuffer<T>::operator=(const CPuffer<T> & tPuffer) {
  ::DATASTRUCTURE::CArray<T>::operator=(tPuffer);
  m_Position = tPuffer.m_Position;
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
template <class T>
T_BOOLEAN CPuffer<T>::IsEmpty() const {
  return (m_Position >= ::DATASTRUCTURE::CArray<T>::GetSize());
} // IsEmpty


/////////////////////////////////////////////////////////////////////////////
template <class T>
T & CPuffer<T>::Peek() const {
  if (IsEmpty() == true) {
    EXCEPTION(MODULES, ::DATASTRUCTURE::CPuffer, Peek,
    MESSAGE("is empty"));
    THROW(EPuffer, IS_EMPTY);
  }

  return (::DATASTRUCTURE::CArray<T>::operator[](m_Position));
} // Peek


/////////////////////////////////////////////////////////////////////////////
template <class T>
T & CPuffer<T>::Next() {
  T & tResult = Peek();
  m_Position = m_Position + 1;
  return (tResult);
} // Next

} // namespace DATASTRUCTURE

#endif // #ifndef _DATASTRUCTURE_CPUFFER
