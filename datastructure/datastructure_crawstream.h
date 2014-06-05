
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_crawstream.h - ::DATASTRUCTURE::CRawStream class header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE_CRAWSTREAM
#define _DATASTRUCTURE_CRAWSTREAM

#include "datastructure_configuration.h"

#include "datastructure_cstream.h"

namespace DATASTRUCTURE {

DERIVE_ABSTRACT_EXCEPTION(::DATASTRUCTURE::EStream, ERawStream);

class CRawStream :
  public ::DATASTRUCTURE::CStream {
public:
  //! constructor
  CRawStream();
  //! destructor
  virtual ~CRawStream();

  //! copy constructor
  CRawStream(const CRawStream & tRawStream);
  //! assignment operator
  CRawStream & operator=(const CRawStream & tRawStream);

  //! shift operator
  virtual CRawStream & operator<<(::DATASTRUCTURE::CArray<T_BYTE> * pArray);

  //! shift operator
  virtual CRawStream & operator<<(T_BOOLEAN bValue);

  //! shift operator
  virtual CRawStream & operator<<(T_INT8 iValue);
  //! shift operator
  virtual CRawStream & operator<<(T_UINT8 uValue);
  //! shift operator
  virtual CRawStream & operator<<(T_INT16 iValue);
  //! shift operator
  virtual CRawStream & operator<<(T_UINT16 uValue);
  //! shift operator
  virtual CRawStream & operator<<(T_INT32 iValue);
  //! shift operator
  virtual CRawStream & operator<<(T_UINT32 uValue);

  //! shift operator
  virtual CRawStream & operator<<(T_FLOAT fValue);
  //! shift operator
  virtual CRawStream & operator<<(T_DOUBLE dValue);

  //! shift operator
  virtual CRawStream & operator<<(const T_CHAR * pValue);
  //! shift operator
  virtual CRawStream & operator<<(const T_STRING & sValue);

  //! write
  virtual REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > Write();
}; // class CRawStream

} // namespace DATASTRUCTURE

#endif // #ifndef _DATASTRUCTURE_CRAWSTREAM
