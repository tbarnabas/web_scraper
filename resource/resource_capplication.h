
/////////////////////////////////////////////////////////////////////////////
//
// resource_capplication.h - ::RESOURCE::CApplication class header
// --------------------------------------------------------------------------
//
// RESOURCE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _RESOURCE_CAPPLICATION
#define _RESOURCE_CAPPLICATION

#include "resource_configuration.h"

#include "resource_cobjectmanager.h"

namespace RESOURCE {

DERIVE_ABSTRACT_EXCEPTION(::THREAD::ELoopThread, EApplication);

class RESOURCE_EXPORT_IMPORT CApplication :
  public ::THREAD::CLoopThread {
private:
  //! copy constructor
  CApplication(const CApplication & tApplication);
  //! assignment operator
  CApplication & operator=(const CApplication & tApplication);

protected:
  //! object manager
  ::RESOURCE::CObjectManager<T_ULONG> m_tObjectManager;

  //! loop
  virtual void Loop();

public:
  //! constructor
  CApplication(::BASE::CObject::synchronizations synchronizations = ::BASE::CObject::NON_BLOCKED, const T_TIME & tTimeOut = T_TIME());
  //! destructor
  virtual ~CApplication();

  //! initialize
  virtual void Initialize();
  //! shutdown
  virtual void Shutdown(T_BOOLEAN bImmediate = false);
  //! maintain
  virtual void Maintain();

  //! run
  virtual void Run() = 0;
}; // class RESOURCE_EXPORT_IMPORT CApplication

} // namespace RESOURCE

#endif // #ifndef _RESOURCE_CAPPLICATION
