
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cwriter.h - ::WSR::CWriter class header
// --------------------------------------------------------------------------
//
// WEB SCRAPER
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WSR_CWRITER
#define _WSR_CWRITER

#include "wsr_configuration.h"

#include "wsr_ctask.h"

namespace WSR {

DERIVE_ABSTRACT_EXCEPTION(::WORKFLOW::EWorker, EWriter);
  
class WSR_EXPORT_IMPORT CWriter :
  public ::WORKFLOW::CWorker< ::WSR::CTask, ::WSR::CTask> {
public:
  //! constructor
  CWriter();
  //! destructor
  virtual ~CWriter();

  //! copy constructor
  CWriter(const CWriter & tWriter);
  //! assignment operator
  CWriter & operator=(const CWriter & tWriter);
}; // class WSR_EXPORT_IMPORT CWriter

} // namespace WSR

#endif // #ifndef _WSR_CWRITER
