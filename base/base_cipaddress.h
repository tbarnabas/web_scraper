
/////////////////////////////////////////////////////////////////////////////
//
// base_cipaddress.h - ::BASE::CIPAddress class header
// --------------------------------------------------------------------------
//
// BASE
//
/////////////////////////////////////////////////////////////////////////////

// PLATFORM DEPENDENT

#pragma once
#ifndef _BASE_CIPADDRESS
#define _BASE_CIPADDRESS

#include "base_configuration.h"

#include "base_cstring.h"

namespace BASE {

class BASE_EXPORT_IMPORT CIPAddress {
private:
  //! seconds
  T_UINT32 m_uIPAddress;

  //! construct
  void __construct(const T_STRING & sIPAddress);

public:
  //! constructor
  CIPAddress(T_UINT32 uIPAddress = 0);
  //! constructor
  CIPAddress(const T_STRING & sIPAddress);
  //! destructor
  virtual ~CIPAddress();

  //! copy constructor
  CIPAddress(const CIPAddress & tIPAddress);
  //! assignment operator
  CIPAddress & operator=(const CIPAddress & tIPAddress);
  //! assignment operator
  CIPAddress & operator=(T_UINT32 uIPAddress);
  //! assignment operator
  CIPAddress & operator=(const T_STRING & sIPAddress);

  //! type cast operator
  operator T_UINT32 () const;
  //! type cast operator
  operator T_STRING () const;

  //! if ipaddress is valid then return true
  T_BOOLEAN IsValid() const;
}; // class BASE_EXPORT_IMPORT CIPAddress

} // namespace BASE

#endif // #ifndef _BASE_CIPADDRESS
