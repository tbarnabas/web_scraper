
/////////////////////////////////////////////////////////////////////////////
//
// resource_cobjectmanager.h - ::RESOURCE::CObjectManager<K> class header
// --------------------------------------------------------------------------
//
// RESOURCE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _RESOURCE_COBJECTMANAGER
#define _RESOURCE_COBJECTMANAGER

#include "resource_configuration.h"

namespace RESOURCE {

DERIVE_EXCEPTION_BEGIN(::DATASTRUCTURE::EMap, EObjectManager)
  NOT_FOUND
DERIVE_EXCEPTION_END(EObjectManager);

template <class K>
class CObjectManager :
  public ::DATASTRUCTURE::CMap<K, REFERENCE< ::THREAD::CObject> > {
public:
  //! constructor
  CObjectManager();
  //! destructor
  virtual ~CObjectManager();

  //! copy constructor
  CObjectManager(const CObjectManager<K> & tObjectManager);
  //! assignment operator
  CObjectManager<K> & operator=(const CObjectManager<K> & tObjectManager);

  //! initialize
  virtual void Initialize();
  //! shutdown
  virtual void Shutdown(T_BOOLEAN bImmediate = false);
  //! maintain
  virtual void Maintain();
}; // class CObjectManager

} // namespace RESOURCE


/////////////////////////////////////////////////////////////////////////////
//
// ::RESOURCE::CObjectManager<K> class source
//
/////////////////////////////////////////////////////////////////////////////

namespace RESOURCE {

/////////////////////////////////////////////////////////////////////////////
template <class K>
CObjectManager<K>::CObjectManager() {
} // CObjectManager


/////////////////////////////////////////////////////////////////////////////
template <class K>
CObjectManager<K>::~CObjectManager() {
} // ~CObjectManager


/////////////////////////////////////////////////////////////////////////////
template <class K>
CObjectManager<K>::CObjectManager(const CObjectManager<K> & tObjectManager) :
  ::DATASTRUCTURE::CMap<K, REFERENCE< ::THREAD::CObject> >(tObjectManager) {
} // CObjectManager


/////////////////////////////////////////////////////////////////////////////
template <class K>
CObjectManager<K> & CObjectManager<K>::operator=(const CObjectManager<K> & tObjectManager) {
  ::DATASTRUCTURE::CMap<K, REFERENCE< ::THREAD::CObject> >::operator=(tObjectManager);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
template <class K>
void CObjectManager<K>::Initialize() {
  T_ULONG i = 0;
  T_ULONG uSize = ::DATASTRUCTURE::CMap<K, REFERENCE< ::THREAD::CObject> >::GetSize();

  for (i = 0; i < uSize; i++) {
    REFERENCE< ::THREAD::CObject> tObject = ::DATASTRUCTURE::CMap<K, REFERENCE< ::THREAD::CObject> >::SelectElement(i);

    THREADGUARD __tGuard(tObject);
    tObject->Initialize();
  }
} // Initialize


/////////////////////////////////////////////////////////////////////////////
template <class K>
void CObjectManager<K>::Shutdown(T_BOOLEAN bImmediate) {
  T_ULONG i = 0;
  T_ULONG uSize = ::DATASTRUCTURE::CMap<K, REFERENCE< ::THREAD::CObject> >::GetSize();

  for (i = uSize; i > 0 ; i--) {
    REFERENCE< ::THREAD::CObject> tObject = ::DATASTRUCTURE::CMap<K, REFERENCE< ::THREAD::CObject> >::SelectElement(i - 1);

    THREADGUARD __tGuard(tObject);
    IGNORE_EXCEPTION(tObject->Shutdown(bImmediate));
  }
} // Shutdown


/////////////////////////////////////////////////////////////////////////////
template <class K>
void CObjectManager<K>::Maintain() {
  T_ULONG i = 0;
  T_ULONG uSize = ::DATASTRUCTURE::CMap<K, REFERENCE< ::THREAD::CObject> >::GetSize();

  for (i = 0; i < uSize; i++) {
    REFERENCE< ::THREAD::CObject> tObject = ::DATASTRUCTURE::CMap<K, REFERENCE< ::THREAD::CObject> >::SelectElement(i);

    THREADGUARD __tGuard(tObject);
    IGNORE_EXCEPTION(tObject->Maintain());
  }
} // Maintain

} // namespace RESOURCE

#endif // #ifndef _RESOURCE_COBJECTMANAGER
