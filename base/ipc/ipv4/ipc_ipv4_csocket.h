
/////////////////////////////////////////////////////////////////////////////
//
// ipc_ipv4_csocket.h - ::IPC::IPV4::CSocket class header
// --------------------------------------------------------------------------
//
// MODULES
//
/////////////////////////////////////////////////////////////////////////////

// PLATFORM DEPENDENT

#pragma once
#ifndef _IPC_IPV4_CSOCKET
#define _IPC_IPV4_CSOCKET

#include "../ipc_configuration.h"

namespace IPC {

namespace IPV4 {

#define __IPC__IPV4__CSocket__WAITINGQUEUESIZE 50
#define __IPC__IPV4__CSocket__ACCEPT_TIMEOUT T_TIME(10)
#define __IPC__IPV4__CSocket__CONNECT_TIMEOUT T_TIME(10)
#define __IPC__IPV4__CSocket__RECONNECT_TIMEOUT T_TIME(0, 10000)
#define __IPC__IPV4__CSocket__COMMUNICATE_TIMEOUT T_TIME(10)

DERIVE_EXCEPTION_BEGIN(::BASE::EObject, ESocket)
  UNABLE_INITIALIZE_MODULE,
  UNABLE_SHUTDOWN_MODULE,
  UNABLE_RESOLVE,
  UNABLE_CREATE,
  UNABLE_BIND_ADDRESS,
  UNABLE_LISTEN,
  UNABLE_ACCEPT,
  UNABLE_CONNECT,
  TIMED_OUT,
  CONNECTION_CLOSED,
  UNABLE_RECEIVE,
  UNABLE_SEND
DERIVE_EXCEPTION_END(ESocket);

class IPC_EXPORT_IMPORT CSocket :
  virtual public ::BASE::CObject {
public:

#ifdef ENABLE_OPENSSL

  //! pointer to SSL context
  static SSL_CTX * STATIC_pSSLContext;

#endif // #ifdef ENABLE_OPENSSL

  //! initialize module
  static void Initialize();
  //! shutdown module
  static void Shutdown(T_BOOLEAN bImmediate = false);

  //! resolve host
  static T_IPADDRESS ResolveHost(const T_STRING & sHost);

public:
  //! enumeration for different transmission types
  enum transmissions {
    STREAM,
    STREAM_SSL,
    DATAGRAM };
  //! convert transmissions enumeration to string
  ENUMERATION_TO_STRING(transmissions,
    CASE_TO_STRING(STREAM)
    CASE_TO_STRING(STREAM_SSL)
    CASE_TO_STRING(DATAGRAM));
  //! convert string to transmissions enumeration
  STRING_TO_ENUMERATION(transmissions,
    CASE_TO_ENUMERATION(STREAM)
    CASE_TO_ENUMERATION(STREAM_SSL)
    CASE_TO_ENUMERATION(DATAGRAM));

  //! transmission
  MEMBER_GET_SET(transmissions, transmission);

#if (OS_FAMILY == OSF_WINDOWS)

#define __IPC__IPV4__CSocket__INVALID_DESCRIPTOR INVALID_SOCKET
#define __IPC__IPV4__CSocket__SOCKET_ERROR SOCKET_ERROR

  typedef SOCKET descriptor_t;

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

#define __IPC__IPV4__CSocket__INVALID_DESCRIPTOR -1
#define __IPC__IPV4__CSocket__SOCKET_ERROR -1

  typedef __T_INT descriptor_t;

#else
#error unsupported operating system
#endif

protected:
  //! indicate listening
  T_BOOLEAN m_bListening;
  //! enable or disable close method
  T_BOOLEAN m_bClose;

  //! address
  sockaddr_in m_tAddress;

  //! descriptor
  descriptor_t m_tDescriptor;

#ifdef ENABLE_OPENSSL

  //! pointer to SSL descriptor
  SSL * m_pSSLDescriptor;

#endif // #ifdef ENABLE_OPENSSL

  //! construct
  void __construct(transmissions transmission, T_BOOLEAN bListening, T_BOOLEAN bClose = true, descriptor_t tDescriptor = __IPC__IPV4__CSocket__INVALID_DESCRIPTOR, const T_IPADDRESS & tIPAddress = T_IPADDRESS(), T_ULONG uPort = 0);
  //! destruct
  void __destruct();

  //! waiting for the specified operation
  static void STATIC_Wait(descriptor_t tDescriptor, ::BASE::IObject::operations operation, const T_TIME & tTimeOut);

public:
  //! constructor
  CSocket(transmissions transmission = STREAM, T_BOOLEAN bClose = true, descriptor_t tDescriptor = __IPC__IPV4__CSocket__INVALID_DESCRIPTOR);
  //! destructor
  virtual ~CSocket();

  //! copy constructor
  CSocket(const CSocket & tSocket);
  //! assignment operator
  CSocket & operator=(const CSocket & tSocket);

  //! listent to the specified address
  void Listen(const T_IPADDRESS & tIPAddress, T_ULONG uPort, T_UINT uWaitingQueueSize = __IPC__IPV4__CSocket__WAITINGQUEUESIZE);
  //! accept a new connection request
  REFERENCE<CSocket> Accept(const T_TIME & tTimeOut = __IPC__IPV4__CSocket__ACCEPT_TIMEOUT);

  //! connect to the specified address
  void Connect(const T_IPADDRESS & tIPAddress, T_ULONG uPort, const T_TIME & tConnectTimeOut = __IPC__IPV4__CSocket__CONNECT_TIMEOUT, const T_TIME & tReConnectTimeOut = __IPC__IPV4__CSocket__RECONNECT_TIMEOUT);

  //! close
  void Close();

  //! receive byte stream
  T_ULONG Receive(T_BYTE * pBuffer, T_ULONG uLength, const T_TIME & tTimeOut = __IPC__IPV4__CSocket__COMMUNICATE_TIMEOUT, ::BASE::IObject::modes mode = ::BASE::IObject::BLOCKED);
  //! send byte stream
  T_ULONG Send(const T_BYTE * pBuffer, T_ULONG uLength, const T_TIME & tTimeOut = __IPC__IPV4__CSocket__COMMUNICATE_TIMEOUT, ::BASE::IObject::modes mode = ::BASE::IObject::BLOCKED);
}; // class IPC_EXPORT_IMPORT CSocket

} // namespace IPV4

} // namespace IPC

#endif // #ifndef _IPC_IPV4_CSOCKET
