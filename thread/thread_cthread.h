
/////////////////////////////////////////////////////////////////////////////
//
// thread_cthread.h - ::THREAD::CThread class header
// --------------------------------------------------------------------------
//
// THREAD
//
/////////////////////////////////////////////////////////////////////////////

// PLATFORM DEPENDENT
// note: not implemented on PAF_AVR platforms

#pragma once
#ifndef _THREAD_CTHREAD
#define _THREAD_CTHREAD

#include "thread_configuration.h"

#include "thread_cobject.h"

namespace THREAD {

DERIVE_EXCEPTION_BEGIN(::THREAD::EObject, EThread)
  UNABLE_CONSTRUCT,
  UNABLE_DESTRUCT
DERIVE_EXCEPTION_END(EThread);

class THREAD_EXPORT_IMPORT CThread :
  virtual public ::THREAD::CObject {
public:
  //! get current thread id
  static T_ULONG STATIC_GetCurrentThreadId();

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
  pthread_attr_t m_tAttribute;
  //! thread
  pthread_t m_tThread;

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
}; // class THREAD_EXPORT_IMPORT CThread

} // namespace THREAD

#endif // #ifndef _THREAD_CTHREAD
