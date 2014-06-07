
/////////////////////////////////////////////////////////////////////////////
//
// base_cthread.h - ::BASE::CThread class header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

// PLATFORM DEPENDENT
// note: not implemented on PAF_AVR platforms

#pragma once
#ifndef _BASE_CTHREAD
#define _BASE_CTHREAD

#include "base_configuration.h"

#include "base_cobject.h"

namespace BASE {

DERIVE_EXCEPTION_BEGIN(EObject, EThread)
  UNABLE_CONSTRUCT,
  UNABLE_DESTRUCT
DERIVE_EXCEPTION_END(EThread);

class BASE_EXPORT_IMPORT CThread :
  virtual public CObject {
public:
  //! get current thread id
  static T_ULONG GetThreadId();

private:
  //! indicate running
  T_BOOLEAN m_bRunning;

  //! construct
  void __construct(T_BOOLEAN bRunning);
  //! destruct
  void __destruct();

#if (OS_FAMILY == OSF_WINDOWS)

  //! thread
  HANDLE m_hThread;

  //! execute
  static DWORD WINAPI STATIC_Execute(LPVOID pArg);

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

  //! thread attribute
  pbase_attr_t m_tAttribute;
  //! thread
  pbase_t m_tThread;

  //! execute
  static void * STATIC_Execute(void * pArg);

#elif (PA_FAMILY == PAF_AVR)

  //! execute
  static void * STATIC_Execute(void * pArg);

#else
#error unsupported platform
#endif

protected:
  //! construct
  virtual void Construct() {}
  //! destruct
  virtual void Destruct() {}

  //! execute
  virtual void Execute() = 0;

public:
  //! constructor
  CThread();
  //! destructor
  virtual ~CThread();

  //! copy constructor
  CThread(const CThread & tThread);
  //! assignment operator
  CThread & operator=(const CThread & tThread);

  //! initialize
  virtual void Initialize();
  //! shutdown
  virtual void Shutdown(T_BOOLEAN bImmediate = false);
}; // class BASE_EXPORT_IMPORT CThread

} // namespace BASE

#endif // #ifndef _BASE_CTHREAD
