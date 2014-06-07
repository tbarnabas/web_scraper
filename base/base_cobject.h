
/////////////////////////////////////////////////////////////////////////////
//
// base_cobject.h - ::BASE::CObject class header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _BASE_COBJECT
#define _BASE_COBJECT

#include "base_configuration.h"

#include "base_iobject.h"
#include "base_creference.h"
#include "base_eexception.h"

namespace BASE {

DERIVE_ABSTRACT_EXCEPTION(EException, EObject);

class BASE_EXPORT_IMPORT CObject :
  virtual public IObject {
private:
  //! construct
  void __construct();

public:
  MEMBER_GET_SET(storages, Storage);
  MEMBER_GET_SET(T_ULONG, References);
  MEMBER_GETP_SETP__REFERENCE(IObject, Synch);
  MEMBER_GET(T_BOOLEAN, Shutdown);
  MEMBER_GET(T_BOOLEAN, Immediate);

public:
  //! constructor
  CObject();
  //! destructor
  virtual ~CObject();

  //! copy constructor
  CObject(const CObject & tObject);
  //! assignment operator
  CObject & operator=(const CObject & tObject);

  //! initialize
  virtual void Initialize() {}
  //! shutdown
  virtual void Shutdown(T_BOOLEAN bImmediate = false);
  //! maintain
  virtual void Maintain() {}

  //! acquire
  virtual void Acquire(IObject::operations operation = IObject::WRITE, IObject::modes mode = IObject::BLOCKED);
  //! release
  virtual void Release();
  //! send wakeup signal to one of waiting threads
  virtual void Signal();
  //! send wakeup signal to all of waiting threads
  virtual void Broadcast();
  //! waiting for wakeup signal
  virtual void Wait(const T_TIME & tTimeOut = T_TIME());
}; // class BASE_EXPORT_IMPORT CObject

} // namespace BASE

#endif // #ifndef _BASE_COBJECT
