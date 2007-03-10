/* -*- mode: c++; indent-tabs-mode: nil -*-
 *
 *  $Id: sequence_type.cpp,v 1.1 2006/10/09 07:07:59 Paul Pedersen Exp $
 *
 *	Copyright 2006-2007 FLWOR Foundation.
 *  Authors: John Cowan, Paul Pedersen
 *
 */

#include "sequence_type.h"

#include "../util/tracer.h"
#include "../util/xqp_exception.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
namespace xqp {

enum sequence_type::occurs_t sequence_type::get_occurs(
	TypeCode t)
{
	switch (t&ARITY_MASK) {
	case ITEM_ARITY: return occurs_once;
	case SEQUENCE_ARITY: return occurs_zero_or_more;
	case NONEMPTY_SEQ_ARITY: return occurs_one_or_more;
	case OPT_ITEM_ARITY: return occurs_zero_or_one;
	default:  {
		cout << TRACE << " : Unexpected arity bits\n";
		return occurs_once;
	}
	}
}

std::string _describe_atomictype(
	TypeCode t)
{
	switch (t&TYPE_MASK) {
	case UNTYPED_TYPE: return "xs:untyped";
	case UNTYPED_ATOMIC_TYPE: return "xs:untypedAtomic";
	case STRING_TYPE: return "xs:string";
	case DECIMAL_TYPE: return "xs:decimal";
	case BOOLEAN_TYPE: return "xs:boolean";
	case FLOAT_TYPE: return "xs:float";
	case DOUBLE_TYPE: return "xs:double";
	case DURATION_TYPE: return "xs:duration";
	case DATE_TIME_TYPE: return "xs:dateTime";
	case TIME_TYPE: return "xs:time";
	case DATE_TYPE: return "xs:date";
	case G_YEAR_MONTH_TYPE: return "xs:gYearMonth";
	case G_YEAR_TYPE: return "xs:gYear";
	case G_MONTH_DAY_TYPE: return "xs:gMonthDay";
	case G_DAY_TYPE: return "xs:gDay";
	case G_MONTH_TYPE: return "xs:gMonth";
	case HEX_BINARY_TYPE: return "xs:hexBinary";
	case BASE_64_BINARY_TYPE: return "xs:base64Binary";
	case ANY_URI_TYPE: return "xs:anyURI";
	case QNAME_TYPE: return "xs:QName";
	case NOTATION_TYPE: return "xs:NOTATION";
	case COMPLEX_TYPE: return "xs:complex_type";
  case EMPTY_TYPE: return "xdm:empty";
  case ANY_TYPE: return "xs:anyType";
  case RAW_BINARY_TYPE: return "xqp:binary";
	case NORMALIZED_STRING_SUB: return "xs:normalizedString";
	case TOKEN_SUB2: return "xs:TOKEN";
	case LANGUAGE_SUB3: return "xs:language";
	case NMTOKEN_SUB3: return "xs:NMTOKEN";
	case NAME_SUB3: return "xs:Name";
	case NCNAME_SUB4: return "xs:NCName";
	case ID_SUB5: return "xs:ID";
	case IDREF_SUB5: return "xs:IDREF";
	case ENTITY_SUB5: return "xs:ENTITY";
	case INTEGER_SUB: return "xs:integer";
	case NON_POSITIVE_INTEGER_SUB2: return "xs:nonPositiveInteger";
	case NEGATIVE_INTEGER_SUB3: return "xs:negativeInteger";
	case LONG_SUB2: return "xs:long";
	case INT_SUB3: return "xs:int";
	case SHORT_SUB4: return "xs:short";
	case BYTE_SUB5: return "xs:byte";
	case NON_NEGATIVE_INTEGER_SUB2: return "xs:nonNegativeInteger";
	case UNSIGNED_LONG_SUB3: return "xs:unsignedLong";
	case UNSIGNED_INT_SUB4: return "xs:unsignedInt";
	case UNSIGNED_SHORT_SUB5: return "xs:unsignedShort";
	case UNSIGNED_BYTE_SUB6: return "xs:unsignedByte";
	case POSITIVE_INTEGER_SUB3: return "xs:positiveInteger";
	case DAY_TIME_DURATION_SUB: return "xs:dayTimeDuration";
	case YEAR_MONTH_DURATION_SUB: return "xs:yearMonthDuration";
	default: {
  	cout << TRACE << " : Unexpected type bits: " << t << "\n";
		return "xs:untyped";
	}
	}
}

std::string _describe_nodetype(
	TypeCode t)
{
	switch (t&NODE_MASK) {
	case DOCUMENT_NODE: return "document()";
	case ELEMENT_NODE: return "element()";
	case ATTRIBUTE_NODE: return "attribute()";
	case NAMESPACE_NODE: return "namespace()";
	case PI_NODE: return "pi()";
	case COMMENT_NODE: return "comment()";
	case TEXT_NODE: return "text()";
	case ANY_NODE: return "node()";
	default: {
  	cout << TRACE << " : Unexpected node bits\n";
		return "node()";
	}
	}
}

std::string _describe_itemtype(
	TypeCode t)
{
	uint32_t node = (t&NODE_MASK);
	return (node ? _describe_nodetype(t) : _describe_atomictype(t));
}

std::string sequence_type::describe(
	TypeCode t)
{
	ostringstream oss;
	oss << _describe_itemtype(t);
	switch (get_occurs(t)) {
	case occurs_once: break;
	case occurs_zero_or_more: oss << "*"; break;
	case occurs_one_or_more: oss << "+"; break;
	case occurs_zero_or_one: oss << "?"; break;
	default: {
		cout << TRACE << " : Unexpected occurence indicator\n";
	}
	}
	return oss.str();
}

bool sequence_type::derives_from(
	TypeCode ta, TypeCode te)
{
	return false;
}

}	/* namepsace xqp */

