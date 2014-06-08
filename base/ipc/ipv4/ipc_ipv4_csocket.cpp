
/////////////////////////////////////////////////////////////////////////////
//
// ipc_ipv4_csocket.cpp - ::IPC::IPV4::CSocket class source
//
/////////////////////////////////////////////////////////////////////////////

#include "ipc_ipv4_csocket.h"

namespace IPC {

namespace IPV4 {

#define __IPC__IPV4__CSocket__WSA_MAJOR_VERSION 2
#define __IPC__IPV4__CSocket__WSA_MINOR_VERSION 2

#define __IPC__IPV4__CSocket__RECEIVE_BUFFER_SIZE 131072
#define __IPC__IPV4__CSocket__SEND_BUFFER_SIZE 131072

#ifdef ENABLE_OPENSSL

#define __IPC__IPV4__CSocket__ENCRYPTION_LEVEL 128

SSL_CTX * CSocket::STATIC_pSSLContext = NULL;

#endif // #ifdef ENABLE_OPENSSL

/////////////////////////////////////////////////////////////////////////////
void CSocket::Initialize() {

#if (OS_FAMILY == OSF_WINDOWS)

  WORD uVersion = MAKEWORD(__IPC__IPV4__CSocket__WSA_MAJOR_VERSION, __IPC__IPV4__CSocket__WSA_MINOR_VERSION);
  WSADATA tData;

  // startup WSA
  if (WSAStartup(uVersion, &tData) != 0) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Initialize,
    MESSAGE("FATAL ERROR: unable to startup Windows Socket API"));
    THROW(ESocket, UNABLE_INITIALIZE_MODULE);
  }

  if ((LOBYTE(tData.wVersion) != __IPC__IPV4__CSocket__WSA_MAJOR_VERSION) || (HIBYTE(tData.wVersion) != __IPC__IPV4__CSocket__WSA_MINOR_VERSION)) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Initialize,
    MESSAGE("FATAL ERROR: Windows Socket API version number doesn't match"));
    THROW(ESocket, UNABLE_INITIALIZE_MODULE);
  }

#endif // (OS_FAMILY == OSF_WINDOWS)

#ifdef ENABLE_OPENSSL

  RSA * pRSAKey = NULL;
  X509 * pCertificate = NULL;
  EVP_PKEY * pEVPKey = NULL;

  // load error messages
  SSL_load_error_strings();

  // initialize SSL library
  SSL_library_init();

  // add all algorithms
  OpenSSL_add_all_algorithms();

  // create a new SSL context
  STATIC_pSSLContext = SSL_CTX_new(SSLv3_method());
  if (STATIC_pSSLContext == NULL) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Initialize,
    MESSAGE("FATAL ERROR: unable to create SSL socket"));
    THROW(ESocket, UNABLE_INITIALIZE_MODULE);
  }

  // generate a new RSA key
  pRSAKey = RSA_generate_key(__IPC__IPV4__CSocket__ENCRYPTION_LEVEL, 65537, NULL, NULL);
  if (pRSAKey == NULL) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Initialize,
    MESSAGE("FATAL ERROR: unable to generate RSA key"));
    THROW(ESocket, UNABLE_INITIALIZE_MODULE);
  }

  // use RSA key
  if (SSL_CTX_use_RSAPrivateKey(STATIC_pSSLContext, pRSAKey) != 1) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Initialize,
    MESSAGE("FATAL ERROR: unable to use RSA key"));
    THROW(ESocket, UNABLE_INITIALIZE_MODULE);
  }

  // create a new EVP key
  pEVPKey = EVP_PKEY_new();
  if (pEVPKey == NULL) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Initialize,
    MESSAGE("FATAL ERROR: unable to create EVP key"));
    THROW(ESocket, UNABLE_INITIALIZE_MODULE);
  }

  // create a new certificate
  pCertificate = X509_new();
  if (pCertificate == NULL) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Initialize,
    MESSAGE("FATAL ERROR: unable to create certificate"));
    THROW(ESocket, UNABLE_INITIALIZE_MODULE);
  }

  // assign RSA key to EVP key
  if (EVP_PKEY_assign_RSA(pEVPKey, pRSAKey) != 1) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Initialize,
    MESSAGE("FATAL ERROR: unable to assign RSA key to EVP key"));
    THROW(ESocket, UNABLE_INITIALIZE_MODULE);
  }

  // set certificate
  X509_set_version(pCertificate, 2);
  ASN1_INTEGER_set(X509_get_serialNumber(pCertificate), 0);
  X509_gmtime_adj(X509_get_notBefore(pCertificate), 0);
  X509_gmtime_adj(X509_get_notAfter(pCertificate), (long)(60 * 60 * 24 * 365));
  X509_set_pubkey(pCertificate, pEVPKey);
  
  // create digital signature
  X509_sign(pCertificate, pEVPKey, EVP_md5());

  // use certificate
  if (SSL_CTX_use_certificate(STATIC_pSSLContext, pCertificate) != 1) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Initialize,
    MESSAGE("FATAL ERROR: unable to use certificate"));
    THROW(ESocket, UNABLE_INITIALIZE_MODULE);
  }

  // check private key
  if (SSL_CTX_check_private_key(STATIC_pSSLContext) != 1) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Initialize,
    MESSAGE("FATAL ERROR: private key is corrupt"));
    THROW(ESocket, UNABLE_INITIALIZE_MODULE);
  }

