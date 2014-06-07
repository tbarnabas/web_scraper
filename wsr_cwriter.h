
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

DERIVE_EXCEPTION_BEGIN(::THREAD::ELoopThread, EWriter)
  UNABLE_TO_OPEN
DERIVE_EXCEPTION_END(EWriter);
  
class WSR_EXPORT_IMPORT CWriter :
  public ::THREAD::CLoopThread {
private:
  MEMBER(::std::ofstream, Output);
  MEMBER__REFERENCE(::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> >, Emails);

  //! copy constructor
  CWriter(const CWriter & tWriter);
  //! assignment operator
  CWriter & operator=(const CWriter & tWriter);

protected:
  //! loop
  virtual void Loop();

public:
  //! constructor
  CWriter(const T_STRING & sOutput, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pEmails);
  //! destructor
  virtual ~CWriter();
}; // class WSR_EXPORT_IMPORT CWriter

} // namespace WSR

#endif // #ifndef _WSR_CWRITER
