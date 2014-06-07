
/////////////////////////////////////////////////////////////////////////////
//
// base_creference.h - ::BASE::CReference<T> class header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _BASE_CREFERENCE
#define _BASE_CREFERENCE

#include "base_configuration.h"

namespace BASE {

#define REFERENCE ::BASE::CReference

//! create class environment
void CReference__Create();
//! destroy class environment
void CReference__Destroy();

template <class T>
class CReference {
private:
  //! pointer to object
  T * m_pObject;

  //! construct
  void __construct(T * pObject);
  //! destruct
  void __destruct();

public:
  //! constructor
  CReference(T * pObject = NULL);
  //! destructor
  virtual ~CReference();

  //! copy constructor
  CReference(const CReference<T> & tReference);
  //! assignment operator
  CReference<T> & operator=(const CReference<T> & tReference);
  //! assignment operator
  const T * operator=(const T * pObject);
  //! assignment operator
  const T & operator=(const T & tObject);

  //! pointer operator
  T * operator->() const;
  //! return pointer
  T * __ptr() const;
  //! type cast operator
  operator T * () const;
  //! type cast operator
  operator T & () const;

  //! if reference is valid then return true
  T_BOOLEAN IsValid() const;

  //! create
  CReference<T> & Create(T * pObject, storages storage = DYNAMIC);
  //! create
  CReference<T> & Create(T & tObject);
}; // class CReference

} // namespace BASE


/////////////////////////////////////////////////////////////////////////////
//
// ::BASE::CReference<T> class source
//
/////////////////////////////////////////////////////////////////////////////

namespace BASE {

//! lock reference mutex
void CReference__tMutex__Acquire();
//! unlock reference mutex
void CReference__tMutex__Release();


/////////////////////////////////////////////////////////////////////////////
template <class T>
void CReference<T>::__construct(T * pObject) {
  m_pObject = pObject;
  if (m_pObject != NULL) {
    CReference__tMutex__Acquire();
    m_pObject->SetReferences(m_pObject->GetReferences() + 1);
    CReference__tMutex__Release();
  }
} // __construct


/////////////////////////////////////////////////////////////////////////////
template <class T>
void CReference<T>::__destruct() {
  if (m_pObject != NULL) {
    T_ULONG uReferences = 0;

    CReference__tMutex__Acquire();
    uReferences = m_pObject->GetReferences() - 1;
    m_pObject->SetReferences(uReferences);
    CReference__tMutex__Release();

    if (uReferences == 0) {
      if (m_pObject->GetStorage() == DYNAMIC) {
        delete m_pObject;
      }
      m_pObject = NULL;
    }
  }
} // __destruct


/////////////////////////////////////////////////////////////////////////////
template <class T>
CReference<T>::CReference(T * pObject) {
  __construct(pObject);
} // CReference


/////////////////////////////////////////////////////////////////////////////
template <class T>
CReference<T>::~CReference() {
  __destruct();
} // ~CReference


/////////////////////////////////////////////////////////////////////////////
template <class T>
CReference<T>::CReference(const CReference<T> & tReference) {
  __construct(tReference.m_pObject);
} // CReference


/////////////////////////////////////////////////////////////////////////////
template <class T>
CReference<T> & CReference<T>::operator=(const CReference<T> & tReference) {
  __destruct();
  __construct(tReference.m_pObject);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
template <class T>
const T * CReference<T>::operator=(const T * pObject) {
  __destruct();
  __construct(const_cast<T *>(pObject));
  return (pObject);
} // operator=


/////////////////////////////////////////////////////////////////////////////
template <class T>
const T & CReference<T>::operator=(const T & tObject) {
  __destruct();
  __construct(const_cast<T *>(&tObject));
  return (tObject);
} // operator=


/////////////////////////////////////////////////////////////////////////////
template <class T>
T * CReference<T>::operator->() const {
  return (m_pObject);
} // operator->


/////////////////////////////////////////////////////////////////////////////
template <class T>
T * CReference<T>::__ptr() const {
  return (m_pObject);
} // __ptr


/////////////////////////////////////////////////////////////////////////////
template <class T>
CReference<T>::operator T * () const {
  return (m_pObject);
} // operator T *


/////////////////////////////////////////////////////////////////////////////
template <class T>
CReference<T>::operator T & () const {
  return (* m_pObject);
} // operator T &


/////////////////////////////////////////////////////////////////////////////
template <class T>
T_BOOLEAN CReference<T>::IsValid() const {
  return (m_pObject != NULL);
} // IsValid


/////////////////////////////////////////////////////////////////////////////
template <class T>
CReference<T> & CReference<T>::Create(T * pObject, storages storage) {
  __destruct();
  pObject->SetStorage(storage);
  __construct(pObject);
  return (* this);
} // Create


/////////////////////////////////////////////////////////////////////////////
template <class T>
CReference<T> & CReference<T>::Create(T & tObject) {
  return (Create(&tObject, tObject.GetStorage()));
} // Create

} // namespace BASE

#endif // #ifndef _BASE_CREFERENCE
