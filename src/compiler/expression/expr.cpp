/* -*- mode: c++; indent-tabs-mode: nil; tab-width: 2 -*-
 *
 *  $Id: expr.cpp.cpp,v 1.1 2006/10/09 07:07:59 Paul Pedersen Exp $
 *
 *	Copyright 2006-2007 FLWOR Foundation.
 *
 *  Author: Paul Pedersen
 *
 */

#include "compiler/expression/expr.h"
#include "functions/function.h"
#include "compiler/parser/parse_constants.h"
#include "compiler/parsetree/parsenodes.h"
#include "util/Assert.h"
#include "util/tracer.h"
#include "errors/Error.h"
#include "compiler/expression/expr_visitor.h"
#include "util/zorba.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace xqp {
  
int printdepth0 = 0;
#define DENT		std::string(printdepth0, ' ')
#define INDENT	    std::string(++printdepth0, ' ')
#define OUTDENT	    std::string(printdepth0--, ' ')
#define UNDENT	    printdepth0--
#define PUT_QNAME( qname, os ) (os << qname->getPrefix () << "[=" << qname->getNamespace () << "]:" << qname->getLocalName ())

#define ITEM_FACTORY (Store::getInstance().getItemFactory())

#define ACCEPT( m ) do { if (m != NULL) m->accept (v); } while (0)

expr::expr(
	yy::location const& _loc)
:
	loc(_loc)
{
}

ostream& expr::put( ostream& os) const
{
	return os;
}

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//  XQuery 1.0 productions                                             //
//  [http://www.w3.org/TR/xquery/]                                     //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

// [29]
enclosed_expr::enclosed_expr(yy::location const& loc, expr_t const& content_arg)
:
	expr(loc), content(content_arg)
{}
enclosed_expr::~enclosed_expr(){}

ostream& enclosed_expr::put( ostream& os) const
{
	os << INDENT << "enclosed_expr[\n";
	content->put(os);

	return os << OUTDENT << "]\n";
}

void enclosed_expr::accept(
	expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;
	content->accept(v);
	v.end_visit(*this);
}

// [33a]

string var_expr::decode_var_kind(
	enum var_kind k)
{
	switch (k) {
	case for_var: return "FOR_VAR"; break;
	case let_var: return "LET_VAR"; break;
	case pos_var: return "POS_VAR"; break;
	case score_var: return "SCORE_VAR"; break;
	case quant_var: return "QUANT_VAR"; break;
	case extern_var: return "EXTERN_VAR"; break;
	case assign_var: return "ASSIGN_VAR"; break;
	case context_var: return "$DOT"; break;
	default: return "???";
	}
}

ostream& var_expr::put( ostream& os) const
{
	os << INDENT << "var_expr[" << decode_var_kind(get_kind());
	if (varname_h!=NULL) {
		os << " name=";
		PUT_QNAME (get_varname(), os);
	}
	os << ", type="; // TODO(VRB) << sequence_type::describe(type);
	return os << OUTDENT << "]\n";
}

void var_expr::accept(expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;
	v.end_visit(*this);
}

expr::expr_t var_expr::clone(expr::substitution_t& substitution)
{
  expr::subst_iter_t i = substitution.find(this);
  if (i == substitution.end()) {
    return this;
  }
  return i->second->clone(substitution);
}

// [33] [http://www.w3.org/TR/xquery/#prod-xquery-FLWORExpr]


forlet_clause::forlet_clause(
	enum forlet_t _type,
	varref_t _var_h,
	varref_t _pos_var_h,
	varref_t _score_var_h,
	expr_t _expr_h)
:
	type(_type),
	var_h(_var_h),
	pos_var_h(_pos_var_h),
	score_var_h(_score_var_h),
	expr_h(_expr_h)
{
}

forlet_clause::~forlet_clause()
{
}

ostream & forlet_clause::put( ostream& os) const
{
	os << INDENT << "forlet[";
	switch (type) {
	case for_clause: os << "FOR\n"; break;
	case let_clause: os << "LET\n"; break;
	default: os << "??\n";
	}
	Assert(var_h != NULL);
	var_h->put(os);
	if (pos_var_h!=NULL) {
		os << INDENT << " AT "; pos_var_h->put(os); UNDENT;
	}
	if (score_var_h!=NULL) {
		os << INDENT << " SCORE "; score_var_h->put(os); UNDENT;
	}
	Assert(expr_h != NULL);
	expr_h->put(os);
	return os << OUTDENT << "]\n";
}

