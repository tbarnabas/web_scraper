
/////////////////////////////////////////////////////////////////////////////
//
// wsr_creader.h - ::WSR::CReader class header
// --------------------------------------------------------------------------
//
// WEB SCRAPER
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WSR_CREADER
#define _WSR_CREADER

#include "wsr_configuration.h"

#include "wsr_ctask.h"

namespace WSR {

DERIVE_ABSTRACT_EXCEPTION(::WORKFLOW::EWorker, EReader);
  
class WSR_EXPORT_IMPORT CReader :
  public ::WORKFLOW::CWorker< ::WSR::CTask, ::WSR::CTask> {
protected:
  //! prepare
  virtual T_BOOLEAN Prepare();

  public:
  //! constructor
  CReader();
  //! destructor
  virtual ~CReader();

  //! copy constructor
  CReader(const CReader & tReader);
  //! assignment operator
  CReader & operator=(const CReader & tReader);
}; // class WSR_EXPORT_IMPORT CReader

} // namespace WSR

#endif // #ifndef _WSR_CREADER
