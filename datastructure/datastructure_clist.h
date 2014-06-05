
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_clist.h - ::DATASTRUCTURE::CList<T> class header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE_CLIST
#define _DATASTRUCTURE_CLIST

#include "datastructure_configuration.h"

namespace DATASTRUCTURE {

DERIVE_EXCEPTION_BEGIN(::THREAD::EObject, EList)
  ELEMENT_NOT_FOUND
DERIVE_EXCEPTION_END(EList);

template <class T>
class CList :
  virtual public ::THREAD::CObject {
private:
  //! type definition for container
  struct TContainer {
    //! element
    T m_tElement;
    //! pointer to next container
    TContainer * m_pNext;
  };

  MEMBER(TContainer *, Head);
  MEMBER(TContainer *, Tail);
  MEMBER_GET(T_ULONG, Size);

  //! construct
  void __construct(TContainer * pHead);
  //! destruct
  void __destruct();

  //! select element
  TContainer * SelectContainer(T_ULONG uIndex) const;

public:
  //! iterator class
  class iterator {
  private:
    //! pointer to element
    TContainer * m_pElement;

  public:
    //! constructor
    iterator(TContainer * pElement = NULL) :
      m_pElement(pElement) {
    }
    //! destructor
    virtual ~iterator() {}

    //! copy constructor
    iterator(const iterator & tIterator) : 
      m_pElement(tIterator.m_pElement) {
    }
    //! assignment operator
    iterator & operator=(const iterator & tIterator) {
      m_pElement = tIterator.m_pElement;
      return (* this);
    }

    //! comparison operator
    __T_BOOLEAN operator==(const iterator & tIterator) const {
      return (m_pElement == tIterator.m_pElement);
    }
    //! comparison operator
    __T_BOOLEAN operator!=(const iterator & tIterator) const {
      return (!((* this) == tIterator));
    }

    //! indirection operator
    T & operator *() {
      return (m_pElement->m_tElement);
    }
    //! dereference operator
    T & operator->() {
      return (m_pElement->m_tElement);
    }

    //! increment operator
    iterator operator++() {
      TContainer * pElement = m_pElement;
      m_pElement = m_pElement->m_pNext;
      return (pElement);
    }
    //! increment operator
    iterator & operator++(int value) {
      m_pElement = m_pElement->m_pNext;
      return (* this);
    }
  };

public:
  //! constructor
  CList();
  //! destructor
  virtual ~CList();

  //! copy constructor
  CList(const CList<T> & tList);
  //! assignment operator
  CList<T> & operator=(const CList<T> & tList);

  //! if list is empty then return true
  T_BOOLEAN IsEmpty() const;
  //! clear elements
  void Clear();

  //! if element exists then return true
  T_BOOLEAN Exists(const T & tElement) const; // deprecated

  //! select first element
  T & First() const;
  //! select last element
  T & Last() const;
  //! select element
  T & Select(T_ULONG uIndex) const;

  //! insert element ordered by specified index
  void Insert(T_ULONG uIndex, const T & tElement);
  //! insert element at last
  void Insert(const T & tElement);
  //! erase element
  T Erase(T_ULONG uIndex);

  //! iterate begin
  iterator Begin() const;
  //! iterate end
  iterator End() const;
}; // class CList

} // namespace DATASTRUCTURE


/////////////////////////////////////////////////////////////////////////////
//
// ::DATASTRUCTURE::CList<T> class source
//
/////////////////////////////////////////////////////////////////////////////