rchandle<forlet_clause> forlet_clause::clone(expr::substitution_t& substitution)
{
  expr_t expr_copy_h = expr_h->clone(substitution);

  varref_t var_copy_h(new var_expr(*var_h));
  substitution[var_h.get_ptr()] = var_copy_h.get_ptr();

  varref_t pos_var_copy_h;
  var_expr *pos_var_ptr = pos_var_h.get_ptr();
  if (pos_var_ptr) {
    pos_var_copy_h = new var_expr(*pos_var_ptr);
    substitution[pos_var_ptr] = pos_var_copy_h.get_ptr();
  }

  varref_t score_var_copy_h;
  var_expr *score_var_ptr = score_var_h.get_ptr();
  if (score_var_ptr) {
    score_var_copy_h = new var_expr(*score_var_ptr);
    substitution[score_var_ptr] = score_var_copy_h.get_ptr();
  }

  return new forlet_clause(type, var_copy_h, pos_var_copy_h, score_var_copy_h, expr_copy_h);
}


flwor_expr::flwor_expr(
	yy::location const& loc)
:
	expr(loc), order_stable (false)
{
}

flwor_expr::~flwor_expr()
{
}

ostream& flwor_expr::put( ostream& os) const
{
	os << INDENT << "flwor_expr[\n";
	vector<forletref_t>::const_iterator it = clause_begin();
	for (; it!=clause_end(); ++it) {
		forletref_t fl_h = *it;
		Assert(fl_h != NULL);
		fl_h->put(os);
	}
	if (where_h!=NULL) where_h->put(os);

	vector<orderspec_t>::const_iterator ord_it = orderspec_begin();
	for (; ord_it!=orderspec_end(); ++ord_it) {
		orderspec_t spec = *ord_it;
		expr_t e_h = spec.first;
		Assert(e_h != NULL);
		orderref_t ord_h = spec.second;
		Assert(ord_h != NULL);

		os << INDENT << "ORDERBY\n";
		os << e_h->put(os) << endl;
		UNDENT;

		os << INDENT;
		switch (ord_h->dir) {
		case dir_ascending: os << "ASCENDING "; break;
		case dir_descending: os << "DESCENDING "; break;
		default: os << "?? ";
		}
		switch (ord_h->empty_mode) {
		case StaticQueryContext::empty_greatest: os << "EMPTY GREATEST "; break;
		case StaticQueryContext::empty_least: os << "EMPTY LEAST "; break;
		default: os << "?? ";
		}
		os << ord_h->collation << endl;
		UNDENT;
	}
	//d Assert<null_pointer>(retval_h!=NULL);
	Assert(retval_h!=NULL);
	os << INDENT << "RETURN\n"; retval_h->put(os); UNDENT;
	return os << OUTDENT << "]\n";

}

void flwor_expr::accept(expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;
	
	vector<forletref_t>::const_iterator it = this->clause_v.begin();
	for (; it!=this->clause_v.end(); ++it) {
		(*it)->expr_h->accept(v);
	}
	
  if (where_h != NULL)
    where_h->accept (v);
  for (vector<orderspec_t>::const_reverse_iterator i = orderspec_rbegin ();
       i != orderspec_rend ();
       i++)
    (*i).first->accept (v);
	
	this->retval_h->accept(v);
	
	v.end_visit(*this);	
}

expr::expr_t flwor_expr::clone(expr::substitution_t& substitution)
{
  expr_t flwor_copy = new flwor_expr(get_loc());
  flwor_expr *flwor_copy_ptr = static_cast<flwor_expr *>(flwor_copy.get_ptr());

  for(clause_list_t::iterator i = clause_v.begin(); i != clause_v.end(); ++i) {
    flwor_copy_ptr->add((*i)->clone(substitution));
  }

  for(orderspec_list_t::iterator i = orderspec_v.begin(); i != orderspec_v.end(); ++i) {
    expr_t e_copy = i->first->clone(substitution);
    flwor_copy_ptr->add(orderspec_t(e_copy, i->second));
  }

  flwor_copy_ptr->set_order_stable(order_stable);

  if (where_h.get_ptr()) {
    flwor_copy_ptr->set_where(where_h->clone(substitution));
  }

  flwor_copy_ptr->set_retval(retval_h->clone(substitution));

  return flwor_copy;
}

// [42] [http://www.w3.org/TR/xquery/#prod-xquery-QuantifiedExpr]

quantified_expr::quantified_expr(
	yy::location const& loc,
	enum quantification_mode_t _qmode)
:
	expr(loc),
	qmode(_qmode)
{
}

quantified_expr::~quantified_expr()
{
}

