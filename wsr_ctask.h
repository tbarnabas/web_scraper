
/////////////////////////////////////////////////////////////////////////////
//
// wsr_ctask.h - ::WSR::CTask class header
// --------------------------------------------------------------------------
//
// WEB SCRAPER
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WSR_CTASK
#define _WSR_CTASK

#include "wsr_configuration.h"

namespace WSR {

DERIVE_ABSTRACT_EXCEPTION(::BASE::EObject, ETask);
  
class WSR_EXPORT_IMPORT CTask :
  public ::BASE::CObject {
public:
  MEMBER_GET_SET(T_STRING, Address);
  MEMBER_GET_SET(T_ULONG, Depth);
  
public:
  //! constructor
  CTask(const T_STRING & sAddress, T_ULONG uDepth);
  //! destructor
  virtual ~CTask();

  //! copy constructor
  CTask(const CTask & tTask);
  //! assignment operator
  CTask & operator=(const CTask & tTask);
}; // class WSR_EXPORT_IMPORT CTask

} // namespace WSR

#endif // #ifndef _WSR_CTASK
