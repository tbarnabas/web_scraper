
/////////////////////////////////////////////////////////////////////////////
//
// datastructure_ctextstream.h - ::DATASTRUCTURE::CTextStream class header
// --------------------------------------------------------------------------
//
// DATASTRUCTURE
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _DATASTRUCTURE_CTEXTSTREAM
#define _DATASTRUCTURE_CTEXTSTREAM

#include "datastructure_configuration.h"

#include "datastructure_cstream.h"

namespace DATASTRUCTURE {

DERIVE_ABSTRACT_EXCEPTION(::DATASTRUCTURE::EStream, ETextStream);

class CTextStream :
  public ::DATASTRUCTURE::CStream {
public:
  //! constructor
  CTextStream();
  //! destructor
  virtual ~CTextStream();

  //! copy constructor
  CTextStream(const CTextStream & tTextStream);
  //! assignment operator
  CTextStream & operator=(const CTextStream & tTextStream);

  //! shift operator
  virtual CTextStream & operator<<(::DATASTRUCTURE::CArray<T_BYTE> * pArray);

  //! shift operator
  virtual CTextStream & operator<<(T_BOOLEAN bValue);

  //! shift operator
  virtual CTextStream & operator<<(T_INT8 iValue);
  //! shift operator
  virtual CTextStream & operator<<(T_UINT8 uValue);
  //! shift operator
  virtual CTextStream & operator<<(T_INT16 iValue);
  //! shift operator
  virtual CTextStream & operator<<(T_UINT16 uValue);
  //! shift operator
  virtual CTextStream & operator<<(T_INT32 iValue);
  //! shift operator
  virtual CTextStream & operator<<(T_UINT32 uValue);

  //! shift operator
  virtual CTextStream & operator<<(T_FLOAT fValue);
  //! shift operator
  virtual CTextStream & operator<<(T_DOUBLE dValue);

  //! shift operator
  virtual CTextStream & operator<<(const T_CHAR * pValue);
  //! shift operator
  virtual CTextStream & operator<<(const T_STRING & sValue);

  //! write
  virtual REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > Write();
}; // class CTextStream

} // namespace DATASTRUCTURE

#endif // #ifndef _DATASTRUCTURE_CTEXTSTREAM
