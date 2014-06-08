
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

DERIVE_EXCEPTION_BEGIN(::BASE::ELoopThread, EReader)
  UNABLE_TO_OPEN
DERIVE_EXCEPTION_END(EReader);
  
class WSR_EXPORT_IMPORT CReader :
  public ::BASE::CLoopThread {
private:
  MEMBER(::std::ifstream, Input);
  MEMBER(T_ULONG, Depth);
  MEMBER__REFERENCE(::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> >, Domains);
  MEMBER__REFERENCE(::BASE::IObject, DomainsProducers);
  MEMBER__REFERENCE(::BASE::IObject, DomainsConsumers);

  //! copy constructor
  CReader(const CReader & tReader);
  //! assignment operator
  CReader & operator=(const CReader & tReader);

protected:
  //! loop
  virtual void Loop();

public:
  //! constructor
  CReader(const T_STRING & sInput, T_ULONG uDepth, ::DATASTRUCTURE::CQueue<REFERENCE< ::WSR::CTask> > * pDomains, ::BASE::IObject * DomainsProducers, ::BASE::IObject * DomainsConsumers);
  //! destructor
  virtual ~CReader();
}; // class WSR_EXPORT_IMPORT CReader

} // namespace WSR

#endif // #ifndef _WSR_CREADER
