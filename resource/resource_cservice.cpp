
/////////////////////////////////////////////////////////////////////////////
//
// resource_cservice.cpp - ::RESOURCE::CService class source
//
/////////////////////////////////////////////////////////////////////////////

#include "resource_cservice.h"

namespace RESOURCE {

#if (OS_FAMILY == OSF_WINDOWS)

// pointer to service instance
CService * CService::STATIC_pService = NULL;

/////////////////////////////////////////////////////////////////////////////
void CService::STATIC_Install(const T_STRING & sName, const T_STRING & sDisplayName, const T_STRING & sBinaryPath) {
  SC_HANDLE hServiceControlManager;
  SC_HANDLE hService;

  // open service control manager
  hServiceControlManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_CREATE_SERVICE);
  if (hServiceControlManager == NULL) {
    EXCEPTION(RESOURCE, ::RESOURCE::CService, STATIC_Install,
    MESSAGE("FATAL ERROR: unable to open service control manager"));
    THROW(EService, UNABLE_INSTALL);
  }

  // create service
  hService = CreateService(hServiceControlManager, C_STR(sName), C_STR(sDisplayName), SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, C_STR(sBinaryPath), NULL, NULL,  NULL, NULL, NULL );
  if (hService == NULL) {
    CloseServiceHandle(hServiceControlManager);
    EXCEPTION(RESOURCE, ::RESOURCE::CService, STATIC_Install,
    MESSAGE("FATAL ERROR: unable to create service"));
    THROW(EService, UNABLE_INSTALL);
  }

  // close handles
  CloseServiceHandle(hServiceControlManager);
  CloseServiceHandle(hService);
} // STATIC_Install


/////////////////////////////////////////////////////////////////////////////
void CService::STATIC_Uninstall(const T_STRING & sName) {
  SC_HANDLE hServiceControlManager;
  SC_HANDLE hService;

  // open service control manager
  hServiceControlManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_CREATE_SERVICE);
  if (hServiceControlManager == NULL) {
    EXCEPTION(RESOURCE, ::RESOURCE::CService, STATIC_Uninstall,
    MESSAGE("FATAL ERROR: unable to open service control manager"));
    THROW(EService, UNABLE_UNINSTALL);
  }

  // open service
  hService = OpenService(hServiceControlManager, C_STR(sName), DELETE);
  if (hService == NULL) {
    CloseServiceHandle(hServiceControlManager);
    EXCEPTION(RESOURCE, ::RESOURCE::CService, STATIC_Uninstall,
    MESSAGE("FATAL ERROR: unable to open service"));
    THROW(EService, UNABLE_UNINSTALL);
  }

  // delete service
  if (DeleteService(hService) == FALSE) {
    CloseServiceHandle(hServiceControlManager);
    CloseServiceHandle(hService);
    EXCEPTION(RESOURCE, ::RESOURCE::CService, STATIC_Uninstall,
    MESSAGE("FATAL ERROR: unable to delete service"));
    THROW(EService, UNABLE_UNINSTALL);
  }

  // close handles
  CloseServiceHandle(hServiceControlManager);
  CloseServiceHandle(hService);
} // STATIC_Uninstall


/////////////////////////////////////////////////////////////////////////////
void CService::STATIC_Start(const T_STRING & sName) {
  SC_HANDLE hServiceControlManager;
  SC_HANDLE hService;

  // open service control manager
  hServiceControlManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_CREATE_SERVICE);
  if (hServiceControlManager == NULL) {
    EXCEPTION(RESOURCE, ::RESOURCE::CService, STATIC_Start,
    MESSAGE("FATAL ERROR: unable to open service control manager"));
    THROW(EService, UNABLE_START);
  }

  // open service
  hService = OpenService(hServiceControlManager, C_STR(sName), SERVICE_START);
  if (hService == NULL) {
    CloseServiceHandle(hServiceControlManager);
    EXCEPTION(RESOURCE, ::RESOURCE::CService, STATIC_Start,
    MESSAGE("FATAL ERROR: unable to open service"));
    THROW(EService, UNABLE_START);
  }

  // start service
  if (StartService(hService, 0, NULL) == FALSE) {
    CloseServiceHandle(hServiceControlManager);
    CloseServiceHandle(hService);
    EXCEPTION(RESOURCE, ::RESOURCE::CService, STATIC_Start,
    MESSAGE("FATAL ERROR: unable to start service"));
    THROW(EService, UNABLE_START);
  }

  // close handles
  CloseServiceHandle(hServiceControlManager);
  CloseServiceHandle(hService);
} // STATIC_Start


