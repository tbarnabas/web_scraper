
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_cstream.h - ::DATASTRUCTURE::CStream class header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE_CSTREAM
#define _DATASTRUCTURE_CSTREAM

#include "datastructure_configuration.h"

#include "datastructure_carray.h"
#include "datastructure_cqueue.h"

namespace DATASTRUCTURE {

DERIVE_ABSTRACT_EXCEPTION(::DATASTRUCTURE::EQueue, EStream);

class CStream :
  public ::DATASTRUCTURE::CQueue<REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > > {
public:
  //! constructor
  CStream();
  //! destructor
  virtual ~CStream();

  //! copy constructor
  CStream(const CStream & tStream);
  //! assignment operator
  CStream & operator=(const CStream & tStream);

  //! shift operator
  virtual CStream & operator<<(::DATASTRUCTURE::CArray<T_BYTE> * pArray);

  //! shift operator
  virtual CStream & operator<<(T_BOOLEAN bValue) = 0;

  //! shift operator
  virtual CStream & operator<<(T_INT8 iValue) = 0;
  //! shift operator
  virtual CStream & operator<<(T_UINT8 uValue) = 0;
  //! shift operator
  virtual CStream & operator<<(T_INT16 iValue) = 0;
  //! shift operator
  virtual CStream & operator<<(T_UINT16 uValue) = 0;
  //! shift operator
  virtual CStream & operator<<(T_INT32 iValue) = 0;
  //! shift operator
  virtual CStream & operator<<(T_UINT32 uValue) = 0;

#if ((PA_FAMILY == PAF_IA64) | (PA_FAMILY == PAF_AMD64))

  //! shift operator
  // virtual CStream & operator<<(T_INT64 iValue) = 0;
  //! shift operator
  // virtual CStream & operator<<(T_UINT64 uValue) = 0;

#endif // #if ((PA_FAMILY == PAF_IA64) | (PA_FAMILY == PAF_AMD64))

  //! shift operator
  virtual CStream & operator<<(T_FLOAT fValue) = 0;
  //! shift operator
  virtual CStream & operator<<(T_DOUBLE dValue) = 0;
  //! shift operator
  // virtual CStream & operator<<(T_LONGDOUBLE dValue) = 0;

  //! shift operator
  virtual CStream & operator<<(const T_CHAR * pValue) = 0;
  //! shift operator
  virtual CStream & operator<<(const T_STRING & sValue) = 0;
  //! shift operator
  // virtual CStream & operator<<(const T_IPADDRESS & tValue) = 0;
  //! shift operator
  // virtual CStream & operator<<(const T_TIME & tValue) = 0;

  //! write
  virtual REFERENCE<::DATASTRUCTURE::CArray<T_BYTE> > Write() = 0;
}; // class CStream

} // namespace DATASTRUCTURE

#endif // #ifndef _DATASTRUCTURE_CSTREAM
