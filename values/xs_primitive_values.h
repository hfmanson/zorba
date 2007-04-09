/* -*- mode: c++; indent-tabs-mode: nil -*-
 *
 *  $Id: xs_primitiveValues.h,v 1.1 2006/10/09 07:07:59 Paul Pedersen Exp $
 *
 *	Copyright 2006-2007 FLWOR Foundation.
 *
 *	Author: Paul Pedersen (pcp071098@yahoo.com)
 */

#ifndef XQP_XS_PRIMITIVE_VALUES_H
#define XQP_XS_PRIMITIVE_VALUES_H

#include "values.h"

#include <time.h>
#include <ctime>
#include <string>
#include <vector>

#include "../types/representations.h"
#include "../types/sequence_type.h"
#include "../util/xqp_exception.h"

namespace xqp {

class xs_anyURIValue : public atomic_value
{
protected:
	xqp_anyURI val;

public:
	xs_anyURIValue(xqp_anyURI const&);
	xs_anyURIValue();
	~xs_anyURIValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_base64BinaryValue : public atomic_value
{
protected:
	xqp_base64Binary val;

public:
	xs_base64BinaryValue(xqp_base64Binary const&);
	xs_base64BinaryValue();
	~xs_base64BinaryValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_booleanValue : public atomic_value
{
protected:
	xqp_boolean m_val;
	
public:
	void* operator new(size_t n,itemstore& istore)
		{ return istore.alloc(n); }
	void* operator new(size_t n,itemstore& istore, off_t offset)
		{ return &istore[offset]; }
	void* operator new(size_t n, void * p)
		{ return p; }
	void* operator new(size_t n)
		{ return new char[n]; }

	bool val() const { return (bool)m_val; }

	xs_booleanValue()
		: atomic_value(xs_boolean,sizeof(xs_booleanValue)) { }
	xs_booleanValue(xqp_boolean const& val)
		: atomic_value(xs_boolean,sizeof(xs_booleanValue)), m_val(val) { }
	
private:
	~xs_booleanValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_decimalValue : public atomic_value
{
protected:
	xqp_decimal val;

public:
	xs_decimalValue(xqp_decimal const&);
	xs_decimalValue();
	~xs_decimalValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_integerValue : public atomic_value
{
protected:
	xqp_integer val;

public:
	xs_integerValue(xqp_integer const&);
	xs_integerValue();
	~xs_integerValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_longValue : public atomic_value
{
protected:
	xqp_long m_val;

public:
	void* operator new(size_t n,itemstore& istore)
		{ return istore.alloc(n); }
	void* operator new(size_t n,itemstore& istore, off_t offset)
		{ return &istore[offset]; }
	void* operator new(size_t n, void * p)
		{ return p; }
	void* operator new(size_t n)
		{ return new char[n]; }

	long val() const { return (long)m_val; }

	xs_longValue()
		: atomic_value(xs_long,sizeof(xs_longValue)) { }
	xs_longValue( xqp_int const& val)
		: atomic_value(xs_long,sizeof(xs_longValue)), m_val(val) { }
	
private:
	~xs_longValue() {}

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_intValue : public atomic_value
{
protected:
	xqp_int m_val;

public:
	void* operator new(size_t n,itemstore& istore)
		{ return istore.alloc(n); }
	void* operator new(size_t n,itemstore& istore, off_t offset)
		{ return &istore[offset]; }
	void* operator new(size_t n, void * p)
		{ return p; }
	void* operator new(size_t n)
		{ return new char[n]; }

	int val() const { return (int)m_val; }

	xs_intValue()
		: atomic_value(xs_int,sizeof(xs_intValue)) { }
	xs_intValue( xqp_int const& val)
		: atomic_value(xs_int,sizeof(xs_intValue)), m_val(val) { }
	
private:
	~xs_intValue() {}

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_shortValue : public atomic_value
{
protected:
	xqp_short val;

public:
	xs_shortValue(xqp_short const&);
	xs_shortValue();
	~xs_shortValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_byteValue : public atomic_value
{
protected:
	xqp_byte val;

public:
	xs_byteValue(xqp_byte const&);
	xs_byteValue();
	~xs_byteValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_dateValue : public atomic_value
{
protected:
	xqp_date val;

public:
	xs_dateValue(xqp_date const&);
	xs_dateValue();
	~xs_dateValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_dateTimeValue : public atomic_value
{
protected:
	xqp_dateTime val;

public:
	xs_dateTimeValue(xqp_dateTime const&);
	xs_dateTimeValue();
	~xs_dateTimeValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_doubleValue : public atomic_value
{
protected:
	xqp_double m_val;
	
public:
	void* operator new(size_t n,itemstore& istore)
		{ return istore.alloc(n); }
	void* operator new(size_t n,itemstore& istore, off_t offset)
		{ return &istore[offset]; }
	void* operator new(size_t n, void * p)
		{ return p; }
	void* operator new(size_t n)
		{ return new char[n]; }

	double val() const { return (double)m_val; }

	xs_doubleValue()
		: atomic_value(xs_double,sizeof(xs_doubleValue)) { }
	xs_doubleValue(xqp_double const& val)
		: atomic_value(xs_double,sizeof(xs_doubleValue)), m_val(val) { }
	
private:
	~xs_doubleValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_durationValue : public atomic_value
{
protected:
	xqp_duration val;

public:
	xs_durationValue(xqp_duration const&);
	xs_durationValue();
	~xs_durationValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_ENTITIESValue : public atomic_value
{
protected:
	xqp_ENTITIES val;

public:
	xs_ENTITIESValue(xqp_ENTITIES const&);
	xs_ENTITIESValue();
	~xs_ENTITIESValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_ENTITYValue : public atomic_value
{
protected:
	xqp_ENTITY val;

public:
	xs_ENTITYValue(xqp_ENTITY const&);
	xs_ENTITYValue();
	~xs_ENTITYValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_floatValue : public atomic_value
{
protected:
	xqp_float val;

public:
	xs_floatValue(xqp_float const&);
	xs_floatValue();
	~xs_floatValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_gDayValue : public atomic_value
{
protected:
	xqp_gDay val;

public:
	xs_gDayValue(xqp_gDay const&);
	xs_gDayValue();
	~xs_gDayValue();

public:
public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_gMonthValue : public atomic_value
{
protected:
	xqp_gMonth val;

public:
	xs_gMonthValue(xqp_gMonth const&);
	xs_gMonthValue();
	~xs_gMonthValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_gMonthDayValue : public atomic_value
{
protected:
	xqp_gMonthDay val;

public:
	xs_gMonthDayValue(xqp_gMonthDay const&);
	xs_gMonthDayValue();
	~xs_gMonthDayValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_gYearValue : public atomic_value
{
protected:
	xqp_gYear val;

public:
	xs_gYearValue(xqp_gYear const&);
	xs_gYearValue();
	~xs_gYearValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_gYearMonthValue : public atomic_value
{
protected:
	xqp_gYearMonth val;

public:
	xs_gYearMonthValue(xqp_gYearMonth const&);
	xs_gYearMonthValue();
	~xs_gYearMonthValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_hexBinaryValue : public atomic_value
{
protected:
	xqp_hexBinary val;

public:
	xs_hexBinaryValue(xqp_hexBinary const&);
	xs_hexBinaryValue();
	~xs_hexBinaryValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_IDValue : public atomic_value
{
protected:
	xqp_ID val;

public:
	xs_IDValue(xqp_ID const&);
	xs_IDValue();
	~xs_IDValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_IDREFValue : public atomic_value
{
protected:
	xqp_IDREF val;

public:
	xs_IDREFValue(xqp_IDREF const&);
	xs_IDREFValue();
	~xs_IDREFValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_IDREFSValue : public atomic_value
{
protected:
	xqp_IDREFS val;

public:
	xs_IDREFSValue(xqp_IDREFS const&);
	xs_IDREFSValue();
	~xs_IDREFSValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_languageValue : public atomic_value
{
protected:
	xqp_language val;

public:
	xs_languageValue(xqp_language const&);
	xs_languageValue();
	~xs_languageValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_NCNameValue : public atomic_value
{
protected:
	xqp_NCName val;

public:
	xs_NCNameValue(xqp_NCName const&);
	xs_NCNameValue();
	~xs_NCNameValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_NMTOKENValue : public atomic_value
{
protected:
	xqp_NMTOKEN val;

public:
	xs_NMTOKENValue(xqp_NMTOKEN const&);
	xs_NMTOKENValue();
	~xs_NMTOKENValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_NMTOKENSValue : public atomic_value
{
protected:
	xqp_NMTOKENS val;

public:
	xs_NMTOKENSValue(xqp_NMTOKENS const&);
	xs_NMTOKENSValue();
	~xs_NMTOKENSValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_NOTATIONValue : public atomic_value
{
protected:
	xqp_NOTATION val;

public:
	xs_NOTATIONValue(xqp_NOTATION const&);
	xs_NOTATIONValue();
	~xs_NOTATIONValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_NameValue : public atomic_value
{
protected:
	xqp_Name val;

public:
	xs_NameValue(xqp_Name const&);
	xs_NameValue();
	~xs_NameValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_negativeIntegerValue : public atomic_value
{
protected:
	xqp_negativeInteger val;

public:
	xs_negativeIntegerValue(xqp_negativeInteger const&);
	xs_negativeIntegerValue();
	~xs_negativeIntegerValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_nonNegativeIntegerValue : public atomic_value
{
protected:
	xqp_nonNegativeInteger val;

public:
	xs_nonNegativeIntegerValue(xqp_nonNegativeInteger const&);
	xs_nonNegativeIntegerValue();
	~xs_nonNegativeIntegerValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_nonPositiveIntegerValue : public atomic_value
{
protected:
	xqp_nonPositiveInteger val;

public:
	xs_nonPositiveIntegerValue(xqp_nonPositiveInteger const&);
	xs_nonPositiveIntegerValue();
	~xs_nonPositiveIntegerValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_normalizedStringValue : public atomic_value
{
protected:
	xqp_normalizedString val;

public:
	xs_normalizedStringValue(xqp_normalizedString const&);
	xs_normalizedStringValue();
	~xs_normalizedStringValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_positiveIntegerValue : public atomic_value
{
protected:
	xqp_positiveInteger val;

public:
	xs_positiveIntegerValue(xqp_positiveInteger const&);
	xs_positiveIntegerValue();
	~xs_positiveIntegerValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_stringValue : public atomic_value
{
protected:
	char rest[0];

public:   // storage interface
	void* operator new(size_t n,itemstore& istore)
		{ return istore.alloc(n); }
	void* operator new(size_t n,itemstore& i, itemref_t o)
		{ return &i[o]; }
	void* operator new(size_t n, void* p)
		{ return p; }
	void* operator new(size_t n, const void* p)
		{ return (void*)p; }
	void* operator new(size_t n, size_t m)
		{ return new char[n+m+1]; }

	xs_stringValue(itemstore&, const std::string&);
	xs_stringValue(const std::string&);
	xs_stringValue() {}

private:
	~xs_stringValue() {}

public:   // accessors
	std::string str() const;
	std::ostream& put(std::ostream&) const;
	std::string describe() const;

public:   // XQuery interface
  iterator_t atomized_value() const { return NULL; }
  string string_value() const;

  bool is_empty() const { return false; }
  bool is_node() const { return false; }
  bool is_atomic() const { return true; }

};


class uri_value : public atomic_value
{
protected:
	uint64_t m_urikey;
	itemref_t m_uriref;
	char rest[0];
	/*
		char[] uri
	*/

	void* operator new(size_t, itemstore&);
	void* operator new(size_t, void*);
	void operator delete(void*) {}
	
  uri_value() {}

private:	// ctor,dtor - lock out
	uri_value(uri_value& qn) : atomic_value(xs_anyURI,0) {}
	~uri_value() {}

public:		// output,debugging
	std::ostream& put(std::ostream& os) const { return os; }
	string describe() const;

};



class xs_timeValue : public atomic_value
{
protected:
	xqp_time val;

public:
	xs_timeValue(xqp_time const&);
	xs_timeValue();
	~xs_timeValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_tokenValue : public atomic_value
{
protected:
	xqp_token val;

public:
	xs_tokenValue(xqp_token const&);
	xs_tokenValue();
	~xs_tokenValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_unsignedByteValue : public atomic_value
{
protected:
	xqp_unsignedByte val;

public:
	xs_unsignedByteValue(xqp_unsignedByte const&);
	xs_unsignedByteValue();
	~xs_unsignedByteValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_unsignedIntValue : public atomic_value
{
protected:
	xqp_unsignedInt val;

public:
	xs_unsignedIntValue(xqp_unsignedInt const&);
	xs_unsignedIntValue();
	~xs_unsignedIntValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_unsignedLongValue : public atomic_value
{
protected:
	xqp_unsignedLong val;

public:
	xs_unsignedLongValue(xqp_unsignedLong const&);
	xs_unsignedLongValue();
	~xs_unsignedLongValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


class xs_unsignedShortValue : public atomic_value
{
protected:
	xqp_unsignedShort val;

public:
	xs_unsignedShortValue(xqp_unsignedShort const&);
	xs_unsignedShortValue();
	~xs_unsignedShortValue();

public:
	std::string describe() const;
	std::string string_value() const;
	std::ostream& put(std::ostream&) const;

};


}	/* namespace xqp */
#endif	/* XQP_XS_PRIMITIVE_VALUES_H */