ostream& quantified_expr::put( ostream& os) const
{
	os << INDENT << "quantified_expr[";
	switch (qmode) {
	case quant_some: os << "SOME\n"; break;
	case quant_every: os << "EVERY\n"; break;
	default: os << "??\n";
	}

	vector<varref_t>::const_iterator it = var_v.begin();
	vector<varref_t>::const_iterator en = var_v.end();

	for (; it!=en; ++it) {
		varref_t var_h = *it;
		//d Assert<null_pointer>(var_h!=NULL);
		Assert(var_h!=NULL);
		//d Assert<null_pointer>(var_h->varname_h!=NULL);
		Assert(var_h->varname_h!=NULL);
		os << INDENT;
		PUT_QNAME (var_h->varname_h, os) << " in ";
		UNDENT;
	}

	os << " SATISFIES\n";
	//d Assert<null_pointer>(sat_expr_h!=NULL);
	Assert(sat_expr_h!=NULL);
	sat_expr_h->put(os);
	return os << OUTDENT << "\n]\n";
}

void quantified_expr::accept(
	expr_visitor& v)
{
}



// [43] [http://www.w3.org/TR/xquery/#prod-xquery-TypeswitchExpr]

typeswitch_expr::typeswitch_expr(
	yy::location const& loc)
:
	expr(loc)
{
}

typeswitch_expr::~typeswitch_expr()
{
}

ostream& typeswitch_expr::put( ostream& os) const
{
	os << INDENT << "typeswitch_expr[\n";
	//d Assert<null_pointer>(switch_expr_h!=NULL);
	Assert(switch_expr_h!=NULL);
	switch_expr_h->put(os);

	vector<clauseref_t>::const_iterator it = case_clause_hv.begin();
	for (; it!=case_clause_hv.end(); ++it) {
		clauseref_t cc_h = *it;
		os << INDENT << "case: ";
		if (cc_h->var_h!=NULL) cc_h->var_h->put(os) << " as ";
		// TODO(VRB) os << sequence_type::describe(cc_h->type);
        os << " return ";
		//d Assert<null_pointer>(cc_h->case_expr_h!=NULL);
		Assert(cc_h->case_expr_h!=NULL);
		cc_h->case_expr_h->put(os) << endl;
		UNDENT;
	}
	return os << OUTDENT << "]\n";
}

void typeswitch_expr::accept(
	expr_visitor& v)
{
}



// [45] [http://www.w3.org/TR/xquery/#prod-xquery-IfExpr]

if_expr::if_expr(
	yy::location const& loc,
	rchandle<expr> _cond_expr_h,
	rchandle<expr> _then_expr_h,
	rchandle<expr> _else_expr_h)
:
	expr(loc),
	cond_expr_h(_cond_expr_h),
	then_expr_h(_then_expr_h),
	else_expr_h(_else_expr_h)
{
}

if_expr::if_expr(
	yy::location const& loc)
:
	expr(loc)
{
}

if_expr::~if_expr()
{
}

ostream& if_expr::put( ostream& os) const
{
	os << INDENT << "if_expr[\n";
	//d Assert<null_pointer>(cond_expr_h!=NULL);
	Assert(cond_expr_h!=NULL);
	cond_expr_h->put(os);
	//d Assert<null_pointer>(then_expr_h!=NULL);
	Assert(then_expr_h!=NULL);
	then_expr_h->put(os);
	//d Assert<null_pointer>(else_expr_h!=NULL);
	Assert(else_expr_h!=NULL);
	else_expr_h->put(os);
	return os << OUTDENT << "\n]\n";
}

void if_expr::accept(
	expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;
	this->cond_expr_h->accept(v);
	this->then_expr_h->accept(v);
	this->else_expr_h->accept(v);
	v.end_visit(*this);
}



////////////////////////////////
//	first-order expressions
////////////////////////////////
// includes:
// [46] [http://www.w3.org/TR/xquery/#prod-xquery-OrExpr]
// [47] [http://www.w3.org/TR/xquery/#prod-xquery-AndExpr]
// [48] [http://www.w3.org/TR/xquery/#prod-xquery-ComparisonExpr]
// [49] [http://www.w3.org/TR/xquery/#prod-xquery-RangeExpr]
// [50] [http://www.w3.org/TR/xquery/#prod-xquery-AdditiveExpr]
// [51] [http://www.w3.org/TR/xquery/#prod-xquery-MultiplicativeExpr]
// [52] [http://www.w3.org/TR/xquery/#prod-xquery-UnionExpr]
// [53] [http://www.w3.org/TR/xquery/#prod-xquery-IntersectExceptExpr]


fo_expr::fo_expr(
	yy::location const& loc)
:
	expr(loc)
{
}

fo_expr::~fo_expr()
{
}

ostream& fo_expr::put( ostream& os) const
{
	os << INDENT << "fo_expr[\n";
	//d Assert<null_pointer>(func!=NULL);
	Assert(func!=NULL);
	os << INDENT << func->get_fname()->getStringProperty() << endl;

	vector<rchandle<expr> >::const_iterator it = begin();
	for (; it != end(); ++it)
  {
		rchandle<expr> e_h = *it;
		//d Assert<null_pointer>(e_h!=NULL);
		Assert(e_h!=NULL);
		e_h->put(os) << endl;
	}
	return os << OUTDENT << "]\n";
}

