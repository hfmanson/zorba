/*
 * Copyright 2006-2008 The FLWOR Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ZORBA_EXPR_BASE_H
#define ZORBA_EXPR_BASE_H

#include "common/common.h"
#include "common/shared_types.h"

#include "context/static_context_consts.h"

#include "types/typeimpl.h"
#include "compiler/parser/query_loc.h"
#include "compiler/parser/parse_constants.h"
#include "compiler/expression/expr_consts.h"
#include "compiler/semantic_annotations/annotation_holder.h"

namespace zorba 
{

enum expr_kind_t 
{
  wrapper_expr_kind,
  sequential_expr_kind,
  exit_expr_kind,
  flowctl_expr_kind,
  while_expr_kind,
  var_expr_kind,
  flwor_expr_kind,
  gflwor_expr_kind,
  trycatch_expr_kind,
  promote_expr_kind,
  typeswitch_expr_kind,
  if_expr_kind,
  fo_expr_kind,
  instanceof_expr_kind,
  treat_expr_kind,
  castable_expr_kind,
  cast_expr_kind,
  name_cast_expr_kind,
  validate_expr_kind,
  extension_expr_kind,
  relpath_expr_kind,
  axis_step_expr_kind,
  match_expr_kind,
  const_expr_kind,
  order_expr_kind,
  elem_expr_kind,
  doc_expr_kind,
  attr_expr_kind,
  text_expr_kind,
  pi_expr_kind,
  insert_expr_kind,
  delete_expr_kind,
  rename_expr_kind,
  replace_expr_kind,
  transform_expr_kind,
  eval_expr_kind,
  debugger_expr_kind,
  unknown_expr_kind
};

class expr;
typedef rchandle<expr> expr_t;

class wrapper_expr;
typedef rchandle<wrapper_expr> wrapper_expr_t;

class expr_visitor;

class expr_iterator_data;

class expr_iterator;


/*******************************************************************************

  A class to iterate over the subexpressions of an expr. 

  Each actual instance of the abstract expr class has its own way of storing
  pointers to its subexpressions. So, to make expr_iterator work with any
  kind of expr, the actual work of expr_iterator is done by an actual instance
  of expr_iterator_data (there is a subclass of expr_iterator_data for each
  kind of expr).

********************************************************************************/
class expr_iterator 
{
private:
  // should be an auto_ptr, but older gcc's don't like auto_ptr w/ forward decl
  expr_iterator_data * iter;

public:
  expr_iterator() : iter(0) {}

  expr_iterator(expr_iterator_data* iter_) : iter (iter_) {}

  expr_iterator(const expr_iterator& other);

  ~expr_iterator();

  expr_iterator& operator=(const expr_iterator& other);

  expr_iterator &operator++();
  expr_iterator operator++(int);

  expr_t& operator*();

  bool done() const;

private:
  // comparisson forbidden; use done()
  bool operator==(const expr_iterator& other) { return false; }
};


/*******************************************************************************
  Base class for the expression tree node hierarchy
********************************************************************************/
class expr : public AnnotationHolder 
{
public:
  typedef rchandle<expr> expr_t;

  typedef std::map<var_expr *, expr_t> substitution_t;

  typedef substitution_t::iterator subst_iter_t;

protected:
  // Pitfall when using the cache -- AVOID THIS SCENARIO:
  // (1) obtain a non-const ptr to a child expression (cache is invalidated)
  // (2) call an operation P() that caches its result
  // (3) modify the child expr (cache is NOT invalidated)
  // (4) call P() again and get (possibly wrong) cached result
  struct Cache
  {
    struct 
    {
      bool             valid;
      xqtref_t         t;
      static_context * sctx;
    } type;

    mutable struct 
    {
      bool          valid;
      expr_update_t kind;
    } upd_seq_kind;
  };

  short    theSctxId;
  QueryLoc loc;
  Cache    theCache;

public:
  SERIALIZABLE_ABSTRACT_CLASS(expr)
  SERIALIZABLE_CLASS_CONSTRUCTOR2(expr, AnnotationHolder)
  void serialize(::zorba::serialization::Archiver& ar);

public:
  static expr_update_t update_type_anding(
        expr_update_t type1, 
        expr_update_t type2, 
        const QueryLoc& loc);

public:
  virtual ~expr();

  virtual expr_kind_t get_expr_kind() const = 0;

  const QueryLoc& get_loc() const { return loc; }

  void set_loc(const QueryLoc& aLoc) { loc = aLoc; }

  short get_sctx_id() const { return theSctxId; }

  virtual expr_update_t get_update_type() const;

  virtual bool is_updating() const;
  virtual bool is_sequential() const;
  virtual bool is_vacuous() const;
  virtual bool is_simple() const;
  virtual bool is_updating_or_vacuous() const;

  virtual void compute_upd_seq_kind() const;

  virtual xqtref_t return_type(static_context* sctx);

  virtual xqtref_t return_type_impl(static_context* sctx);

  virtual expr_iterator expr_begin();

  virtual void next_iter(expr_iterator_data &) = 0;

  expr_t clone();

  virtual expr_t clone(substitution_t& substitution);

  virtual void accept(expr_visitor& v);

  virtual void accept_children(expr_visitor& v);

  virtual std::ostream& put(std::ostream&) const = 0;

  virtual std::string toString() const;

protected:
  expr(short, const QueryLoc&);

  void invalidate() 
  {
    theCache.type.valid = false;
    theCache.upd_seq_kind.valid = false;
  }

  // Returns true if all modifiers, as well as all accessors that permit future
  // modifications of child expressions, call invalidate(). Note that expr
  // iterators are compliant.
  virtual bool cache_compliant() { return false; }

  virtual expr_iterator_data* make_iter();
};


/*******************************************************************************
  Base class for iterators that iterate over the subexpressions (children) of
  a given expr. Notice that theCurrentChild is a pointer to expr_t.
********************************************************************************/
class expr_iterator_data 
{
protected:
  expr   * theExpr;
  
public:
  expr_t * theCurrentChild;
  int      theState;
  
public:
  expr_iterator_data(expr* e) : theExpr(e), theCurrentChild(NULL), theState(0) {}

  virtual ~expr_iterator_data() {}

  virtual void next() { theExpr->next_iter(*this); }

  bool done() const;
};

} /* namespace zorba */
#endif  /*  ZORBA_EXPR_BASE_H */

/*
 * Local variables:
 * mode: c++
 * End:
 */
/* vim:set ts=2 sw=2: */
