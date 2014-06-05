
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_cmap.h - ::DATASTRUCTURE::CMap<K, T> class header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
/////////////////////////////////////////////////////////////////////////////

// PLATFORM DEPENDENT

#pragma once
#ifndef _DATASTRUCTURE_CMAP
#define _DATASTRUCTURE_CMAP

#include "datastructure_configuration.h"

#if (ENABLE_STL)

namespace DATASTRUCTURE {

DERIVE_EXCEPTION_BEGIN(::THREAD::EObject, EMap)
  NOT_FOUND
DERIVE_EXCEPTION_END(EMap);

template <class K, class T>
class CMap :
  virtual public ::THREAD::CObject {
private:
  //! elements
  ::std::map<K, T> m_tElements;

public:
  //! constructor
  CMap();
  //! destructor
  virtual ~CMap();

  //! copy constructor
  CMap(const CMap<K, T> & tMap);
  //! assignment operator
  CMap<K, T> & operator=(const CMap<K, T> & tMap);

  //! if map is empty then return true
  T_BOOLEAN IsEmpty() const;
  //! get size
  T_ULONG GetSize() const;

  //! if element exists then return true
  T_BOOLEAN Exists(const K & tKey) /*const*/;
  //! select element
  T & Select(const K & tKey) /*const*/;
  //! select key with index
  const K & SelectKey(T_ULONG uIndex) /*const*/;
  //! select element with index
  T & SelectElement(T_ULONG uIndex) /*const*/;

  //! clear elements
  void Clear();
  //! insert element
  const K & Insert(const K & tKey, const T & tElement);
  //! remove element
  T Remove(const K & tKey);
  //! remove element with index
  T RemoveElement(T_ULONG uIndex);
  //! erase element
  void Erase(const K & tKey);
  //! erase element with index
  void EraseElement(T_ULONG uIndex);
}; // class CMap

} // namespace DATASTRUCTURE


/////////////////////////////////////////////////////////////////////////////
//
// ::DATASTRUCTURE::CMap<K, T> class source
//
/////////////////////////////////////////////////////////////////////////////

namespace DATASTRUCTURE {

/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
CMap<K, T>::CMap() {
} // CMap


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
CMap<K, T>::~CMap() {
} // ~CMap


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
CMap<K, T>::CMap(const CMap<K, T> & tMap) :
  ::THREAD::CObject(tMap),
  m_tElements(tMap.m_tElements) {
} // CMap


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
CMap<K, T> & CMap<K, T>::operator=(const CMap<K, T> & tMap) {
  ::THREAD::CObject::operator=(tMap);
  m_tElements = tMap.m_tElements;
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
T_BOOLEAN CMap<K, T>::IsEmpty() const {
  return (m_tElements.empty());
} // IsEmpty


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
T_ULONG CMap<K, T>::GetSize() const {
  return ((T_ULONG)m_tElements.size());
} // GetSize


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
T_BOOLEAN CMap<K, T>::Exists(const K & tKey) /*const*/ {
  T_BOOLEAN bResult = false;

  IGNORE_EXCEPTION( 
    Select(tKey);
    bResult = true);

  return (bResult);
} // Exists


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
T & CMap<K, T>::Select(const K & tKey) /*const*/ {
  typename ::std::map<K, T>::iterator it;

  it = m_tElements.find(tKey);
  if (it == m_tElements.end()) {
    EXCEPTION(MODULES, ::DATASTRUCTURE::CMap, Select,
    MESSAGE("element not found"));
    THROW(EMap, NOT_FOUND);
  }

  return (it->second);
} // Select


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
const K & CMap<K, T>::SelectKey(T_ULONG uIndex) /*const*/ {
  typename ::std::map<K, T>::iterator it;
  T_ULONG i = 0;

  if (GetSize() <= uIndex) {
    EXCEPTION(MODULES, ::DATASTRUCTURE::CMap, SelectKey,
    MESSAGE("element not found"));
    THROW(EMap, NOT_FOUND);
  }
 
  it = m_tElements.begin();
  for (i = 0; i < uIndex; i++) {
    it++;
  }

  return (it->first);
} // SelectKey


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
T & CMap<K, T>::SelectElement(T_ULONG uIndex) /*const*/ {
  typename ::std::map<K, T>::iterator it;
  T_ULONG i = 0;

  if (GetSize() <= uIndex) {
    EXCEPTION(MODULES, ::DATASTRUCTURE::CMap, SelectElement,
    MESSAGE("element not found"));
    THROW(EMap, NOT_FOUND);
  }
 
  it = m_tElements.begin();
  for (i = 0; i < uIndex; i++) {
    it++;
  }

  return (it->second);
} // SelectElement


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
void CMap<K, T>::Clear() {
  m_tElements.clear();
} // Clear


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
const K & CMap<K, T>::Insert(const K & tKey, const T & tElement) {
  if (Exists(tKey) == true) {
    Erase(tKey);
  }
  m_tElements.insert(::std::make_pair(tKey, tElement));

  return (tKey);
} // Insert


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
T CMap<K, T>::Remove(const K & tKey) {
  T tResult; 
  typename ::std::map<K, T>::iterator it;

  it = m_tElements.find(tKey);
  if (it == m_tElements.end()) {
    EXCEPTION(MODULES, ::DATASTRUCTURE::CMap, Erase,
    MESSAGE("element not found"));
    THROW(EMap, NOT_FOUND);
  }

  tResult = it->second;
  m_tElements.erase(it);

  return (tResult);
} // Remove


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
T CMap<K, T>::RemoveElement(T_ULONG uIndex) {
  T tResult;
  typename ::std::map<K, T>::iterator it;
  T_ULONG i = 0;

  if (GetSize() <= uIndex) {
    EXCEPTION(MODULES, ::DATASTRUCTURE::CMap, EraseElement,
    MESSAGE("element not found"));
    THROW(EMap, NOT_FOUND);
  }

  it = m_tElements.begin();
  for (i = 0; i < uIndex; i++) {
    it++;
  }

  tResult = it->second;
  m_tElements.erase(it);

  return (tResult);  
} // RemoveElement


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
void CMap<K, T>::Erase(const K & tKey) {
  typename ::std::map<K, T>::iterator it;

  it = m_tElements.find(tKey);
  if (it == m_tElements.end()) {
    EXCEPTION(MODULES, ::DATASTRUCTURE::CMap, Erase,
    MESSAGE("element not found"));
    THROW(EMap, NOT_FOUND);
  }
  
  m_tElements.erase(it);
} // Erase


/////////////////////////////////////////////////////////////////////////////
template <class K, class T>
void CMap<K, T>::EraseElement(T_ULONG uIndex) {
  typename ::std::map<K, T>::iterator it;
  T_ULONG i = 0;

  if (GetSize() <= uIndex) {
    EXCEPTION(MODULES, ::DATASTRUCTURE::CMap, EraseElement,
    MESSAGE("element not found"));
    THROW(EMap, NOT_FOUND);
  }

  it = m_tElements.begin();
  for (i = 0; i < uIndex; i++) {
    it++;
  }

  m_tElements.erase(it);
} // EraseElement

} // namespace DATASTRUCTURE

#endif // #if (ENABLE_STL)

#endif // #ifndef _DATASTRUCTURE_CMAP