#endif // #ifdef ENABLE_OPENSSL

} // Initialize


/////////////////////////////////////////////////////////////////////////////
void CSocket::Shutdown(T_BOOLEAN bImmediate) {

#ifdef ENABLE_OPENSSL

  // free SSL context
  if (STATIC_pSSLContext != NULL) {
    SSL_CTX_free(STATIC_pSSLContext);
    STATIC_pSSLContext = NULL;
  }

  // free error messages
  ERR_free_strings();

#endif // #ifdef ENABLE_OPENSSL

#if (OS_FAMILY == OSF_WINDOWS)

  // cleanup WSA
  if (WSACleanup() == __IPC__IPV4__CSocket__SOCKET_ERROR) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Shutdown,
    MESSAGE("FATAL ERROR: unable to cleanup Windows Socket API"));
    THROW(ESocket, UNABLE_SHUTDOWN_MODULE);
  }

#endif // (OS_FAMILY == OSF_WINDOWS)

} // Shutdown


/////////////////////////////////////////////////////////////////////////////
T_IPADDRESS CSocket::ResolveHost(const T_STRING & sHost) {
  struct hostent * pHost = NULL;

  pHost = gethostbyname(C_STR(sHost));
  if (pHost == NULL) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, ResolveHost,
    MESSAGE("WARNING: unable to resolve host (name=\"%s\")", C_STR(sHost)));
    THROW(ESocket, UNABLE_RESOLVE);
  }

  return (T_IPADDRESS(::BASETYPE::dword((const T_BYTE *)(pHost->h_addr), true)));
} // ResolveHost


