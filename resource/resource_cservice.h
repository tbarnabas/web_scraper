
/////////////////////////////////////////////////////////////////////////////
//
// resource_cservice.h - ::RESOURCE::CService class header
// --------------------------------------------------------------------------
//
// RESOURCE
//
/////////////////////////////////////////////////////////////////////////////

// PLATFORM DEPENDENT

#pragma once
#ifndef _RESOURCE_CSERVICE
#define _RESOURCE_CSERVICE

#include "resource_configuration.h"

#if (OS_FAMILY == OSF_WINDOWS)

#include "resource_capplication.h"

namespace RESOURCE {

DERIVE_EXCEPTION_BEGIN(::RESOURCE::EApplication, EService)
  UNABLE_INSTALL,
  UNABLE_UNINSTALL,
  UNABLE_START,
  UNABLE_STOP,
  UNABLE_EXECUTE
DERIVE_EXCEPTION_END(EService);

class RESOURCE_EXPORT_IMPORT CService :
  public ::RESOURCE::CApplication {
public:
  //! install
  static void STATIC_Install(const T_STRING & sName, const T_STRING & sDisplayName, const T_STRING & sBinaryPath);
  //! uninstall
  static void STATIC_Uninstall(const T_STRING & sName);
  //! start
  static void STATIC_Start(const T_STRING & sName);
  //! stop
  static void STATIC_Stop(const T_STRING & sName);

public:
  MEMBER_GET(T_STRING, Name);
  
private:
  //! pointer to service instance
  static CService * STATIC_pService;

  //! service status
  SERVICE_STATUS m_tStatus;
  //! service status handle
  SERVICE_STATUS_HANDLE m_hStatus;

  //! start service
  static void WINAPI STATIC_Start(DWORD uArgc, LPTSTR * pArgv);
  //! service control handler
  static void WINAPI STATIC_ServiceControlHandler(DWORD uOperationCode);

  //! copy constructor
  CService(const CService & tService) {}
  //! assignment operator
  CService & operator=(const CService & tService) {}

public:
  //! constructor
  CService(::BASE::CObject::synchronizations synchronizations = ::BASE::CObject::NON_BLOCKED, const T_TIME & tTimeOut = T_TIME(), const T_STRING & sName = T_STRING());
  //! destructor
  virtual ~CService();

  //! run
  virtual void Run();
}; // class RESOURCE_EXPORT_IMPORT CService

} // namespace RESOURCE

#endif // #if (OS_FAMILY == OSF_WINDOWS)

#endif // #ifndef _RESOURCE_CSERVICE