void fo_expr::accept(
	expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;
	vector<expr_t>::const_iterator it = begin();
	for (; it!=end(); ++it) {
		(*it)->accept(v);
	}
	v.end_visit(*this);
}



// [48a] [http://www.w3.org/TR/xquery-full-text/#prod-xquery-FTContainsExpr]

ft_contains_expr::ft_contains_expr(
	yy::location const& loc,
	rchandle<expr> _range_h,
	rchandle<expr> _ft_select_h,
	rchandle<expr> _ft_ignore_h)
:
	expr(loc),
	range_h(_range_h),
	ft_select_h(_ft_select_h),
	ft_ignore_h(_ft_ignore_h)
{
}

ft_contains_expr::~ft_contains_expr()
{
}

ostream& ft_contains_expr::put( ostream& os) const
{
	os << "ft_contains_expr[\n";
	//d Assert<null_pointer>(range_h!=NULL);
	Assert(range_h!=NULL);
	range_h->put(os) << endl;
	//d Assert<null_pointer>(ft_select_h!=NULL);
	Assert(ft_select_h!=NULL);
	os << "ft_contains\n";
	ft_select_h->put(os) << endl;
	if (ft_ignore_h!=NULL) ft_ignore_h->put(os);
	return os << "\n]\n";
}

void ft_contains_expr::accept(
	expr_visitor& v)
{
}




// [54] [http://www.w3.org/TR/xquery/#prod-xquery-InstanceofExpr]

instanceof_expr::instanceof_expr(
	yy::location const& loc,
	rchandle<expr> _expr_h,
	TypeSystem::xqtref_t _type)
:
	expr(loc),
	expr_h(_expr_h),
	type(_type)
{
}

instanceof_expr::~instanceof_expr()
{
}

ostream& instanceof_expr::put( ostream& os) const
{
	os << INDENT << "instanceof_expr[\n";
	//d Assert<null_pointer>(expr_h!=NULL);
	Assert(expr_h!=NULL);
	expr_h->put(os) << endl;
	os << "instance of\n";
	// TODO(VRB) os << sequence_type::describe(type);
	return os << OUTDENT << "]\n";
}

void instanceof_expr::accept(
	expr_visitor& v)
{
}



// [55] [http://www.w3.org/TR/xquery/#prod-xquery-TreatExpr]

treat_expr::treat_expr(
	yy::location const& loc,
	rchandle<expr> _expr_h,
	TypeSystem::xqtref_t _type)
:
	expr(loc),
	expr_h(_expr_h),
	type(_type)
{
}

treat_expr::~treat_expr()
{
}

ostream& treat_expr::put( ostream& os) const
{
	os << INDENT << "treat_expr[\n";
	//d Assert<null_pointer>(expr_h!=NULL);
	Assert(expr_h!=NULL);
	expr_h->put(os) << endl;
	os << "treat as\n";
	// TODO(VRB) os << sequence_type::describe(type);
	return os << OUTDENT << "]\n";
}

void treat_expr::accept(
	expr_visitor& v)
{
}



// [56] [http://www.w3.org/TR/xquery/#prod-xquery-CastableExpr]

castable_expr::castable_expr(
	yy::location const& loc,
	rchandle<expr> _expr_h,
	TypeSystem::xqtref_t _type)
:
	expr(loc),
	expr_h(_expr_h),
	type(_type)
{
}

castable_expr::~castable_expr()
{
}

ostream& castable_expr::put( ostream& os) const
{
	os << INDENT << "castable_expr[\n";
	//d Assert<null_pointer>(expr_h!=NULL);
	Assert(expr_h!=NULL);
	expr_h->put(os) << endl;
	os << "castable as\n";
	// TODO(VRB) os << sequence_type::describe(get_type());
	if (is_optional()) os << "?";
	return os << OUTDENT << "]\n";
}

void castable_expr::accept(
	expr_visitor& v)
{
}



// [57] [http://www.w3.org/TR/xquery/#prod-xquery-CastExpr]

cast_expr::cast_expr(
	yy::location const& loc,
	rchandle<expr> _expr_h,
	TypeSystem::xqtref_t _type)
:
	expr(loc),
	expr_h(_expr_h),
	type(_type)
{
}

cast_expr::~cast_expr()
{
}

ostream& cast_expr::put( ostream& os) const
{
	os << INDENT << "cast_expr[\n";
	//d Assert<null_pointer>(expr_h!=NULL);
	Assert(expr_h!=NULL);
	expr_h->put(os) << endl;
	os << "cast as\n";
	// TODO(VRB) os << sequence_type::describe(type);
	if (is_optional()) os << "?";
	return os << OUTDENT << "]\n";
}