/////////////////////////////////////////////////////////////////////////////
void CSocket::__construct(transmissions transmission, T_BOOLEAN bListening, T_BOOLEAN bClose, descriptor_t tDescriptor, const T_IPADDRESS & tIPAddress, T_ULONG uPort) {
  __T_INT iValue = 0;
  m_transmission = transmission;
  m_bListening = bListening;
  m_bClose = bClose;
  m_tDescriptor = tDescriptor;

#ifdef ENABLE_OPENSSL

  m_pSSLDescriptor = NULL;

#endif // #ifdef ENABLE_OPENSSL

  if ((tIPAddress.IsValid() == true) && (uPort != 0)) {
    m_tAddress.sin_family = AF_INET;
    if (m_bListening == true) {
      m_tAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
      m_tAddress.sin_addr.s_addr = htonl((__T_UINT32)tIPAddress);
    }
    m_tAddress.sin_port = htons(uPort);
    memset(m_tAddress.sin_zero, '\0', 8); 

    if (m_tDescriptor == __IPC__IPV4__CSocket__INVALID_DESCRIPTOR) {
      // set socket options
      switch (m_transmission) {
      case STREAM :
      case STREAM_SSL : {
        // create a new socket
        m_tDescriptor = socket(PF_INET, SOCK_STREAM, 0);
        if (m_tDescriptor == __IPC__IPV4__CSocket__INVALID_DESCRIPTOR) {
          EXCEPTION(IPC, ::IPC::IPV4::CSocket, __construct,
          MESSAGE("FATAL ERROR: unable to create a new socket"));
          THROW(ESocket, UNABLE_CREATE);
        }
    
        // enable keep alive
        iValue = 1;

#if (OS_FAMILY == OSF_WINDOWS)

        if (setsockopt(m_tDescriptor, SOL_SOCKET, SO_KEEPALIVE, (const char *)&iValue, sizeof(int)) == __IPC__IPV4__CSocket__SOCKET_ERROR) {

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

        if (setsockopt(m_tDescriptor, SOL_SOCKET, SO_KEEPALIVE, (const void *)&iValue, sizeof(int)) == __IPC__IPV4__CSocket__SOCKET_ERROR) {

#else
#error unsupported operating system
#endif

          EXCEPTION(IPC, ::IPC::IPV4::CSocket, __construct,
          MESSAGE("FATAL ERROR: unable to enable keep alive"));
          THROW(ESocket, UNABLE_CREATE);
        }

        // set receive buffer size
        iValue = __IPC__IPV4__CSocket__RECEIVE_BUFFER_SIZE;

#if (OS_FAMILY == OSF_WINDOWS)

        if (setsockopt(m_tDescriptor, SOL_SOCKET, SO_RCVBUF, (const char *)&iValue, sizeof(int)) == __IPC__IPV4__CSocket__SOCKET_ERROR) {

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

        if (setsockopt(m_tDescriptor, SOL_SOCKET, SO_RCVBUF, (const void *)&iValue, sizeof(int)) == __IPC__IPV4__CSocket__SOCKET_ERROR) {

#else
#error unsupported operating system
#endif

          EXCEPTION(IPC, ::IPC::IPV4::CSocket, __construct,
          MESSAGE("FATAL ERROR: unable to set receive buffer size"));
          THROW(ESocket, UNABLE_CREATE);
        }

        // set send buffer size
        iValue = __IPC__IPV4__CSocket__SEND_BUFFER_SIZE;

#if (OS_FAMILY == OSF_WINDOWS)

        if (setsockopt(m_tDescriptor, SOL_SOCKET, SO_SNDBUF, (const char *)&iValue, sizeof(int)) == __IPC__IPV4__CSocket__SOCKET_ERROR) {

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

        if (setsockopt(m_tDescriptor, SOL_SOCKET, SO_SNDBUF, (const void *)&iValue, sizeof(int)) == __IPC__IPV4__CSocket__SOCKET_ERROR) {

#else
#error unsupported operating system
#endif

          EXCEPTION(IPC, ::IPC::IPV4::CSocket, __construct,
          MESSAGE("FATAL ERROR: unable to set send buffer size"));
          THROW(ESocket, UNABLE_CREATE);
        }

#ifdef ENABLE_OPENSSL

        if (m_transmission == STREAM_SSL) {
          // create a new SSL descriptor
          m_pSSLDescriptor = SSL_new(STATIC_pSSLContext);
          if (m_pSSLDescriptor == NULL) {
            EXCEPTION(IPC, ::IPC::IPV4::CSocket, __construct,
            MESSAGE("FATAL ERROR: unable to create descriptor"));
            THROW(ESocket, UNABLE_CREATE);
          }

          // set file descriptor
          SSL_set_fd(m_pSSLDescriptor, (__T_INT)m_tDescriptor);
        }

#endif // #ifdef ENABLE_OPENSSL

        break;
      }
      case DATAGRAM : {
        // create a new socket
        m_tDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
        if (m_tDescriptor == __IPC__IPV4__CSocket__INVALID_DESCRIPTOR) {
          EXCEPTION(IPC, ::IPC::IPV4::CSocket, __construct,
          MESSAGE("FATAL ERROR: unable to create a new socket"));
          THROW(ESocket, UNABLE_CREATE);
        }
        break;
      }
      }
    } else {
      // enable broadcast
      iValue = 1;

#if (OS_FAMILY == OSF_WINDOWS)

      if (setsockopt(m_tDescriptor, SOL_SOCKET, SO_BROADCAST, (const char *)&iValue, sizeof(int)) == __IPC__IPV4__CSocket__SOCKET_ERROR) {

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

      if (setsockopt(m_tDescriptor, SOL_SOCKET, SO_BROADCAST, (const void *)&iValue, sizeof(int)) == __IPC__IPV4__CSocket__SOCKET_ERROR) {

#else
#error unsupported operating system
#endif

        EXCEPTION(IPC, ::IPC::IPV4::CSocket, __construct,
        MESSAGE("FATAL ERROR: unable to enable broadcast"));
        THROW(ESocket, UNABLE_CREATE);
      }
    }
  }
} // __construct


/////////////////////////////////////////////////////////////////////////////
void CSocket::__destruct() {

#ifdef ENABLE_OPENSSL

  if (m_pSSLDescriptor != NULL) {
    // shutdown SSL connection
    SSL_shutdown(m_pSSLDescriptor);

    // release SSL descriptor
    SSL_free(m_pSSLDescriptor);
    m_pSSLDescriptor = NULL;
  }

#endif // #ifdef ENABLE_OPENSSL

  if ((m_tDescriptor != __IPC__IPV4__CSocket__INVALID_DESCRIPTOR) && (m_bClose == true)) {

#if (OS_FAMILY == OSF_WINDOWS)

    // close socket
    closesocket((__T_INT)m_tDescriptor);

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))


    // close socket
    close((__T_INT)m_tDescriptor);

#else
#error unsupported operating system
#endif

  }
  m_bListening = false;
  m_bClose = true;
  m_tDescriptor = __IPC__IPV4__CSocket__INVALID_DESCRIPTOR;
} // __destruct


/////////////////////////////////////////////////////////////////////////////
void CSocket::STATIC_Wait(descriptor_t tDescriptor, ::BASE::IObject::operations operation, const T_TIME & tTimeOut) {
  if (tTimeOut.IsValid() == true) {
    fd_set tSet;
    struct timeval tTime;
    __T_INT iResult;

    // set timeout
    tTime.tv_sec = tTimeOut.GetDelaySec();
    tTime.tv_usec = tTimeOut.GetDelayUSec();

    // initialize descriptor set
    FD_ZERO(&tSet);
    FD_SET(tDescriptor, &tSet);

    // waiting for operation
    switch (operation) {
    case ::BASE::IObject::READ : {
      iResult = select((__T_INT)tDescriptor + 1, &tSet, NULL, NULL, &tTime);
      break;            
    }
    case ::BASE::IObject::WRITE : {
      iResult = select((__T_INT)tDescriptor + 1, NULL, &tSet, NULL, &tTime);
      break;
    }
    }

    if (iResult <= 0) {
      EXCEPTION(IPC, ::IPC::IPV4::CSocket, STATIC_Wait,
      MESSAGE("WARNING: waiting is timed out"));
      THROW(ESocket, TIMED_OUT);
    }
  }
} // STATIC_Wait


/////////////////////////////////////////////////////////////////////////////
CSocket::CSocket(transmissions transmission, T_BOOLEAN bClose, descriptor_t tDescriptor) {
  __construct(transmission, false, bClose, tDescriptor);
} // CSocket


/////////////////////////////////////////////////////////////////////////////
CSocket::~CSocket() {
  __destruct();
} // ~CSocket


/////////////////////////////////////////////////////////////////////////////
CSocket::CSocket(const CSocket & tSocket) :
  ::BASE::CObject(tSocket) {
  __construct(tSocket.m_transmission, false);
} // CSocket


/////////////////////////////////////////////////////////////////////////////
CSocket & CSocket::operator=(const CSocket & tSocket) {
  __destruct();
  ::BASE::CObject::operator=(tSocket);
  __construct(tSocket.m_transmission, false);
  return (* this);
} // operator=


/////////////////////////////////////////////////////////////////////////////
void CSocket::Listen(const T_IPADDRESS & tIPAddress, T_ULONG uPort, T_UINT uWaitingQueueSize) {
  // close socket
  Close ();

  // construct socket
  __construct(m_transmission, true, true, __IPC__IPV4__CSocket__INVALID_DESCRIPTOR, tIPAddress, uPort);

  // bind address
  if (bind(m_tDescriptor, (struct sockaddr *)&m_tAddress, sizeof(struct sockaddr_in)) == __IPC__IPV4__CSocket__SOCKET_ERROR) {
    EXCEPTION(IPC, ::IPC::IPV4::CSocket, Listen,
    MESSAGE("FATAL ERROR: unable to bind address"));
    THROW(ESocket, UNABLE_BIND_ADDRESS);
  }

  switch (m_transmission) {
  case STREAM :
  case STREAM_SSL : {
    // listen to the specified address
    if (listen(m_tDescriptor, uWaitingQueueSize) == __IPC__IPV4__CSocket__SOCKET_ERROR) {
      EXCEPTION(IPC, ::IPC::IPV4::CSocket, Listen,
      MESSAGE("FATAL ERROR: unable to listen"));
      THROW(ESocket, UNABLE_LISTEN);
    }
    break;
  }
  case DATAGRAM : {
    break;
  }
  }
} // Listen


/////////////////////////////////////////////////////////////////////////////
// accept a new connection request
REFERENCE<CSocket> CSocket::Accept(const T_TIME & tTimeOut) {
  REFERENCE<CSocket> tResult;

  switch (m_transmission) {
  case STREAM :
  case STREAM_SSL : {
    descriptor_t tDescriptor;

    // waiting from READ operation
    STATIC_Wait(m_tDescriptor, ::BASE::IObject::READ, tTimeOut);

    // accept a new connection request
    tDescriptor = accept(m_tDescriptor, NULL, NULL);
    if (tDescriptor == __IPC__IPV4__CSocket__INVALID_DESCRIPTOR) {
      EXCEPTION(IPC, ::IPC::IPV4::CSocket, Accept,
      MESSAGE("FATAL ERROR: unable to accept"));
      THROW(ESocket, UNABLE_ACCEPT);
    }

    // create a new socket
    tResult.Create(new CSocket(m_transmission, true, tDescriptor));

#ifdef ENABLE_OPENSSL

    // negotiate
    if (m_transmission == STREAM_SSL) {
      __T_INT iResult = 0;

      iResult = SSL_accept(m_pSSLDescriptor);
      if (iResult == -1) {
        EXCEPTION(IPC, ::IPC::IPV4::CSocket, Accept,
        MESSAGE("ERROR: negotiate failed"));
        THROW(ESocket, UNABLE_ACCEPT);
      }  
    }

#endif // #ifdef ENABLE_OPENSSL

    break;
  }
  case DATAGRAM :
  {
    // create a new socket
    tResult.Create(new CSocket(m_transmission, false, m_tDescriptor));
    break;
  }
  }

  return (tResult);
} // Accept


/////////////////////////////////////////////////////////////////////////////
// connect to the specified address
void CSocket::Connect(const T_IPADDRESS & tIPAddress, T_ULONG uPort, const T_TIME & tConnectTimeOut, const T_TIME & tReConnectTimeOut) {
  switch (m_transmission) {
  case STREAM : 
  case STREAM_SSL : {
    __destruct();
    break;
  }
  }

  // construct socket
  __construct(m_transmission, false, true, __IPC__IPV4__CSocket__INVALID_DESCRIPTOR, tIPAddress, uPort);

  switch (m_transmission) {
  case STREAM : 
  case STREAM_SSL : {
    T_TIME tTimeStamp = T_TIME::STATIC_GetCurrent();

    // connect to the specified address
    while (connect(m_tDescriptor, (struct sockaddr *)&m_tAddress, sizeof(struct sockaddr_in)) == __IPC__IPV4__CSocket__SOCKET_ERROR) {
      if (tTimeStamp.IsTimedOut(tConnectTimeOut) == true) {
        EXCEPTION(IPC, ::IPC::IPV4::CSocket, Connect,
        MESSAGE("FATAL ERROR: unable to connect"));
        THROW(ESocket, UNABLE_CONNECT);
      } else {
        // waiting for reconnection
        ::BASE::CObject::Wait(tReConnectTimeOut);
      }
    }
  
#ifdef ENABLE_OPENSSL

    // negotiate
    if (m_transmission == STREAM_SSL) {
      __T_INT iResult = 0;

      iResult = SSL_connect(m_pSSLDescriptor);
      if (iResult == -1) {
        EXCEPTION(IPC, ::IPC::IPV4::CSocket, Connect,
        MESSAGE("ERROR: negotiate failed"));
        THROW(ESocket, UNABLE_CONNECT);
      }  
    }

#endif // #ifdef ENABLE_OPENSSL

    break;
  }
  case DATAGRAM :
  {
    break;
  }
  }
} // Connect


/////////////////////////////////////////////////////////////////////////////
// close
void CSocket::Close() {
  __destruct();
} // Close


/////////////////////////////////////////////////////////////////////////////
// receive byte stream
T_ULONG CSocket::Receive(T_BYTE * pBuffer, T_ULONG uLength, const T_TIME & tTimeOut, ::BASE::IObject::modes mode) {
  T_ULONG uReceived = 0;
  T_ULONG uLeft = uLength;

  while (uLeft > 0) {
    __T_INT iResult = 0;

    // waiting from READ operation
    STATIC_Wait(m_tDescriptor, ::BASE::IObject::READ, tTimeOut);

    // receive byte stream
    switch (m_transmission) {
    case STREAM : {

#if (OS_FAMILY == OSF_WINDOWS)

      iResult = recv(m_tDescriptor, (__T_CHAR *)pBuffer, (__T_INT)uLeft, 0);

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

      iResult = recv(m_tDescriptor, (void *)pBuffer, (__T_INT)uLeft, 0);

#else
#error unsupported operating system
#endif

      break;
    }

#ifdef ENABLE_OPENSSL

    case STREAM_SSL : {
      iResult = SSL_read(m_pSSLDescriptor, (void *)pBuffer, (__T_INT)uLeft);
      if (iResult <= 0) {
        iResult = __IPC__IPV4__CSocket__SOCKET_ERROR;
      }
      break;
    }

#endif // #ifdef ENABLE_OPENSSL

    case DATAGRAM : {

#if (OS_FAMILY == OSF_WINDOWS)

      iResult = recvfrom(m_tDescriptor, (__T_CHAR *)pBuffer, (__T_INT)uLeft, 0, NULL, NULL);

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

      iResult = recvfrom(m_tDescriptor, (void *)pBuffer, (__T_INT)uLeft, 0, NULL, NULL);

#else
#error unsupported operating system
#endif

      break;
    }
    }

    if (iResult == 0) {
      EXCEPTION(IPC, ::IPC::IPV4::CSocket, Receive,
      MESSAGE("ERROR: connection closed"));
      THROW(ESocket, CONNECTION_CLOSED);
    }
    if (iResult == __IPC__IPV4__CSocket__SOCKET_ERROR) {
      EXCEPTION(IPC, ::IPC::IPV4::CSocket, Receive,
      MESSAGE("FATAL ERROR: unable to receive"));
      THROW(ESocket, UNABLE_RECEIVE);
    }

    uReceived = uReceived + (T_ULONG)iResult;
    switch (mode) {
    case ::BASE::IObject::BLOCKED : {
      uLeft = uLeft - (T_ULONG)iResult;
      break;
    }
    case ::BASE::IObject::NON_BLOCKED : {
      uLeft = 0;
      break;
    }
    }
  }
  
  return (uReceived);
} // Receive


/////////////////////////////////////////////////////////////////////////////
// send byte stream
T_ULONG CSocket::Send(const T_BYTE * pBuffer, T_ULONG uLength, const T_TIME & tTimeOut, ::BASE::IObject::modes mode) {
  T_ULONG uSent = 0;
  T_ULONG uLeft = uLength;

  while (uLeft > 0) {
    __T_INT iResult = 0;

    // waiting from READ operation
    STATIC_Wait(m_tDescriptor, ::BASE::IObject::WRITE, tTimeOut);

    // send byte stream
    switch (m_transmission) {
    case STREAM : {

#if (OS_FAMILY == OSF_WINDOWS)

      iResult = send(m_tDescriptor, (__T_CHAR *)pBuffer, (__T_INT)uLeft, 0);

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

      iResult = send(m_tDescriptor, (void *)pBuffer, (__T_INT)uLeft, 0);

#else
#error unsupported operating system
#endif

      break;
    }

#ifdef ENABLE_OPENSSL

    case STREAM_SSL : {
      iResult = SSL_write(m_pSSLDescriptor, (const void *)pBuffer, (__T_INT)uLeft);
      if (iResult <= 0) {
        iResult = __IPC__IPV4__CSocket__SOCKET_ERROR;
      }
      break;
    }

#endif // #ifdef ENABLE_OPENSSL

    case DATAGRAM : {

#if (OS_FAMILY == OSF_WINDOWS)

      iResult = sendto(m_tDescriptor, (__T_CHAR *)pBuffer, (__T_INT)uLeft, 0, (struct sockaddr *)&m_tAddress, sizeof(struct sockaddr_in));

#elif ((OS_FAMILY == OSF_LINUX) | (OS_FAMILY == OSF_UNIX))

      iResult = sendto(m_tDescriptor, (void *)pBuffer, (__T_INT)uLeft, 0, (struct sockaddr *)&m_tAddress, sizeof(struct sockaddr_in));

#else
#error unsupported operating system
#endif

      break;
    }
    }
  
    if (iResult == 0) {
      EXCEPTION(IPC, ::IPC::IPV4::CSocket, Send,
      MESSAGE("ERROR: connection closed"));
      THROW(ESocket, CONNECTION_CLOSED);
    }
    if (iResult == __IPC__IPV4__CSocket__SOCKET_ERROR) {
      EXCEPTION(IPC, ::IPC::IPV4::CSocket, Send,
      MESSAGE("FATAL ERROR: unable to send"));
      THROW(ESocket, UNABLE_SEND);
    }

    uSent = uSent + (T_ULONG)iResult;
    switch (mode) {
    case ::BASE::IObject::BLOCKED : {
      uLeft = uLeft - (T_ULONG)iResult;
      break;
    }
    case ::BASE::IObject::NON_BLOCKED : {
      uLeft = 0;
      break;
    }
    }
  }
  
  return (uSent);
} // Send

} // namespace IPV4

} // namespace IPC
