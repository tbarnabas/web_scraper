
/////////////////////////////////////////////////////////////////////////////
//
// base_iobject.h - ::BASE::IObject interface header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _BASE_IOBJECT
#define _BASE_IOBJECT

#include "base_configuration.h"

#include "base_creference.h"
#include "base_ctime.h"

namespace BASE {

class BASE_EXPORT_IMPORT IObject {
public:
  //! convert storages enumeration to string
  ENUMERATION_TO_STRING(storages,
    CASE_TO_STRING(STATIC)
    CASE_TO_STRING(DYNAMIC));
  //! convert string to storages enumeration
  STRING_TO_ENUMERATION(storages, 
    CASE_TO_ENUMERATION(STATIC)
    CASE_TO_ENUMERATION(DYNAMIC));

  //! enumeration for different operation types
  enum operations {
    READ,
    WRITE };
  //! convert operations enumeration to string
  ENUMERATION_TO_STRING(operations,
    CASE_TO_STRING(READ)
    CASE_TO_STRING(WRITE));
  //! convert string to operations enumeration
  STRING_TO_ENUMERATION(operations,
    CASE_TO_ENUMERATION(READ)
    CASE_TO_ENUMERATION(WRITE));

  //! enumeration for different mode types
  enum modes {
    BLOCKED,
    BLOCKED_UNTIL_SEND,
    BLOCKED_UNTIL_RECEIVE,
    BLOCKED_UNTIL_PROCESS,
    NON_BLOCKED };
  //! convert modes enumeration to string
  ENUMERATION_TO_STRING(modes,
    CASE_TO_STRING(BLOCKED)
    CASE_TO_STRING(BLOCKED_UNTIL_SEND)
    CASE_TO_STRING(BLOCKED_UNTIL_RECEIVE)
    CASE_TO_STRING(BLOCKED_UNTIL_PROCESS)
    CASE_TO_STRING(NON_BLOCKED));
  //! convert string to modes enumeration
  STRING_TO_ENUMERATION(modes,
    CASE_TO_ENUMERATION(BLOCKED)
    CASE_TO_ENUMERATION(BLOCKED_UNTIL_SEND)
    CASE_TO_ENUMERATION(BLOCKED_UNTIL_RECEIVE)
    CASE_TO_ENUMERATION(BLOCKED_UNTIL_PROCESS)
    CASE_TO_ENUMERATION(NON_BLOCKED));

public:
  IMEMBER_GET_SET(storages, Storage);
  IMEMBER_GET_SET(T_ULONG, References);
  IMEMBER_GETP_SETP__REFERENCE(IObject, Synch);
  IMEMBER_GET(T_BOOLEAN, Shutdown);
  IMEMBER_GET(T_BOOLEAN, Immediate);
    
public:
  //! destructor
  virtual ~IObject() {}  

  //! initialize
  virtual void Initialize() = 0;
  //! shutdown
  virtual void Shutdown(T_BOOLEAN bImmediate = false) = 0;
  //! maintain
  virtual void Maintain() = 0;

  //! acquire
  virtual void Acquire(operations operation = WRITE, modes mode = BLOCKED) = 0;
  //! release
  virtual void Release() = 0;
  //! send wakeup signal to one of waiting threads
  virtual void Signal() = 0;
  //! send wakeup signal to all of waiting threads
  virtual void Broadcast() = 0;
  //! waiting for wakeup signal
  virtual void Wait(const T_TIME & tTimeOut = T_TIME()) = 0;
}; // class BASE_EXPORT_IMPORT IObject

} // namespace BASE

#endif // #ifndef _BASE_IOBJECT