void cast_expr::accept(
	expr_visitor& v)
{
}


// [63] [http://www.w3.org/TR/xquery/#prod-xquery-ValidateExpr]

validate_expr::validate_expr(
	yy::location const& loc,
	enum validation_mode_t _valmode,
	rchandle<expr> _expr_h)
:
	expr(loc),
	valmode(_valmode),
	expr_h(_expr_h)
{
}

validate_expr::~validate_expr()
{
}

ostream& validate_expr::put( ostream& os) const
{
	os << INDENT << "validate_expr[";
	switch (valmode) {
	case val_strict: os << "strict\n"; break;
	case val_lax: os << "lax\n"; break;
	default: os << "??\n";
	}
	//d Assert<null_pointer>(expr_h!=NULL);
	Assert(expr_h!=NULL);
	expr_h->put(os) << endl;
	return os << OUTDENT << "]\n";
}

void validate_expr::accept(
	expr_visitor& v)
{
}



// [65] [http://www.w3.org/TR/xquery/#prod-xquery-ExtensionExpr]

extension_expr::extension_expr(
	yy::location const& loc)
:
	expr(loc)
{
}

extension_expr::extension_expr(
	yy::location const& loc,
	expr_t _expr_h)
:
	expr(loc),
	expr_h(_expr_h)
{
}

extension_expr::~extension_expr()
{
}

ostream& extension_expr::put( ostream& os) const
{
	os << INDENT << "extension_expr[\n";
	/*
	vector<rchandle<pragma> >::const_iterator it = begin();
	for (; it!=end(); ++it) {
		os << INDENT;
		rchandle<pragma> p_h = *it;
		Assert<null_pointer>(p_h!=NULL);
		Assert<null_pointer>(p_h->name_h!=NULL);
		os << "?"; p_h->name_h->put(zorp,os);
		os << " " << p_h->content << endl;
		UNDENT;
	}
	*/

	os << INDENT;
	//d Assert<null_pointer>(pragma_h!=NULL);
	Assert(pragma_h!=NULL);
	//d Assert<null_pointer>(pragma_h->name_h!=NULL);
	Assert(pragma_h->name_h!=NULL);
	os << "?"; PUT_QNAME (pragma_h->name_h, os);
	os << " " << pragma_h->content << endl;
	UNDENT;

	//d Assert<null_pointer>(expr_h!=NULL);
	Assert(expr_h!=NULL);
	expr_h->put(os) << endl;
	return os << OUTDENT << "]\n";
}

void extension_expr::accept(
	expr_visitor& v)
{
}


/*******************************************************************************

 [69] [http://www.w3.org/TR/xquery/#prod-xquery-RelativePathExpr]

  RelativPathExpr ::= "/" | ("/" | "//")?  StepExpr (("/" | "//") StepExpr)*

********************************************************************************/
relpath_expr::relpath_expr(yy::location const& loc)
  :
  expr(loc)
{
}


relpath_expr::~relpath_expr()
{
}


ostream& relpath_expr::put( ostream& os) const
{
	os << INDENT << "relpath_expr[\n";
	list_iterator<expr_t> it = begin();
	for (; it != end(); ++it)
  {
		expr_t expr = *it;
		Assert(expr != NULL);
		expr->put(os);
	}
	return os << OUTDENT << "]\n";
}


void relpath_expr::accept(expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;

	list_iterator<expr_t> it = begin();
	for (; it != end(); ++it)
  {
		expr_t expr = *it;
		Assert(expr != NULL);
		expr->accept(v);
	}

	v.end_visit(*this);
}


/*******************************************************************************

  [71] [http://www.w3.org/TR/xquery/#prod-xquery-AxisStep]

	AxisStep ::= Axis NodeTest Predicate*

********************************************************************************/
axis_step_expr::axis_step_expr(yy::location const& loc)
  :
	expr(loc)
{
}


axis_step_expr::~axis_step_expr()
{
}


ostream& axis_step_expr::put(ostream& os) const
{
	os << INDENT << "axis_step_expr[";
	switch (theAxis)
  {
	case axis_kind_self:								os << "self::"; break;
	case axis_kind_child:								os << "child::"; break;
	case axis_kind_parent:							os << "parent::"; break;
	case axis_kind_descendant:					os << "descendant::"; break;
	case axis_kind_descendant_or_self:	os << "descendant-or-self::"; break;
	case axis_kind_ancestor:						os << "ancestor::"; break;
	case axis_kind_ancestor_or_self:		os << "ancestor-or-self::"; break;
	case axis_kind_following_sibling:		os << "following-sibling::"; break;
	case axis_kind_following:						os << "following::"; break;
	case axis_kind_preceding_sibling:		os << "preceding-sibling::"; break;
	case axis_kind_preceding:						os << "preceding::"; break;
	case axis_kind_attribute:						os << "attribute::"; break;
	default: os << "??";
	}
	os << endl;

	if (theNodeTest != NULL)
    theNodeTest->put(os);

	vector<rchandle<expr> >::const_iterator it = thePreds.begin();
	vector<rchandle<expr> >::const_iterator en = thePreds.end();
	for (; it != en; ++it)
  {
		rchandle<expr> e = *it;
		//d Assert<null_pointer>(e_h!=NULL);
		Assert(e != NULL);
		e->put(os);
	}
	return os << OUTDENT << "]\n";
}


