
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_cqueue.h - ::DATASTRUCTURE::CQueue<T> class header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE_CQUEUE
#define _DATASTRUCTURE_CQUEUE

#include "datastructure_configuration.h"

#include "datastructure_clist.h"

namespace DATASTRUCTURE {

DERIVE_ABSTRACT_EXCEPTION(::DATASTRUCTURE::EList, EQueue);

template <class T>
class CQueue :
  public ::DATASTRUCTURE::CList<T> {
public:
  //! constructor
  CQueue();
  //! destructor
  virtual ~CQueue();

  //! copy constructor
  CQueue(const CQueue<T> & tQueue);
  //! assignment operator
  CQueue<T> & operator=(const CQueue<T> & tQueue);

  //! push element
  void Push(const T & tElement);
  //! pop element
  T Pop();
}; // class CQueue

} // namespace DATASTRUCTURE


/////////////////////////////////////////////////////////////////////////////
//
// ::DATASTRUCTURE::CQueue<T> class source
//
/////////////////////////////////////////////////////////////////////////////

namespace DATASTRUCTURE {

/////////////////////////////////////////////////////////////////////////////
template <class T>
CQueue<T>::CQueue() {
} // CQueue


/////////////////////////////////////////////////////////////////////////////
template <class T>
CQueue<T>::~CQueue() {
} // ~CQueue


/////////////////////////////////////////////////////////////////////////////
template <class T>
CQueue<T>::CQueue(const CQueue<T> & tQueue) :
  ::DATASTRUCTURE::CList<T>(tQueue) {
} // CQueue


/////////////////////////////////////////////////////////////////////////////
template <class T>
CQueue<T> & CQueue<T>::operator=(const CQueue<T> & tQueue) {
  ::DATASTRUCTURE::CList<T>::operator=(tQueue);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
template <class T>
void CQueue<T>::Push(const T & tElement) {
  ::DATASTRUCTURE::CList<T>::Insert(tElement);
} // Push


/////////////////////////////////////////////////////////////////////////////
template <class T>
T CQueue<T>::Pop() {
  return (::DATASTRUCTURE::CList<T>::Erase(0));
} // Pop

} // namespace DATASTRUCTURE

#endif // #ifndef _DATASTRUCTURE_CQUEUE
