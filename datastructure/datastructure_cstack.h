
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_cstack.h - ::DATASTRUCTURE::CStack<T> class header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE_CSTACK
#define _DATASTRUCTURE_CSTACK

#include "datastructure_configuration.h"

#include "datastructure_clist.h"

namespace DATASTRUCTURE {

DERIVE_ABSTRACT_EXCEPTION(::DATASTRUCTURE::EList, EStack);

template <class T>
class CStack :
  public ::DATASTRUCTURE::CList<T> {
public:
  //! constructor
  CStack();
  //! destructor
  virtual ~CStack();

  //! copy constructor
  CStack(const CStack<T> & tStack);
  //! assignment operator
  CStack<T> & operator=(const CStack<T> & tStack);

  //! push element
  void Push(const T & tElement);
  //! pop element
  T Pop();
}; // class CStack

} // namespace DATASTRUCTURE


/////////////////////////////////////////////////////////////////////////////
//
// ::DATASTRUCTURE::CStack<T> class source
//
/////////////////////////////////////////////////////////////////////////////

namespace DATASTRUCTURE {

/////////////////////////////////////////////////////////////////////////////
template <class T>
CStack<T>::CStack() {
} // CStack


/////////////////////////////////////////////////////////////////////////////
template <class T>
CStack<T>::~CStack() {
} // ~CStack


/////////////////////////////////////////////////////////////////////////////
template <class T>
CStack<T>::CStack(const CStack<T> & tStack) :
  ::DATASTRUCTURE::CList<T>(tStack) {
} // CStack


/////////////////////////////////////////////////////////////////////////////
template <class T>
CStack<T> & CStack<T>::operator=(const CStack<T> & tStack) {
  ::DATASTRUCTURE::CList<T>::operator=(tStack);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
template <class T>
void CStack<T>::Push(const T & tElement) {
  ::DATASTRUCTURE::CList<T>::Insert(0, tElement);
} // Push


/////////////////////////////////////////////////////////////////////////////
template <class T>
T CStack<T>::Pop() {
  return (::DATASTRUCTURE::CList<T>::Erase(0));
} // Pop

} // namespace DATASTRUCTURE

#endif // #ifndef _DATASTRUCTURE_CSTACK