/////////////////////////////////////////////////////////////////////////////
void CService::STATIC_Stop(const T_STRING & sName) {
  SC_HANDLE hServiceControlManager;
  SC_HANDLE hService;
  SERVICE_STATUS tStatus;

  // open service control manager
  hServiceControlManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_CREATE_SERVICE);
  if (hServiceControlManager == NULL) {
    EXCEPTION(RESOURCE, ::RESOURCE::CService, STATIC_Stop,
    MESSAGE("FATAL ERROR: unable to open service control manager"));
    THROW(EService, UNABLE_STOP);
  }

  // open service
  hService = OpenService(hServiceControlManager, C_STR(sName), SERVICE_STOP);
  if (hService == NULL) {
    CloseServiceHandle(hServiceControlManager);
    EXCEPTION(RESOURCE, ::RESOURCE::CService, STATIC_Stop,
    MESSAGE("FATAL ERROR: unable to open service"));
    THROW(EService, UNABLE_STOP);
  }

  // stop service
  ControlService(hService, SERVICE_CONTROL_STOP, &tStatus);

  // close handles
  CloseServiceHandle(hServiceControlManager);
  CloseServiceHandle(hService);
} // STATIC_Stop


/////////////////////////////////////////////////////////////////////////////
void WINAPI CService::STATIC_Start(DWORD uArgc, LPTSTR * pArgv) {
  // register control handler
  STATIC_pService->m_hStatus = RegisterServiceCtrlHandler("SERVICE", STATIC_ServiceControlHandler);
  if (STATIC_pService->m_hStatus == NULL) {
    EXCEPTION(RESOURCE, ::RESOURCE::CService, STATIC_Start,
    MESSAGE("FATAL ERROR: unable to register service control handler"));
    THROW(EService, UNABLE_EXECUTE);
  }

  // set state to SERVICE_RUNNING
  STATIC_pService->m_tStatus.dwCurrentState = SERVICE_RUNNING;
  SetServiceStatus(STATIC_pService->m_hStatus, &(STATIC_pService->m_tStatus));
} // STATIC_Start


/////////////////////////////////////////////////////////////////////////////
void WINAPI CService::STATIC_ServiceControlHandler(DWORD uOperationCode) {
  switch (uOperationCode) {
  case SERVICE_CONTROL_STOP :
  case SERVICE_CONTROL_SHUTDOWN : {
    // set state to SERVICE_STOP_PENDING
    STATIC_pService->m_tStatus.dwCurrentState = SERVICE_STOP_PENDING;
    SetServiceStatus(STATIC_pService->m_hStatus, &(STATIC_pService->m_tStatus));

    // set state to SERVICE_STOPPED
    STATIC_pService->m_tStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(STATIC_pService->m_hStatus, &(STATIC_pService->m_tStatus));
    break;
  }
  }
} // STATIC_ServiceControlHandler


/////////////////////////////////////////////////////////////////////////////
CService::CService(::BASE::CObject::synchronizations synchronization, const T_TIME & tTimeOut, const T_STRING & sName) :
  ::RESOURCE::CApplication(synchronization, tTimeOut),
  m_Name(sName) {

  STATIC_pService = this;
  m_tStatus.dwServiceType = SERVICE_WIN32;
  m_tStatus.dwCurrentState = SERVICE_START_PENDING;
  m_tStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
  m_tStatus.dwWin32ExitCode = 0;
  m_tStatus.dwServiceSpecificExitCode = 0;
  m_tStatus.dwCheckPoint = 0;
  m_tStatus.dwWaitHint = 0;
} // CService


/////////////////////////////////////////////////////////////////////////////
CService::~CService() {
  STATIC_pService = NULL;
} // ~CService


/////////////////////////////////////////////////////////////////////////////
void CService::Run() {
  SERVICE_TABLE_ENTRY tDispatchTable[] = {
    { (LPSTR)C_STR(m_Name), CService::STATIC_Start },
    { NULL, NULL } };
  
  // start service
  if (StartServiceCtrlDispatcher(tDispatchTable) == FALSE) {
    EXCEPTION(RESOURCE, ::RESOURCE::CService, Run,
    MESSAGE("FATAL ERROR: unable to start service control dispatcher"));
    THROW(EService, UNABLE_EXECUTE);
  }
} // Run

#endif // #if (OS_FAMILY == OSF_WINDOWS)

} // namespace RESOURCE