void axis_step_expr::accept(expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;

  theNodeTest->accept(v);

  if (numPreds() > 0)
  {
    ZORBA_ERROR_ALERT(
       error_messages::XQP0014_SYSTEM_SHOUD_NEVER_BE_REACHED,
       error_messages::SYSTEM_ERROR,
       NULL,
       false,
       "Path expressions with predicates are not implemented");
  }

	v.end_visit(*this);
}


/*******************************************************************************

  [78] NodeTest ::= KindTest | NameTest

  [79] NameTest ::= QName | Wildcard
  [80] Wildcard ::= "*" | (NCName ":" "*") | ("*" ":" NCName)

  [123] KindTest ::= DocumentTest | ElementTest | AttributeTest |
								     SchemaElementTest | SchemaAttributeTest |
								     PITest | CommentTest | TextTest | AnyKindTest

********************************************************************************/
match_expr::match_expr(yy::location const& loc)
  :
	expr(loc),
	theWildKind(match_no_wild),
	theQName(NULL),
	theTypeName(NULL),
  theNilledAllowed(false)
{
}


match_expr::~match_expr()
{
}


ostream& match_expr::put(ostream& os) const
{
	os << INDENT << "match_expr[";
	switch (theTestKind)
  {
	case match_no_test:   os << "no_test("; break;
	case match_name_test: os << "name_test("; break;
	case match_doc_test:
  {
		os << "doc_test(";
		switch (theDocTestKind)
    {
		case match_no_test:   os << "no_test("; break;
		case match_elem_test: os << "element("; break;
		case match_attr_test: os << "attribute("; break;
		default: os << "(??";
		}
		break;
	}
	case match_elem_test:			os << "element("; break;
	case match_attr_test:			os << "attribute("; break;
	case match_xs_elem_test:	os << "schema-element("; break;
	case match_xs_attr_test:	os << "schema-element("; break;
	case match_pi_test:				os << "pi("; break;
	case match_comment_test:	os << "comment("; break;
	case match_text_test:			os << "text("; break;
	case match_anykind_test:	os << "node("; break;
	default: os << "(??";
	}

  switch (theWildKind)
  {
  	case match_no_wild:
      if (theQName != NULL)
        PUT_QNAME (theQName, os);
      break;
  	case match_all_wild:
      os << "*";
      break;
  	case match_prefix_wild:
      os << "*:" << theWildName;
      break;
  	case match_name_wild:
      os << theWildName << ":*";
      break;
  	default:
      os << "??";
  }

	if (theTypeName != NULL)
  {
		PUT_QNAME (theTypeName, os) << endl;
	}

	os << ")";
	return os << "]\n";
}


void match_expr::accept(expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;
	v.end_visit(*this);
}


StoreConsts::NodeKind_t match_expr::getNodeKind() const
{
  switch (theTestKind)
  {
  case match_name_test:
    return StoreConsts::elementNode;
  case match_doc_test:
    return StoreConsts::elementNode;
  case match_elem_test:
    return StoreConsts::elementNode;
  case match_attr_test:
    return StoreConsts::attributeNode;
  case match_xs_elem_test:
    return StoreConsts::elementNode;
  case match_xs_attr_test:
    return StoreConsts::attributeNode;
  case match_pi_test:
    return StoreConsts::piNode;
  case match_text_test:
    return StoreConsts::textNode;
  case match_comment_test:
    return StoreConsts::commentNode;
  case match_anykind_test:
    return StoreConsts::anyNode;
  default:
  {
    ZORBA_ERROR_ALERT(
         error_messages::XQP0014_SYSTEM_SHOUD_NEVER_BE_REACHED,
         error_messages::SYSTEM_ERROR,
         NULL,
         false,
         "Unknown node test kind");
  }
  }
  return StoreConsts::anyNode;
}


// [84] [http://www.w3.org/TR/xquery/#prod-xquery-PrimaryExpr]



// [85] [http://www.w3.org/TR/xquery/#prod-xquery-PrimaryExpr]

const_expr::const_expr(yy::location const& loc,
                           xqpString v)
  :
	expr(loc),
	val (ITEM_FACTORY.createString (v))
{
}