namespace DATASTRUCTURE {

/////////////////////////////////////////////////////////////////////////////
template <class T>
void CList<T>::__construct(TContainer * pHead) {
  TContainer * pContainer = pHead;

  m_Head = NULL;
  m_Tail = NULL;
  m_Size = 0;

  while (pContainer != NULL) {
    Insert(GetSize(), pContainer->m_tElement);
    pContainer = pContainer->m_pNext;
  }
} // __construct


/////////////////////////////////////////////////////////////////////////////
template <class T>
void CList<T>::__destruct() {
  while (IsEmpty() == false) {
    Erase(0);
  }
} // __destruct


/////////////////////////////////////////////////////////////////////////////
template <class T>
typename CList<T>::TContainer * CList<T>::SelectContainer(T_ULONG uIndex) const {
  TContainer * pResult = m_Head;

  if (uIndex >= GetSize()) {
    EXCEPTION(MODULES, ::DATASTRUCTURE::CList, SelectContainer,
    MESSAGE("element not found"));
    THROW(EList, ELEMENT_NOT_FOUND);
  }

  while ((uIndex > 0) && (pResult != NULL)) {
    pResult = pResult->m_pNext;
    uIndex = uIndex - 1;
  }

  return (pResult);
} // SelectContainer


/////////////////////////////////////////////////////////////////////////////
template <class T>
CList<T>::CList() {
  __construct(NULL);
} // CList


/////////////////////////////////////////////////////////////////////////////
template <class T>
CList<T>::~CList() {
  __destruct();
} // ~CList


/////////////////////////////////////////////////////////////////////////////
template <class T>
CList<T>::CList(const CList<T> & tList) :
  ::THREAD::CObject(tList) {
  __construct(tList.m_Head);
} // CList


/////////////////////////////////////////////////////////////////////////////
template <class T>
CList<T> & CList<T>::operator=(const CList<T> & tList) {
  __destruct();
  ::THREAD::CObject::operator=(tList);
  __construct(tList.m_Head);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
template <class T>
T_BOOLEAN CList<T>::IsEmpty() const {
  return (GetSize() == 0);
} // IsEmpty


/////////////////////////////////////////////////////////////////////////////
template <class T>
void CList<T>::Clear() {
  __destruct();
} // Clear


/////////////////////////////////////////////////////////////////////////////
template <class T>
T_BOOLEAN CList<T>::Exists(const T & tElement) const {
  T_BOOLEAN bResult = false;
  TContainer * pContainer = m_Head;

  while ((bResult == false) && (pContainer != NULL)) {
    if (pContainer->m_tElement == tElement) {
      bResult = true;
    } else {
      pContainer = pContainer->m_pNext;
    }
  }

  return (bResult);
} // Exists


/////////////////////////////////////////////////////////////////////////////
template <class T>
T & CList<T>::First() const {
  return (SelectContainer(0)->m_tElement);
} // First


/////////////////////////////////////////////////////////////////////////////
template <class T>
T & CList<T>::Last() const {
  return (SelectContainer(GetSize() - 1)->m_tElement);
} // SelectLast


/////////////////////////////////////////////////////////////////////////////
template <class T>
T & CList<T>::Select(T_ULONG uIndex) const {
  return (SelectContainer(uIndex)->m_tElement);
} // Select


/////////////////////////////////////////////////////////////////////////////
template <class T>
void CList<T>::Insert(T_ULONG uIndex, const T & tElement) {
  TContainer * pContainer;

  if (uIndex > GetSize()) {
    uIndex = GetSize();
  }
  if (IsEmpty() == true) {
    pContainer = new TContainer;
    pContainer->m_tElement = tElement;
    pContainer->m_pNext = NULL;
    m_Head = pContainer;
    m_Tail = pContainer;
  } else {
    if (uIndex == 0) {
      pContainer = new TContainer;
      pContainer->m_tElement = tElement;
      pContainer->m_pNext = m_Head;
      m_Head = pContainer;
    } else {
      TContainer * pPreviousContainer = SelectContainer(uIndex - 1);

      pContainer = new TContainer;
      pContainer->m_tElement = tElement;
      pContainer->m_pNext = pPreviousContainer->m_pNext;
      pPreviousContainer->m_pNext = pContainer;
      if (pContainer->m_pNext == NULL) {
        m_Tail = pContainer;
      }
    }
  }

  m_Size = m_Size + 1;
} // Insert


/////////////////////////////////////////////////////////////////////////////
template <class T>
void CList<T>::Insert(const T & tElement) {
  TContainer * pContainer;

  pContainer = new TContainer;
  pContainer->m_tElement = tElement;
  pContainer->m_pNext = NULL;
  if (IsEmpty() == true) {
    m_Head = pContainer;
    m_Tail = pContainer;
  } else {
    m_Tail->m_pNext = pContainer;
    m_Tail = pContainer;
  }

  m_Size = m_Size + 1;
} // Insert


/////////////////////////////////////////////////////////////////////////////
template <class T>
T CList<T>::Erase(T_ULONG uIndex) {
  T tResult;
  TContainer * pContainer;

  if (uIndex == 0) {
    pContainer = SelectContainer(0);

    m_Head = pContainer->m_pNext;
    if (m_Head == NULL) {
      m_Tail = NULL;
    }
  } else {
    TContainer * pPreviousContainer = SelectContainer(uIndex - 1);
    pContainer = pPreviousContainer->m_pNext;

    pPreviousContainer->m_pNext = pContainer->m_pNext;
    if (pPreviousContainer->m_pNext == NULL) {
      m_Tail = pPreviousContainer;
    }
  }

  tResult = pContainer->m_tElement;

  delete pContainer;
  m_Size = m_Size - 1;

  return (tResult);
} // Erase


/////////////////////////////////////////////////////////////////////////////
template <class T>
typename CList<T>::iterator CList<T>::Begin() const {
  return (m_Head);
} // Begin


/////////////////////////////////////////////////////////////////////////////
template <class T>
typename CList<T>::iterator CList<T>::End() const {
  return (NULL);
} // End

} // namespace DATASTRUCTURE

#endif // #ifndef _DATASTRUCTURE_CLIST