const_expr::const_expr(yy::location const& loc,
                           xqp_integer v)
:
	expr(loc),
	val (ITEM_FACTORY.createInteger (v))
{
}

const_expr::const_expr(
	yy::location const& loc,
	xqp_decimal v)
:
	expr(loc),
	val (ITEM_FACTORY.createDecimal (v))
{
}

const_expr::const_expr(
	yy::location const& loc,
	xqp_double v)
:
	expr(loc),
	val (ITEM_FACTORY.createDouble (v))
{
}

const_expr::const_expr(
	yy::location const& loc,
	xqp_boolean v)
:
	expr(loc),
	val (ITEM_FACTORY.createBoolean (v))
{
}

const_expr::~const_expr()
{
}

ostream& const_expr::put( ostream& os) const
{
	switch (1) {
#if 0  // TODO
	case lit_string: os << INDENT << "string[" << sval; break;
	case lit_integer: os << INDENT << "integer[" << ival; break;
	case lit_decimal: os << INDENT << "decimal[" << decval; break;
	case lit_double: os << INDENT << "double[" << dval; break;
#endif
	default: os << INDENT << "???[]";
	}
	return os << OUTDENT << "]\n";
}

void const_expr::accept(
	expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;
	v.end_visit(*this);
}



// [91] [http://www.w3.org/TR/xquery/#prod-xquery-OrderedExpr]

order_expr::order_expr(
	yy::location const& loc,
	order_type_t _type,
	rchandle<expr> _expr_h)
:
	expr(loc),
	type(_type),
	expr_h(_expr_h)
{
}

order_expr::~order_expr()
{
}

ostream& order_expr::put( ostream& os) const
{
	os << "order_expr[";
	switch (type) {
	case ordered: os << "ordered\n"; break;
	case unordered: os << "unordered\n"; break;
	default: os << "??\n";
	}
	//d Assert<null_pointer>(expr_h!=NULL);
	Assert(expr_h!=NULL);
	expr_h->put(os) << endl;
	return os << "]\n";
}

void order_expr::accept(
	expr_visitor& v)
{
}



// [93] [http://www.w3.org/TR/xquery/#prod-xquery-FunctionCall]

// [96] [http://www.w3.org/TR/xquery/#doc-exquery-DirElemConstructor]
elem_expr::elem_expr(
	yy::location const& loc,
	QNameItem_t _qname_h,
	expr_t _attrs_expr_h,
	expr_t _content_expr_h)
:
	constructor_expr(loc),
	qname_h(_qname_h),
	attrs_expr_h(_attrs_expr_h),
	content_expr_h(_content_expr_h)
{}
elem_expr::~elem_expr(){}

void elem_expr::accept(expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;
	if (this->attrs_expr_h != NULL)
		this->attrs_expr_h->accept(v);
	if (this->content_expr_h != NULL)
		this->content_expr_h->accept(v);
	v.end_visit(*this);
}

std::ostream& elem_expr::put(std::ostream& os) const
{
	os << INDENT << "elem_expr[";
	PUT_QNAME (qname_h, os);
	if (attrs_expr_h != NULL)
		attrs_expr_h->put(os);
	if (content_expr_h != NULL)
		content_expr_h->put(os);
	return os << OUTDENT << "]\n";
}

// [110] [http://www.w3.org/TR/xquery/#prod-xquery-CompDocConstructor]

doc_expr::doc_expr(
	yy::location const& loc,
	rchandle<expr> _docuri_h)
:
	constructor_expr(loc),
	docuri_h(_docuri_h)
{
}

doc_expr::~doc_expr()
{
}

ostream& doc_expr::put( ostream& os) const
{
	os << INDENT << "doc_expr[\n";
	Assert(docuri_h != NULL);
	docuri_h->put(os);
	return os << OUTDENT << "]\n";
}

void doc_expr::accept(
	expr_visitor& v)
{
}



// [111] [http://www.w3.org/TR/xquery/#prod-xquery-CompElemConstructor]

compElem_expr::compElem_expr(
	yy::location const& loc,
	QNameItem_t _qname_h,
	rchandle<expr> _content_expr_h)
:
	constructor_expr(loc),
	qname_h(_qname_h),
	qname_expr_h(NULL),
	content_expr_h(_content_expr_h)
{
}

compElem_expr::compElem_expr(
	yy::location const& loc,
	rchandle<expr> _qname_expr_h,
	rchandle<expr> _content_expr_h)
:
	constructor_expr(loc),
	qname_h(NULL),
	qname_expr_h(_qname_expr_h),
	content_expr_h(_content_expr_h)
{
}

compElem_expr::~compElem_expr()
{
}

ostream& compElem_expr::put( ostream& os) const
{
	os << INDENT << "compElem_expr[<";
	//d Assert<bad_arg>(qname_h!=NULL || qname_expr_h!=NULL);
	Assert(qname_h!=NULL || qname_expr_h!=NULL);
	if (qname_h!=NULL) {
		PUT_QNAME (qname_h, os) << ">\n";
	}
	else {
		qname_expr_h->put(os) << ">\n";
	}
	vector<nsbinding>::const_iterator it = ns_begin();
	vector<nsbinding>::const_iterator en = ns_end();
	for (; it!=en; ++it) {
		nsbinding nsb = *it;
		string ncname = nsb.first;
		string nsuri = nsb.second;
		os << INDENT << "xmlns:" << ncname << "=\"" << nsuri << "\"\n"; UNDENT;
	}
	//d Assert<null_pointer>(content_expr_h!=NULL);
	if (content_expr_h != NULL )
		content_expr_h->put(os);
	return os << OUTDENT << "]\n";
}

void compElem_expr::accept(
	expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;
	if (this->content_expr_h != NULL)
		this->content_expr_h->accept(v);
	v.end_visit(*this);
}



// [113] [http://www.w3.org/TR/xquery/#prod-xquery-CompAttrConstructor]

attr_expr::attr_expr(
	yy::location const& loc,
	QNameItem_t _qname_h,
	rchandle<expr> _val_expr_h)
:
	constructor_expr(loc),
	qname_h(_qname_h),
	qname_expr_h(NULL),
	val_expr_h(_val_expr_h)
{
}

attr_expr::attr_expr(
	yy::location const& loc,
	rchandle<expr> _qname_expr_h,
	rchandle<expr> _val_expr_h)
:
	constructor_expr(loc),
	qname_h(NULL),
	qname_expr_h(_qname_expr_h),
	val_expr_h(_val_expr_h)
{
}

attr_expr::~attr_expr()
{
}

ostream& attr_expr::put( ostream& os) const
{
	os << INDENT << "attr_expr[@";
	//d Assert<bad_arg>(qname_h!=NULL || qname_expr_h!=NULL);
	Assert(qname_h!=NULL || qname_expr_h!=NULL);
	if (qname_h!=NULL) {
		PUT_QNAME (qname_h, os);
	}
	else {
		qname_expr_h->put (os);
	}
	
	if (val_expr_h != NULL)
	{
		os << "=";
		val_expr_h->put(os);
	}
	return os << OUTDENT << "]\n";
}

void attr_expr::accept(
	expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;
	if (this->qname_expr_h != NULL)
		this->qname_expr_h->accept(v);
	if (this->val_expr_h != NULL)
		this->val_expr_h->accept(v);
	v.end_visit(*this);
}



// [114] [http://www.w3.org/TR/xquery/#prod-xquery-CompTextConstructor]

text_expr::text_expr(
	yy::location const& loc,
  text_constructor_type type_arg,
	expr_t text_arg)
:
	constructor_expr(loc),
  type (type_arg),
	text(text_arg)
{
}

text_expr::~text_expr()
{
}

ostream& text_expr::put( ostream& os) const
{
	os << INDENT << "text_expr[";
	os << INDENT << "text="; text->put (os); os << "\n";
	return os << OUTDENT << "]\n";
}

void text_expr::accept(
	expr_visitor& v)
{
	if (!v.begin_visit(*this)) return;
  ACCEPT (text);
	v.end_visit(*this);
}



// [115] [http://www.w3.org/TR/xquery/#prod-xquery-CompCommentConstructor]


// [114] [http://www.w3.org/TR/xquery/#prod-xquery-CompPIConstructor]

pi_expr::pi_expr(
	yy::location const& loc,
	std::string _target,
	rchandle<expr> _content_expr_h)
:
	text_expr(loc, text_expr::pi_constructor, _content_expr_h),
	target(_target),
	target_expr_h(NULL)
{
}

pi_expr::pi_expr(
	yy::location const& loc,
	rchandle<expr> _target_expr_h,
	rchandle<expr> _content_expr_h)
:
	text_expr(loc, text_expr::pi_constructor, _content_expr_h),
	target(""),
	target_expr_h(_target_expr_h)
{
}

pi_expr::~pi_expr()
{
}

ostream& pi_expr::put( ostream& os) const
{
	os << INDENT << "pi_expr[target=";
	Assert(target.length()>0 || target_expr_h!=NULL);
	if (target.length()>0) {
		os << target;
	}
	else {
		target_expr_h->put(os);
	}
	Assert(get_text () !=NULL);
	os << ", content=";
	get_text ()->put(os);
	return os << OUTDENT << "]\n";
}

void pi_expr::accept(
	expr_visitor& v)
{
}



} /* namespace xqp */
/* vim:set ts=2 sw=2: */

