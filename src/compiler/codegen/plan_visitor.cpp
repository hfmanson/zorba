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

#include <iostream>
#include <vector>
#include <stack>

#include "util/properties.h"
#include "util/tracer.h"
#include "util/stl_extra.h"
#include "util/hashmap.h"

#include "compiler/codegen/plan_visitor.h"
#include "compiler/expression/expr.h"
#include "compiler/expression/expr_visitor.h"
#include "compiler/parser/parse_constants.h"

#include "system/globalenv.h"

#include "zorbaerrors/Assert.h"

#include "context/namespace_context.h"
#include "context/static_context.h"
#include "context/static_context_consts.h"

#include "runtime/sequences/SequencesImpl.h"
#include "runtime/core/sequencetypes.h"
#include "runtime/core/item_iterator.h"
#include "runtime/core/var_iterators.h"
#include "runtime/core/constructors.h"
#include "runtime/core/path_iterators.h"
#include "runtime/core/path.h"
#include "runtime/core/nodeid_iterators.h"
#include "runtime/core/flwor_iterator.h"
#include "runtime/core/trycatch.h"
#include "runtime/core/gflwor/groupby_iterator.h"
#include "runtime/core/gflwor/tuplestream_iterator.h"
#include "runtime/core/gflwor/for_iterator.h"
#include "runtime/core/gflwor/outerfor_iterator.h"
#include "runtime/core/gflwor/let_iterator.h"
#include "runtime/core/gflwor/window_iterator.h"
#include "runtime/core/gflwor/where_iterator.h"
#include "runtime/core/gflwor/count_iterator.h"
#include "runtime/core/gflwor/tuplesource_iterator.h"
#include "runtime/core/gflwor/orderby_iterator.h"
#include "runtime/validate/validate.h"
#include "runtime/fncontext/FnContextImpl.h"
#include "runtime/misc/MiscImpl.h"
#include "runtime/update/update.h"
#include "runtime/visitors/printervisitor.h"
#include "runtime/visitors/iterprinter.h"

#include "functions/function.h"

#ifdef ZORBA_DEBUGGER
#include <zorba/zorba.h>
#include "runtime/debug/zorba_debugger_iterators.h"
#endif

#include "types/typeops.h"

#include "store/api/store.h"
#include "store/api/item.h"
#include "store/api/item_factory.h"
#include "store/api/iterator.h"


#define QLOCDECL const QueryLoc &qloc = v.get_loc(); (void) qloc
#ifndef NDEBUG
#define CODEGEN_TRACE(msg)                      \
  QLOCDECL;                                     \
  if (Properties::instance()->traceCodegen()) { \
    cout << (msg) << TRACE << ", stk size " << itstack.size () << endl; \
    cout << endl; \
    v.put(cout); \
    cout << "StackTop:" << endl; \
    print_stack(); \
  }
#define CODEGEN_TRACE_IN( msg )  \
  CODEGEN_TRACE (string (++depth, ' ') + msg)
#define CODEGEN_TRACE_OUT( msg ) \
  CODEGEN_TRACE (string (depth--, ' ') + msg)
#else
#define CODEGEN_TRACE(msg) QLOCDECL
#define CODEGEN_TRACE_IN(msg) CODEGEN_TRACE(msg)
#define CODEGEN_TRACE_OUT(msg) CODEGEN_TRACE(msg)
#endif

#define DOT_VAR "$$dot"
#define DOT_POS_VAR "$$pos"
#define LAST_IDX_VAR "$$last-idx"

using namespace std;
namespace zorba 
{

#define ITEM_FACTORY (GENV.getStore().getItemFactory())

template <typename T> T pop_stack (stack<T> &stk) {
    assert (! stk.empty ());
    T x = stk.top ();
    stk.pop ();
    return x;
  }


template<class T> T &peek_stack (stack<T> &stk) {
    ZORBA_ASSERT (! stk.empty ());
    return stk.top ();
  }


template <typename V>
struct vector_destroyer {
    void operator()(const struct hash64map<vector<V> *>::entry& entry)
    {
        delete (const_cast<struct hash64map<vector<V> *>::entry&>(entry)).val;
    }
};


/*******************************************************************************

********************************************************************************/
class plan_visitor : public expr_visitor
{
public:

  typedef rchandle<expr> expr_h_t;
  typedef rchandle<ForVarIterator> ForVarIter_t;
  typedef rchandle<LetVarIterator> LetVarIter_t;

protected:
  uint32_t depth;
  
  stack<PlanIter_t>                      itstack;

  stack<expr*>                           theConstructorsStack;
  stack<bool>                            theAttrContentStack;

  hash64map<vector<ForVarIter_t> *>      fvar_iter_map;
  hash64map<vector<ForVarIter_t> *>      pvar_iter_map;
  hash64map<vector<LetVarIter_t> *>      lvar_iter_map;
  hash64map<vector<LetVarIter_t> *>    * param_var_iter_map;
  hash64map<vector<LetVarIter_t> *>      catchvar_iter_map;
  hash64map<vector<ForVarIter_t> *>      copy_var_iter_map;

  static vector<ForVarIter_t> no_var_iters;

  CompilerCB                           * ccb;

#define LOOKUP_OP1( local ) (ccb->m_sctx->lookup_builtin_fn (":" local, 1))

public:

	plan_visitor(CompilerCB *ccb_, hash64map<vector<LetVarIter_t> *> *param_var_map = NULL)
    :
    depth (0),
    param_var_iter_map(param_var_map),
    ccb (ccb_)
  {}

	~plan_visitor() {
    for_each(fvar_iter_map.begin(), fvar_iter_map.end(), vector_destroyer<ForVarIter_t>());
    for_each(pvar_iter_map.begin(), pvar_iter_map.end(), vector_destroyer<ForVarIter_t>());
    for_each(lvar_iter_map.begin(), lvar_iter_map.end(), vector_destroyer<LetVarIter_t>());
    for_each(catchvar_iter_map.begin(), catchvar_iter_map.end(), vector_destroyer<LetVarIter_t>());
    for_each(copy_var_iter_map.begin(), copy_var_iter_map.end(), vector_destroyer<ForVarIter_t>());
  }

public:
	PlanIter_t pop_itstack() {
    return pop_stack (itstack);
	}

  bool is_enclosed_expr(expr* e) {
    fo_expr* foe = dynamic_cast<fo_expr*>(e);
    if (foe != NULL &&
        foe->get_fname()->getLocalName()->byteEqual(":enclosed-expr", 14))
      return true;

    return false;
  }

void print_stack()
{
  if (itstack.empty()) {
    cout << "EMPTY" << endl;
  } else {
    XMLIterPrinter vp (cout);
    print_iter_plan (vp, peek_stack(itstack));
  }
}


bool begin_visit (expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (expr& v) {
  CODEGEN_TRACE_OUT("");
}

#ifdef ZORBA_DEBUGGER
bool begin_visit (debugger_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (debugger_expr& v) {
  CODEGEN_TRACE_OUT(""); 
  checked_vector<PlanIter_t> argv;
  checked_vector<store::Item_t> varnames;
  checked_vector<string> var_keys;
  checked_vector<global_binding> globals;
  checked_vector<xqtref_t> vartypes;
  for (unsigned i = 0; i < v.var_count (); i++) 
  {
    varnames.push_back (v.var_at (i).varname);
    var_keys.push_back (v.var_at (i).var_key);
    vartypes.push_back (v.var_at (i).type);
    argv.push_back (pop_itstack ());
  }
  list<global_binding> lGlobals = v.getGlobals();
  list<global_binding>::iterator it;
  for ( it = lGlobals.begin(); it != lGlobals.end(); ++it )
  {
    globals.push_back( *it );
  }
  argv.push_back (pop_itstack ());
  reverse (argv.begin (), argv.end ());
  itstack.push (new FnDebugIterator(qloc, varnames, var_keys, vartypes, globals, argv, v.isForExpr()));
}
#endif

bool begin_visit (wrapper_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (wrapper_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t iter = pop_itstack ();
  iter->setLocation (v.get_loc ());
  itstack.push (iter);
}

bool begin_visit (sequential_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (sequential_expr& v) {
  CODEGEN_TRACE_OUT("");
  checked_vector<PlanIter_t> argv;
  size_t lSize = v.size();
  for (unsigned i = 0; i < lSize; i++)
    argv.push_back (pop_itstack ());
  reverse (argv.begin (), argv.end ());

  itstack.push (new SequentialIterator (qloc, argv, v.is_updating ()));
}

bool begin_visit (var_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

template<class Iter>
void var_codegen (const var_expr &v, hash64map<vector<rchandle<Iter> > *> &mmap) {
  const QueryLoc &qloc = v.get_loc ();
  vector<rchandle<Iter> > *map = NULL;
  bool bound = mmap.get ((uint64_t) &v, map);
  
  ZORBA_ASSERT (bound);
  Iter *v_p =
    new Iter(v.get_varname()->getLocalName(), qloc, (void *) &v);
  map->push_back (v_p);
  itstack.push(v_p);
}

void general_var_codegen (const var_expr& v)
{
  const QueryLoc &qloc = v.get_loc ();
  switch (v.kind) 
  {
  case var_expr::count_var:
  case var_expr::wincond_var:
  case var_expr::wincond_in_var:
  case var_expr::wincond_pos_var:
  case var_expr::wincond_in_pos_var:
  case var_expr::groupby_var:
  case var_expr::for_var: {
    var_codegen (v, fvar_iter_map);
    break;
  }

  case var_expr::pos_var: {
    var_codegen (v, pvar_iter_map);
    break;
  }
    
  case var_expr::win_var:
  case var_expr::non_groupby_var:
  case var_expr::let_var: {
    var_codegen (v, lvar_iter_map);
    break;
  }

  case var_expr::param_var: {
    var_codegen (v, *param_var_iter_map);
    break;
  }

  case var_expr::catch_var:
  {
    var_codegen (v, catchvar_iter_map);
    break;
  }

  case var_expr::copy_var:
  {
    var_codegen (v, copy_var_iter_map);
    break;
  }

  case var_expr::context_var: {
    xqpString varname = v.get_varname()->getStringValue().getp();
    if (varname == DOT_VAR) {
      vector<PlanIter_t> ctx_args;
      store::Item_t qname;
      xqpStringStore_t dot = new xqpStringStore (".");
      ITEM_FACTORY->createString (qname, dot);
      ctx_args.push_back (new SingletonIterator (qloc, qname));
      itstack.push (new CtxVariableIterator (qloc, ctx_args));
    } else if (varname == DOT_POS_VAR) {
      store::Item_t i;
      ITEM_FACTORY->createInteger (i, Integer::parseInt((int32_t)1));
      itstack.push (new SingletonIterator (qloc, i));
    } else if (varname == LAST_IDX_VAR) {
      store::Item_t i;
      ITEM_FACTORY->createInteger (i, Integer::parseInt((int32_t)1));
      itstack.push (new SingletonIterator (qloc, i));
    } else {
      expr_t lookup_expr =
        new fo_expr (qloc, LOOKUP_OP1 ("ctxvariable"),
                     new const_expr (qloc, dynamic_context::var_key (&v)));
      lookup_expr->accept (*this);
    }
 
    break;
  }

  case var_expr::unknown_var:
  default:
    // ZORBA_ASSERT (false);
    break;
  }
}

void end_visit (var_expr& v) {
  CODEGEN_TRACE_OUT("");
  general_var_codegen (v);
}

bool begin_visit (flwor_expr& v) {
  CODEGEN_TRACE_IN("");
  for (vector<rchandle<forlet_clause> >::const_iterator it = v.clause_begin ();
       it != v.clause_end();
       ++it) 
  {
    rchandle<var_expr> var = (*it)->get_var ();
    uint64_t k = (uint64_t) &*var;

    if (var->kind == var_expr::for_var) {
      fvar_iter_map.put(k, new vector<ForVarIter_t>());
      var_expr* pos_var = (*it)->get_pos_var().getp();
      if (pos_var != NULL)
        pvar_iter_map.put((uint64_t)pos_var, new vector<ForVarIter_t>());
    }
    else if (var->kind == var_expr::let_var) {
      lvar_iter_map.put(k, new vector<LetVarIter_t>());
    }
    else
      ZORBA_ASSERT (false);
  }
  
  for(flwor_expr::group_list_t::const_iterator it = v.group_begin();
      it != v.group_end();
      ++it)
  {
    rchandle<var_expr> var = (*it)->getInnerVar();
    uint64_t k = (uint64_t) &*var;
    fvar_iter_map.put(k, new vector<ForVarIter_t>());
  }

  for(flwor_expr::group_list_t::const_iterator it = v.non_group_begin();
      it != v.non_group_end();
      ++it)
  {
    rchandle<var_expr> var = (*it)->getInnerVar();
    uint64_t k = (uint64_t) &*var;
    lvar_iter_map.put(k, new vector<LetVarIter_t>());
  }

  return true;
}



void end_visit (flwor_expr& v) {
  CODEGEN_TRACE_OUT("");

  PlanIter_t ret = pop_itstack ();

  vector<FLWORIterator::OrderSpec> orderSpecs;
  for (flwor_expr::orderspec_list_t::reverse_iterator i = v.orderspec_rbegin ();
       i != v.orderspec_rend ();
       i++)
  {
    flwor_expr::orderspec_t spec = *i;
    orderSpecs.push_back(FLWORIterator::OrderSpec(pop_itstack(),
                                                  spec.second->empty_mode == StaticContextConsts::empty_least,
                                                  spec.second->dir == ParseConstants::dir_descending, spec.second->collation));
  }
  reverse (orderSpecs.begin (), orderSpecs.end ());
  
  auto_ptr<FLWORIterator::OrderByClause> orderby(orderSpecs.empty() ? NULL : new FLWORIterator::OrderByClause(orderSpecs, v.get_order_stable ()));
  
  PlanIter_t group_where = 0;
  if (v.get_group_where() != 0)
  group_where = pop_itstack();
   
  vector<FLWORIterator::GroupingOuterVar> nonGroupBys;
  for(flwor_expr::group_list_t::reverse_iterator i = v.non_group_rbegin();
      i != v.non_group_rend();
      ++i)
  {
    rchandle<group_clause> group = *i;
    vector<LetVarIter_t>* lInnerVars = 0;
    var_expr* lVar = group->getInnerVar();
    ZORBA_ASSERT(lvar_iter_map.get((uint64_t)lVar, lInnerVars));

    PlanIter_t lInput = pop_itstack();

    nonGroupBys.push_back(FLWORIterator::GroupingOuterVar(lInput, *lInnerVars));
  }
  
  vector<FLWORIterator::GroupingSpec> groupBys;
  for(flwor_expr::group_list_t::reverse_iterator i = v.group_rbegin();
      i != v.group_rend();
      ++i)
  {
    rchandle<group_clause> group = *i;
    vector<ForVarIter_t>* lInnerVars = 0;
    var_expr* lVar = group->getInnerVar();
    ZORBA_ASSERT(fvar_iter_map.get((uint64_t)lVar, lInnerVars));
    
    PlanIter_t lInput = pop_itstack();
    
    xqp_string lCollation = group->getCollation();
    
    groupBys.push_back(FLWORIterator::GroupingSpec(lInput, *lInnerVars, lCollation));
  }
  
  auto_ptr<FLWORIterator::GroupByClause> groupby(
  groupBys.empty() 
  ? NULL 
  : new FLWORIterator::GroupByClause(groupBys, nonGroupBys, group_where));

  PlanIter_t where = NULL;
  if (v.get_where () != NULL)
    where = pop_itstack ();
  
  vector<FLWORIterator::ForLetClause> clauses;
  stack<PlanIter_t> inputs;
  for (vector<rchandle<forlet_clause> >::const_iterator it = v.clause_begin ();
       it != v.clause_end();
       ++it)
  {
    inputs.push(pop_itstack());
  }

  for (vector<rchandle<forlet_clause> >::const_iterator it = v.clause_begin ();
       it != v.clause_end();
       ++it) 
  {
    PlanIter_t input = pop_stack(inputs);

    if ((*it)->get_type () == forlet_clause::for_clause) {
      vector<ForVarIter_t> *var_iters = NULL, *pvar_iters = NULL;
      var_expr* var = (*it)->get_var ().getp();
      var_expr* pos_var = (*it)->get_pos_var().getp();
      ZORBA_ASSERT( fvar_iter_map.get((uint64_t)var, var_iters) );
      if (pos_var == NULL) {
        clauses.push_back(FLWORIterator::ForLetClause(var, *var_iters, input));
      } else {
        ZORBA_ASSERT( pvar_iter_map.get((uint64_t) pos_var, pvar_iters) );
        clauses.push_back(FLWORIterator::ForLetClause(var, *var_iters, *pvar_iters, input));
      }
    }
    else if ((*it)->get_type () == forlet_clause::let_clause) {
      vector<LetVarIter_t> *var_iters = NULL;
      var_expr* var = (*it)->get_var ().getp ();
      ZORBA_ASSERT( lvar_iter_map.get((uint64_t)var, var_iters) );
      clauses.push_back(FLWORIterator::ForLetClause(var, *var_iters, input, true));
    }
  }

  FLWORIterator *iter = new FLWORIterator(
      qloc, clauses, where, groupby.release(), 
      orderby.release(), ret, v.isUpdating());
  itstack.push(iter);
}

void init_wincond_var_map2 (const flwor_wincond::vars vars) {
  if (vars.posvar != NULL)
    fvar_iter_map.put((uint64_t) &*vars.posvar, new vector<ForVarIter_t>());
  if (vars.curr != NULL)
    fvar_iter_map.put((uint64_t) &*vars.curr, new vector<ForVarIter_t>());
  if (vars.prev != NULL)
    fvar_iter_map.put((uint64_t) &*vars.prev, new vector<ForVarIter_t>());
  if (vars.next != NULL)
    fvar_iter_map.put((uint64_t) &*vars.next, new vector<ForVarIter_t>());
}

void init_wincond_var_map (const flwor_wincond *wc) {
  if (wc == NULL) return;
  init_wincond_var_map2 (wc->get_in_vars ());
  init_wincond_var_map2 (wc->get_out_vars ());
}

void init_var_maps (varref_t var, const forletwin_gclause *flc) {
  enum var_expr::var_kind kind = var->get_kind ();
  uint64_t k = (uint64_t) &*var;
  if (kind == var_expr::for_var 
      || (flc == NULL && (kind == var_expr::groupby_var || kind == var_expr::count_var || kind == var_expr::wincond_var || kind == var_expr::wincond_pos_var))) {
    fvar_iter_map.put(k, new vector<ForVarIter_t>());
    if (flc != NULL) {
      var_expr* pos_var = flc->get_pos_var().getp();
      if (pos_var != NULL)
        pvar_iter_map.put((uint64_t)pos_var, new vector<ForVarIter_t>());
    }
  } else if (kind == var_expr::let_var || kind == var_expr::non_groupby_var) {
    lvar_iter_map.put(k, new vector<LetVarIter_t>());
  } else if (kind == var_expr::win_var) {
    lvar_iter_map.put(k, new vector<LetVarIter_t>());
    if (flc != NULL) {
      init_wincond_var_map (flc->get_win_start ());
      init_wincond_var_map (flc->get_win_stop ());
    }
  }
  else ZORBA_ASSERT (false);
}

bool begin_visit (gflwor_expr& v) {
  CODEGEN_TRACE_IN("");
  for (gflwor_expr::clause_list_t::const_iterator it = v.clause_begin ();
       it != v.clause_end(); ++it)
  {
    const flwor_clause &c = *(*it);
    if (typeid (c) == typeid (forletwin_gclause)) {
      const forletwin_gclause *flc = static_cast<const forletwin_gclause *> (&c);
      init_var_maps (flc->get_var (), flc);
    } else if (typeid (c) == typeid (group_gclause)) {
      const group_gclause *gbc = static_cast<const group_gclause *> (&c);
      const group_gclause::rebind_list_t &inner_rebind = gbc->get_inner_rebind (), &outer_rebind = gbc->get_outer_rebind ();
      for (unsigned i = 0; i < inner_rebind.size (); i++)
        init_var_maps (inner_rebind [i].second, NULL);
      for (unsigned i = 0; i < outer_rebind.size (); i++)
        init_var_maps (outer_rebind [i].second, NULL);
    } else if (typeid (c) == typeid (count_gclause)) {
      rchandle<var_expr> var = static_cast<const count_gclause *> (&c)->get_var();
      uint64_t k = (uint64_t) &*var;
      fvar_iter_map.put(k, new vector<ForVarIter_t>());
    } else if (typeid (c) == typeid (orderby_gclause)) {
      const orderby_gclause::rebind_list_t &list = static_cast<const orderby_gclause *> (&c)->get_rebind ();
      for (unsigned i = 0; i < list.size (); i++)
        init_var_maps (list [i].second, NULL);
    }
  }

  return true;
}

struct gflwor_codegen_data {
};

struct window_var_iters {
  vector<ForVarIter_t > *posvar, *curr, *prev, *next;
  vector<ForVarIter_t > *lookup (varref_t var, hash64map<vector<ForVarIter_t> *> &map) {
    vector<ForVarIter_t > *vi = &plan_visitor::no_var_iters;
    if (var != NULL)
      ZORBA_ASSERT (map.get((uint64_t)&*var, vi));
    return vi;
  }
  window_var_iters (flwor_wincond::vars vars, hash64map<vector<ForVarIter_t> *> &map) {
    posvar = lookup (vars.posvar, map);
    curr = lookup (vars.curr, map);
    next = lookup (vars.next, map);
    prev = lookup (vars.prev, map);
  }
};

PlanIter_t gflwor_codegen(gflwor_expr& v, int ccnt, gflwor_codegen_data &gdata) {
#define PREV_ITER gflwor_codegen (v, ccnt - 1, gdata)
  const QueryLoc& qloc = v.get_loc ();
  if (ccnt < 0)
    return new gflwor::TupleSourceIterator(qloc);

  const flwor_clause &c = *(v [ccnt]);
  if (typeid (c) == typeid (where_gclause)) {
    PlanIter_t where = pop_itstack ();
    return new gflwor::WhereIterator(static_cast<const where_gclause *>(&c)->get_loc (), PREV_ITER, where);
  } else if (typeid (c) == typeid (forletwin_gclause)) {
    const forletwin_gclause *flwc = static_cast<const forletwin_gclause *> (&c);
    var_expr* var = flwc->get_var ().getp ();
    PlanIter_t input = pop_itstack ();
    if (flwc->get_type () == forletwin_gclause::for_clause) {
      vector<ForVarIter_t> *var_iters = NULL, *pvar_iters = &no_var_iters;
      var_expr* pos_var = flwc->get_pos_var().getp();
      ZORBA_ASSERT (fvar_iter_map.get((uint64_t)var, var_iters));
      if (pos_var != NULL)
        ZORBA_ASSERT (pvar_iter_map.get((uint64_t) pos_var, pvar_iters));
      if (! flwc->is_outer ()) {
#ifdef ZORBA_DEBUGGER
        if(ccb->m_debugger != 0)
        {
          
            checked_vector<PlanIter_t> argv;
             checked_vector<store::Item_t> varnames;
            checked_vector<string> var_keys;
             checked_vector<global_binding> globals;
             checked_vector<xqtref_t> vartypes;
   
             checked_vector<bound_var> lVars = flwc->get_bound_variables();
             checked_vector<bound_var>::iterator lIter;
   
             for ( lIter = lVars.begin(); lIter != lVars.end(); ++lIter )
             {
               varnames.push_back (lIter->varname);
               var_keys.push_back (lIter->var_key);
               vartypes.push_back (lIter->type);
               lIter->var->accept(*this);
               argv.push_back(pop_itstack());
             }
             reverse(argv.begin(), argv.end());
             list<global_binding> lGlobals = flwc->get_global_variables();
             list<global_binding>::iterator it;
             for ( it = lGlobals.begin(); it != lGlobals.end(); ++it )
             {
               globals.push_back( *it );
             }
             argv.push_back(new gflwor::ForIterator(var->get_loc (), var->get_varname(), PREV_ITER, input, *var_iters, *pvar_iters));
             reverse (argv.begin (), argv.end ());
             return new FnDebugIterator(var->get_loc(), varnames, var_keys, vartypes, globals, argv, true);
      }
#endif
        return new gflwor::ForIterator(var->get_loc (), var->get_varname(), PREV_ITER, input, *var_iters, *pvar_iters);
      } else {
        ZORBA_ASSERT (pos_var == NULL);
        return new gflwor::OuterForIterator(var->get_loc (), var->get_varname(),
                                            PREV_ITER, input, *var_iters);
      }
    } else if (flwc->get_type () == forletwin_gclause::let_clause) {
      vector<LetVarIter_t> *var_iters = NULL;
      ZORBA_ASSERT (lvar_iter_map.get((uint64_t)var, var_iters));
      return new gflwor::LetIterator(var->get_loc (), var->get_varname(), PREV_ITER, input, *var_iters, true);
   } else if (flwc->get_type () == forletwin_gclause::win_clause) {
      vector<LetVarIter_t> *var_iters = NULL;
      ZORBA_ASSERT (lvar_iter_map.get((uint64_t)var, var_iters));
      auto_ptr<gflwor::StartClause> start_clause;
      auto_ptr<gflwor::EndClause> end_clause;
      const flwor_wincond *cond;
      if (NULL == (cond = flwc->get_win_stop ()))
        end_clause.reset (new gflwor::EndClause);
      else {
        window_var_iters wvi_in  (cond->get_in_vars (), fvar_iter_map);
        window_var_iters wvi_out (cond->get_out_vars (), fvar_iter_map);
        gflwor::WindowVars wvars (*wvi_in.curr, *wvi_in.prev, *wvi_in.next, *wvi_in.posvar,
                                  *wvi_out.curr, *wvi_out.prev, *wvi_out.next, *wvi_out.posvar);
        end_clause.reset (new gflwor::EndClause (pop_itstack (), wvars, cond->is_only ()));
      }
      if (NULL != (cond = flwc->get_win_start ())) {
        window_var_iters wvi_in  (cond->get_in_vars (), fvar_iter_map);
        window_var_iters wvi_out (cond->get_out_vars (), fvar_iter_map);
        gflwor::WindowVars wvars (*wvi_in.curr, *wvi_in.prev, *wvi_in.next, *wvi_in.posvar,
                                  *wvi_out.curr, *wvi_out.prev, *wvi_out.next, *wvi_out.posvar);
        start_clause.reset (new gflwor::StartClause (pop_itstack (), wvars));
      }
      return new gflwor::WindowIterator (var->get_loc (), PREV_ITER, input, flwc->get_wintype () == forletwin_gclause::tumbling_window ? gflwor::WindowIterator::TUMBLING : gflwor::WindowIterator::SLIDING, *var_iters, *start_clause, *end_clause);
    }
  } else if (typeid (c) == typeid (count_gclause)) {
    rchandle<var_expr> var = static_cast<const count_gclause *> (&c)->get_var();
    vector<ForVarIter_t> *var_iters = NULL;
    ZORBA_ASSERT( fvar_iter_map.get((uint64_t)&*var, var_iters) );
    return new gflwor::CountIterator(var->get_loc (), var->get_varname (), PREV_ITER, *var_iters);
  } else if (typeid (c) == typeid (orderby_gclause)) {
    vector<ForVarIter_t> theForVariableInput;
    vector<LetVarIter_t> theLetVariableInput;
    vector< vector< ForVarIter_t > > theForVariableOutput;
    vector< vector< LetVarIter_t > > theLetVariableOutput;
    // TODO: collapse sequences of orderby_gclause into ONE OrderByIterator
    const orderby_gclause *obc = static_cast<const orderby_gclause *> (&c);
    vector<gflwor::OrderSpec> lOrderSpecs;
    string col = obc->get_collation ();
    if (col.empty ())
      lOrderSpecs.push_back (gflwor::OrderSpec (pop_itstack (), 
                                                obc->get_empty_mode() == StaticContextConsts::empty_least, 
                                                obc->get_dir() == ParseConstants::dir_descending));
    else
      lOrderSpecs.push_back (gflwor::OrderSpec (pop_itstack (), 
                                                obc->get_empty_mode() == StaticContextConsts::empty_least, 
                                                obc->get_dir() == ParseConstants::dir_descending, 
                                                col));
    
    const orderby_gclause::rebind_list_t &rebind_list = obc->get_rebind ();
    for (unsigned i = 0; i < rebind_list.size (); i++) {
      varref_t inner = rebind_list [i].first, outer = rebind_list [i].second;
      general_var_codegen (*inner);
      PlanIter_t iter = pop_itstack ();
      if (typeid (*iter) == typeid (ForVarIterator)) {
        theForVariableInput.push_back (iter.cast<ForVarIterator> ());
        vector<ForVarIter_t> *var_iters = NULL;
        ZORBA_ASSERT (fvar_iter_map.get((uint64_t) &*outer, var_iters));
        theForVariableOutput.push_back (*var_iters);
      } else {  // let
        theLetVariableInput.push_back (iter.cast<LetVarIterator> ());
        vector<LetVarIter_t> *var_iters = NULL;
        ZORBA_ASSERT (lvar_iter_map.get((uint64_t) &*outer, var_iters));
        theLetVariableOutput.push_back (*var_iters);
      }
    }
    return new gflwor::OrderByIterator (obc->get_loc (), PREV_ITER, lOrderSpecs, theForVariableInput, theLetVariableInput, theForVariableOutput, theLetVariableOutput);
  } else if (typeid (c) == typeid (group_gclause)) {
    const group_gclause *gbc = static_cast<const group_gclause *> (&c);
    const group_gclause::rebind_list_t &inner_rebind = gbc->get_inner_rebind (), &outer_rebind = gbc->get_outer_rebind ();
    const vector<string> &collations = gbc->getCollations ();
    vector<gflwor::GroupingSpec> gspecs;
    vector<gflwor::GroupingOuterVar> gouters;
    
    for (unsigned i = 0; i < collations.size (); i++) {
      vector<ForVarIter_t> *var_iters = NULL;
      ZORBA_ASSERT (fvar_iter_map.get((uint64_t) &*inner_rebind [i].second, var_iters));
      general_var_codegen (*inner_rebind [i].first);
      gspecs.push_back (gflwor::GroupingSpec (pop_itstack (), *var_iters, collations [i]));
    }

    for (unsigned i = 0; i < outer_rebind.size (); i++) {
      vector<LetVarIter_t> *var_iters = NULL;
      ZORBA_ASSERT (lvar_iter_map.get((uint64_t) &*outer_rebind [i].second, var_iters));
      general_var_codegen (*outer_rebind [i].first);
      gouters.push_back (gflwor::GroupingOuterVar (pop_itstack (), *var_iters));
    }
    return new gflwor::GroupByIterator (gbc->get_loc (), PREV_ITER, gspecs, gouters);
  }
  ZORBA_ASSERT (false);
  return NULL;
#undef PREV_ITER
}

void end_visit (gflwor_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t ret = pop_itstack ();
  gflwor_codegen_data gdata;
  PlanIter_t gflwor = gflwor_codegen (v, v.size () - 1, gdata);
  itstack.push (new gflwor::TupleStreamIterator(qloc, gflwor, ret, false));
}

bool begin_visit (promote_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (promote_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t lChild = pop_itstack();
  // TODO: Currently we use cast. Promotion may be more efficient.
  itstack.push(new PromoteIterator(qloc, lChild, v.get_target_type()));
}

bool begin_visit (trycatch_expr& v) {
  CODEGEN_TRACE_IN("");
  for(int i = v.clause_count() - 1; i >= 0; --i) {
    catch_clause *cc = &*v[i];
    if (cc->get_errorcode_var_h() != NULL) {
      catchvar_iter_map.put((uint64_t)&*cc->get_errorcode_var_h(), new vector<LetVarIter_t>());
    }
    if (cc->get_errordesc_var_h() != NULL) {
      catchvar_iter_map.put((uint64_t)&*cc->get_errordesc_var_h(), new vector<LetVarIter_t>());
    }
    if (cc->get_errorobj_var_h() != NULL) {
      catchvar_iter_map.put((uint64_t)&*cc->get_errorobj_var_h(), new vector<LetVarIter_t>());
    }
  }
  return true;
}

void end_visit (trycatch_expr& v) {
  CODEGEN_TRACE_OUT("");
  vector<LetVarIter_t> *vec = NULL;
  vector<TryCatchIterator::CatchClause> rev_ccs;
  for(int i = v.clause_count() - 1; i >= 0; --i) {
    catch_clause *cc = &*v[i];
    TryCatchIterator::CatchClause rcc;
    rcc.node_name = cc->get_nametest_h();
    rcc.catch_expr = pop_itstack();
    if (cc->get_errorcode_var_h() != NULL) {
      bool bound = catchvar_iter_map.get((uint64_t)&*cc->get_errorcode_var_h(), vec);
      ZORBA_ASSERT(bound);
      rcc.errorcode_var = *vec;
    }
    if (cc->get_errordesc_var_h() != NULL) {
      bool bound = catchvar_iter_map.get((uint64_t)&*cc->get_errordesc_var_h(), vec);
      ZORBA_ASSERT(bound);
      rcc.errordesc_var = *vec;
    }
    if (cc->get_errorobj_var_h() != NULL) {
      bool bound = catchvar_iter_map.get((uint64_t)&*cc->get_errorobj_var_h(), vec);
      ZORBA_ASSERT(bound);
      rcc.errorobj_var = *vec;
    }
    rev_ccs.push_back(rcc);
  }
  vector<TryCatchIterator::CatchClause> ccs(rev_ccs.rbegin(), rev_ccs.rend());
  PlanIter_t lChild = pop_itstack();
  itstack.push(new TryCatchIterator(qloc, lChild, ccs));
}

bool begin_visit (eval_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (eval_expr& v) {
  CODEGEN_TRACE_OUT("");
  checked_vector<PlanIter_t> argv;
  checked_vector<store::Item_t> varnames;
  checked_vector<string> var_keys;
  checked_vector<xqtref_t> vartypes;
  for (unsigned i = 0; i < v.var_count (); i++) {
    varnames.push_back (v.var_at (i).varname);
    var_keys.push_back (v.var_at (i).var_key);
    argv.push_back (pop_itstack ());
  }
  argv.push_back (pop_itstack ());
  reverse (argv.begin (), argv.end ());
  itstack.push (new EvalIterator (qloc, varnames, var_keys, vartypes, argv));
}

bool begin_visit (typeswitch_expr& v) {
  CODEGEN_TRACE_IN("");
  ZORBA_NOT_IMPLEMENTED ("typeswitch codegen");
  return true;
}

void end_visit (typeswitch_expr& v) {
  CODEGEN_TRACE_OUT("");
}


bool begin_visit (if_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}


void end_visit (if_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t iterElse = pop_itstack();
  PlanIter_t iterThen = pop_itstack();
  PlanIter_t iterCond = pop_itstack();
  PlanIter_t iterIfThenElse = new IfThenElseIterator(
    qloc, iterCond, iterThen, iterElse, v.isUpdating());
  itstack.push(&*iterIfThenElse);
}


bool begin_visit (insert_expr& v) {
  CODEGEN_TRACE_IN("");

  expr_t targetExpr = v.getTargetExpr();
  xqtref_t targetType = targetExpr->return_type(ccb->m_sctx);

  if (v.getType() == store::UpdateConsts::AFTER ||
      v.getType() == store::UpdateConsts::BEFORE)
  {
    if (TypeOps::is_equal(*targetType, *GENV_TYPESYSTEM.UNTYPED_TYPE) ||
        TypeOps::is_subtype(*GENV_TYPESYSTEM.ELEMENT_TYPE_ONE, *targetType) ||
        TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.ELEMENT_TYPE_ONE) ||
        TypeOps::is_subtype(*GENV_TYPESYSTEM.TEXT_TYPE_ONE, *targetType) ||
        TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.TEXT_TYPE_ONE) ||
        TypeOps::is_subtype(*GENV_TYPESYSTEM.PI_TYPE_ONE, *targetType) ||
        TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.PI_TYPE_ONE) ||
        TypeOps::is_subtype(*GENV_TYPESYSTEM.COMMENT_TYPE_ONE, *targetType) ||
        TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.COMMENT_TYPE_ONE))
      return true;

    if (TypeOps::is_equal(*targetType, *GENV_TYPESYSTEM.EMPTY_TYPE))
      ZORBA_ERROR_LOC(XUDY0027, qloc);
    else
      ZORBA_ERROR_LOC(XUTY0006, qloc);
  } else {
    if (TypeOps::is_equal(*targetType, *GENV_TYPESYSTEM.UNTYPED_TYPE) ||
        TypeOps::is_subtype(*GENV_TYPESYSTEM.ELEMENT_TYPE_ONE, *targetType) ||
        TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.ELEMENT_TYPE_ONE) ||
        TypeOps::is_subtype(*GENV_TYPESYSTEM.DOCUMENT_TYPE_ONE, *targetType) ||
        TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.DOCUMENT_TYPE_ONE))
      return true;

    if (TypeOps::is_equal(*targetType, *GENV_TYPESYSTEM.EMPTY_TYPE))
      ZORBA_ERROR_LOC(XUDY0027, qloc);
    else
      ZORBA_ERROR_LOC(XUTY0005, qloc);
  }

  return true;
}

void end_visit (insert_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t lTarget = pop_itstack();
  PlanIter_t lSource = pop_itstack();
  PlanIter_t lInsert = new InsertIterator(qloc, v.getType(), lSource, lTarget); 
  itstack.push(&*lInsert);
}

bool begin_visit (delete_expr& v) {
  CODEGEN_TRACE_IN("");

  expr_t targetExpr = v.getTargetExpr();
  xqtref_t targetType = targetExpr->return_type(ccb->m_sctx);

  if (TypeOps::is_equal(*targetType, *GENV_TYPESYSTEM.UNTYPED_TYPE) ||
      TypeOps::is_subtype(*GENV_TYPESYSTEM.ANY_NODE_TYPE_STAR, *targetType) ||
      TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.ANY_NODE_TYPE_STAR))
    return true;

  ZORBA_ERROR_LOC(XUTY0007, qloc);

  return true;
}

void end_visit (delete_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t lTarget = pop_itstack();
  PlanIter_t lDelete = new DeleteIterator(qloc, lTarget);
  itstack.push(&*lDelete);
}

bool begin_visit (replace_expr& v) {
  CODEGEN_TRACE_IN("");

  expr_t targetExpr = v.getTargetExpr();
  xqtref_t targetType = targetExpr->return_type(ccb->m_sctx);

  if (TypeOps::is_equal(*targetType, *GENV_TYPESYSTEM.UNTYPED_TYPE) ||
      TypeOps::is_subtype(*GENV_TYPESYSTEM.ANY_NODE_TYPE_ONE, *targetType) ||
      TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.ANY_NODE_TYPE_ONE))
    return true;

  if (TypeOps::is_equal(*targetType, *GENV_TYPESYSTEM.EMPTY_TYPE))
    ZORBA_ERROR_LOC(XUDY0027, qloc);
  else
    ZORBA_ERROR_LOC(XUTY0008, qloc);

  return true;
}

void end_visit (replace_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t lReplacement = pop_itstack();
  PlanIter_t lTarget = pop_itstack();
  PlanIter_t lReplace = new ReplaceIterator(qloc,
                                            v.getType(),
                                            lTarget,
                                            lReplacement);
  itstack.push(&*lReplace);
}

bool begin_visit (rename_expr& v) {
  CODEGEN_TRACE_IN("");

  expr_t targetExpr = v.getTargetExpr();
  xqtref_t targetType = targetExpr->return_type(ccb->m_sctx);

  if (TypeOps::is_equal(*targetType, *GENV_TYPESYSTEM.UNTYPED_TYPE) ||
      TypeOps::is_subtype(*GENV_TYPESYSTEM.ELEMENT_TYPE_ONE, *targetType) ||
      TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.ELEMENT_TYPE_ONE) ||
      TypeOps::is_subtype(*GENV_TYPESYSTEM.ATTRIBUTE_TYPE_ONE, *targetType) ||
      TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.ATTRIBUTE_TYPE_ONE) ||
      TypeOps::is_subtype(*GENV_TYPESYSTEM.PI_TYPE_ONE, *targetType) ||
      TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.PI_TYPE_ONE))
    return true;

  if (TypeOps::is_equal(*targetType, *GENV_TYPESYSTEM.EMPTY_TYPE))
    ZORBA_ERROR_LOC(XUDY0027, qloc);
  else
    ZORBA_ERROR_LOC(XUTY0012, qloc);

  return true;
}

void end_visit (rename_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t lName = pop_itstack();
  PlanIter_t lTarget = pop_itstack();
  PlanIter_t lRename = new RenameIterator(qloc, lTarget, lName);
  itstack.push(&*lRename);
}

bool begin_visit (transform_expr& v) {
  CODEGEN_TRACE_IN("");

  vector<rchandle<copy_clause> >::const_iterator lIter = v.begin();
  vector<rchandle<copy_clause> >::const_iterator lEnd  = v.end();
  for (; lIter != lEnd; ++lIter)
  {
    rchandle<var_expr> var = (*lIter)->getVar();
    uint64_t k = (uint64_t) &*var;
    copy_var_iter_map.put(k, new vector<ForVarIter_t>());
  }
  return true;
}

void end_visit (transform_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t lReturn = pop_itstack();
  PlanIter_t lModify = pop_itstack();

  vector<CopyClause> lClauses;
  stack<PlanIter_t> lInputs;
  size_t lSize = v.size();
  for (size_t i = 0; i < lSize; ++i)
  {
    lInputs.push(pop_itstack());  
  }

  vector<rchandle<copy_clause> >::const_iterator lIter = v.begin();
  vector<rchandle<copy_clause> >::const_iterator lEnd  = v.end();
  for(;lIter!=lEnd;++lIter)
  {
    PlanIter_t lInput = pop_stack(lInputs);
    vector<ForVarIter_t>* lVarIters = 0;
    var_expr* lVar = (*lIter)->getVar();
    ZORBA_ASSERT(copy_var_iter_map.get((uint64_t)lVar, lVarIters));
    lClauses.push_back(CopyClause (*lVarIters, lInput));
  }

  TransformIterator* lTransform = new TransformIterator(qloc, lClauses, lModify, lReturn); 
  itstack.push(lTransform);
}


bool begin_visit (exit_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (exit_expr& v) {
  CODEGEN_TRACE_OUT("");
  checked_vector<PlanIter_t> argv;
  argv.push_back (pop_itstack ());
  itstack.push (new FlowCtlIterator (qloc, argv, FlowCtlIterator::EXIT));
}

bool begin_visit (flowctl_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (flowctl_expr& v) {
  CODEGEN_TRACE_OUT("");
  enum FlowCtlIterator::action a;
  switch (v.get_action ()) {
  case flowctl_expr::BREAK:
    a = FlowCtlIterator::BREAK;
    break;
  case flowctl_expr::CONTINUE:
    a = FlowCtlIterator::CONTINUE;
    break;
  default:
    ZORBA_FATAL(false, "");
  }
  checked_vector<PlanIter_t> argv;
  itstack.push (new FlowCtlIterator (qloc, argv, a));
}

bool begin_visit (while_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (while_expr& v) {
  CODEGEN_TRACE_OUT("");
  checked_vector<PlanIter_t> argv;
  argv.push_back (pop_itstack ());
  itstack.push (new LoopIterator (qloc, argv));
}


bool begin_visit (fo_expr& v) {
  CODEGEN_TRACE_IN ("");

  // If the function is an enclosed expression, push it in the constructors
  // stack to "hide" the current constructor context, if any. This way, a new
  // constructor context can be started if a node constructor exists inside
  // the enclosed expr.
  if (is_enclosed_expr(&v))
    theConstructorsStack.push(&v);

	return true;
}


void end_visit (fo_expr& v) {
  CODEGEN_TRACE_OUT("");

  const function* func = v.get_func();
  ZORBA_ASSERT (func != NULL);

  vector<PlanIter_t> argv (v.size ());
  generate (argv.rbegin (), argv.rend (), stack_to_generator (itstack));

  const QueryLoc& loc = qloc;

  if (func->validate_args (argv)) {
    PlanIter_t iter = func->codegen (loc, argv, v);
    ZORBA_ASSERT(iter != NULL);
    itstack.push(iter);

    if (is_enclosed_expr(&v)) {
      expr *e = pop_stack(theConstructorsStack);
      ZORBA_ASSERT(e == &v);

      if (!theAttrContentStack.empty() && theAttrContentStack.top() == true)
        dynamic_cast<EnclosedIterator*>(iter.getp())->setAttrContent();
    }
  } else {
    ZORBA_ERROR_LOC_PARAM(XPST0017, loc,
                          func->get_signature().get_name()->getStringValue(),
                          argv.size());
  }
}


bool begin_visit (ft_select_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

bool begin_visit (ft_contains_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

bool begin_visit (instanceof_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (instanceof_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t p = pop_itstack ();
  itstack.push (new InstanceOfIterator (qloc, p, v.get_target_type ()));
}

bool begin_visit (treat_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (treat_expr& v) {
  CODEGEN_TRACE_OUT("");
  vector<PlanIter_t> argv;
  argv.push_back (pop_itstack ());
  itstack.push (new TreatIterator (qloc, argv, v.get_target_type (), v.get_check_prime (), v.get_err ()));
}

bool begin_visit (castable_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (castable_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t lChild = pop_itstack();
  itstack.push(new CastableIterator(qloc, lChild, v.get_target_type()));
}

bool begin_visit (cast_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (cast_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t lChild = pop_itstack();
  itstack.push(new CastIterator(qloc, lChild, v.get_target_type()));
}

bool begin_visit (name_cast_expr& v) {
  CODEGEN_TRACE_IN("");

  expr_t targetExpr = v.get_input();
  xqtref_t targetType = targetExpr->return_type(ccb->m_sctx);

  if (TypeOps::is_subtype(*GENV_TYPESYSTEM.UNTYPED_ATOMIC_TYPE_ONE, *targetType) ||
      TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.UNTYPED_ATOMIC_TYPE_ONE) ||
      TypeOps::is_subtype(*GENV_TYPESYSTEM.QNAME_TYPE_ONE, *targetType) ||
      TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.QNAME_TYPE_ONE) ||
      TypeOps::is_subtype(*GENV_TYPESYSTEM.NCNAME_TYPE_ONE, *targetType) ||
      TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.NCNAME_TYPE_ONE) ||
      TypeOps::is_subtype(*GENV_TYPESYSTEM.STRING_TYPE_ONE, *targetType) ||
      TypeOps::is_subtype(*targetType, *GENV_TYPESYSTEM.STRING_TYPE_ONE))
    return true;

  ZORBA_ERROR_LOC(XPTY0004, qloc);

  return true;
}

void end_visit (name_cast_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t lChild = pop_itstack();
  itstack.push(new NameCastIterator(qloc, lChild, v.getNamespaceContext()));
}


bool begin_visit (validate_expr& v) 
{
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (validate_expr& v) 
{
  CODEGEN_TRACE_OUT("");

#ifndef ZORBA_NO_XMLSCHEMA
  PlanIter_t lChild = pop_itstack();

  itstack.push (new ValidateIterator (qloc,
                                      lChild,
                                      v.get_typemgr(),
									  v.get_type_name(),
                                      v.get_valmode()));
#else
  //no schema support
  ZORBA_ERROR_LOC(XQST0009, qloc);
#endif
}


bool begin_visit (extension_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}


#ifdef PATH_ITERATOR

/*******************************************************************************

********************************************************************************/
bool begin_visit (relpath_expr& v) {
  CODEGEN_TRACE_IN("");

  //PlanIter_t input = pop_itstack();
  //ZORBA_ASSERT(input != NULL);
  //PlanIter_t pathIte(new PathIterator(qloc, input));
  //itstack.push(pathIte);

  return true;
}


void end_visit (relpath_expr& v) {
  CODEGEN_TRACE_OUT("");
}


/*******************************************************************************

********************************************************************************/
bool begin_visit (axis_step_expr& v) {
  CODEGEN_TRACE_IN("");

  store::ItemFactory& factory = *(GENV.getStore().getItemFactory());
  store::Item_t qname;

  PlanIter_t& ite = peek_stack(itstack);
  PathIterator* pathIte = dynamic_cast<PathIterator*>(ite.getp());

  if (pathIte == NULL) {
    PlanIter_t inputStep = pop_itstack();
    pathIte = new PathIterator(qloc, inputStep);
    itstack.push(pathIte);
  }
  //ZORBA_ASSERT(pathIte != NULL);

  rchandle<match_expr> testExpr = v.getTest();

  NodePredicate* prd = new NodePredicate();

  prd->setTestKind(testExpr->getTestKind());

  if (testExpr->getTestKind() == match_name_test) {
    if (v.getAxis() == axis_kind_attribute)
      prd->setNodeKind(store::StoreConsts::attributeNode);
    else
      prd->setNodeKind(store::StoreConsts::elementNode);

    match_wild_t wildKind = testExpr->getWildKind();
    prd->setWildKind(wildKind);

    if (wildKind == match_no_wild) {
      prd->setQName(testExpr->getQName());
    }
    else if (wildKind == match_prefix_wild) {
      factory.createQName(qname, "", "*", testExpr->getWildName().c_str());

      prd->setQName(qname);
    }
    else if (wildKind == match_name_wild) {
      prd->setQName(testExpr->getQName());
    }
  } else {
    prd->setDocTestKind(testExpr->getDocTestKind());
    prd->setNodeKind(testExpr->getNodeKind());
    prd->setQName(testExpr->getQName());
    store::Item *typeName = testExpr->getTypeName();
    if (typeName != NULL) {
      prd->setType(ccb->m_sctx->get_typemanager()->create_named_type(typeName));
    }
    prd->setNilledAllowed(testExpr->getNilledAllowed());
  }

  pathIte->addStep(v.getAxis(), prd);

  return true;
}


void end_visit (axis_step_expr& v) {
  CODEGEN_TRACE_OUT("");
}


bool begin_visit (match_expr& v) {
  CODEGEN_TRACE_IN("");

  return true;
}


void end_visit (match_expr& v) {
  CODEGEN_TRACE_OUT("");
}


#else

/*******************************************************************************

********************************************************************************/
bool begin_visit (relpath_expr& v) {
  CODEGEN_TRACE_IN("");
  // Done in axis itself
  return true;
}


void end_visit (relpath_expr& v) {
  CODEGEN_TRACE_OUT("");
}


/*******************************************************************************

********************************************************************************/
bool begin_visit (axis_step_expr& v) {
  CODEGEN_TRACE_IN("");

  bool result = true;

  PlanIter_t input = pop_itstack();

  ZORBA_ASSERT(input != NULL);

  PlanIter_t axisIte;

  switch (v.getAxis())
  {
  case axis_kind_self:
  {
    axisIte = new SelfAxisIterator(qloc, input);
    break;
  }
  case axis_kind_child:
  {
    axisIte = new ChildAxisIterator(qloc, input);
    break;
  }
  case axis_kind_parent:
  {
    axisIte = new ParentAxisIterator(qloc, input);
    break;
  }
  case axis_kind_descendant:
  {
    axisIte = new DescendantAxisIterator(qloc, input);
    break;
  }
  case axis_kind_descendant_or_self:
  {
    axisIte = new DescendantSelfAxisIterator(qloc, input);
    break;
  }
  case axis_kind_ancestor:
  {
    axisIte = new AncestorAxisIterator(qloc, input);
    break;
  }
  case axis_kind_ancestor_or_self:
  {
    axisIte = new AncestorSelfAxisIterator(qloc, input);
    break;
  }
  case axis_kind_following_sibling:
  {
    axisIte = new RSiblingAxisIterator(qloc, input);
    break;
  }
  case axis_kind_following:
  {
    axisIte = new FollowingAxisIterator(qloc, input);
    break;
  }
  case axis_kind_preceding_sibling:
  {
    axisIte = new LSiblingAxisIterator(qloc, input);
    break;
  }
  case axis_kind_preceding:
  {
    axisIte = new PrecedingAxisIterator(qloc, input);
    break;
  }
  case axis_kind_attribute:
  {
    if (v.getTest()->getTestKind() == match_name_test ||
        v.getTest()->getTestKind() == match_attr_test ||
        v.getTest()->getTestKind() == match_xs_attr_test ||
        v.getTest()->getTestKind() == match_anykind_test)
    {
      axisIte = new AttributeAxisIterator(qloc, input);
    } else {
      axisIte = new EmptyIterator(qloc);
      result = false;
    }

    break;
  }
  default:
    ZORBA_ASSERT (false && "Unknown axis kind");
  }

  itstack.push(axisIte);

  return result;
}


void end_visit (axis_step_expr& v) {
  CODEGEN_TRACE_OUT("");
}


/*******************************************************************************

********************************************************************************/
bool begin_visit (match_expr& v) {
  CODEGEN_TRACE_IN ("");

  PlanIter_t axisIte = pop_itstack();
  AxisIteratorHelper* axisItep = dynamic_cast<AxisIteratorHelper*>(axisIte.getp());
  ZORBA_ASSERT(axisItep != NULL);

  store::Item_t qname;

  store::ItemFactory& iFactory = *(GENV.getStore().getItemFactory());

  if (v.getTestKind() == match_name_test) {
    match_wild_t wildKind = v.getWildKind();

    axisItep->setTestKind(match_name_test);
 
    if (dynamic_cast<AttributeAxisIterator*>(axisIte.getp()) != NULL)
      axisItep->setNodeKind(store::StoreConsts::attributeNode);
    else
      axisItep->setNodeKind(store::StoreConsts::elementNode);

    axisItep->setWildKind(wildKind);

    if (wildKind == match_no_wild) {
      axisItep->setQName(v.getQName());
    }
    else if (wildKind == match_prefix_wild) {
      iFactory.createQName(qname, "", "wildcard", v.getWildName().c_str());

      axisItep->setQName(qname);
    }
    else if (wildKind == match_name_wild) {
      axisItep->setQName(v.getQName());
    }
  } else {
    axisItep->setTestKind(v.getTestKind());
    axisItep->setDocTestKind(v.getDocTestKind());
    axisItep->setNodeKind(v.getNodeKind());
    axisItep->setQName(v.getQName());
    store::Item *typeName = v.getTypeName();
    if (typeName != NULL) {
      axisItep->setType(ccb->m_sctx->get_typemanager()->create_named_type(typeName));
    }
    axisItep->setNilledAllowed(v.getNilledAllowed());
  }

  itstack.push(axisIte);
  return true;
}


void end_visit (match_expr& v) {
  CODEGEN_TRACE_OUT("");
}

#endif // PATH_ITER


/*******************************************************************************

  Node Constructors

********************************************************************************/

bool begin_visit (doc_expr& v) {
  CODEGEN_TRACE_IN("");

  theConstructorsStack.push(&v);
  theAttrContentStack.push(false);

  return true;
}


void end_visit (doc_expr& v) {
  CODEGEN_TRACE_OUT("");
  
  PlanIter_t lContent = pop_itstack();
  PlanIter_t lContIter = new DocumentContentIterator(qloc, lContent);
  PlanIter_t lDocIter = new DocumentIterator(qloc, lContIter);
  itstack.push(lDocIter);

  theAttrContentStack.pop();
  expr* e = pop_stack(theConstructorsStack);
  ZORBA_ASSERT(e == &v);
}


bool begin_visit (elem_expr& v) {
  CODEGEN_TRACE_IN ("");

  theConstructorsStack.push(&v);
  theAttrContentStack.push(false);

  return true;
}


void end_visit (elem_expr& v) {
  CODEGEN_TRACE_OUT ("");

  PlanIter_t lQNameIter = 0;
  PlanIter_t lContentIter = 0;
  PlanIter_t lAttrsIter = 0;

  if ( v.getContent() != 0 )
    lContentIter = pop_itstack();

  if ( v.getAttrs() != 0 )
    lAttrsIter = pop_itstack();

  lQNameIter = pop_itstack();

  bool isRoot = false;
  theAttrContentStack.pop();
  expr* e = pop_stack(theConstructorsStack);
  ZORBA_ASSERT(e == &v);
  if (theConstructorsStack.empty() || is_enclosed_expr(theConstructorsStack.top())) {
    isRoot = true;
  }

	PlanIter_t iter = new ElementIterator(qloc,
                                        lQNameIter,
                                        lAttrsIter,
                                        lContentIter,
                                        v.getNSCtx().getp(),
                                        isRoot);
  itstack.push(iter);
}


bool begin_visit (attr_expr& v) {
  CODEGEN_TRACE_IN("");

  theConstructorsStack.push(&v);
  theAttrContentStack.push(true);

	return true;
}


void end_visit (attr_expr& v) {
  CODEGEN_TRACE_OUT("");

  PlanIter_t lQNameIter = 0;
  PlanIter_t lVarIter = 0;
  PlanIter_t lContentIter = 0;
  
  if (v.getValueExpr() != 0) {
    lVarIter = pop_itstack();
  } else {
    lVarIter = new EmptyIterator(qloc);
  }
  
  PlanIter_t lAttrIter = 0;
  lQNameIter = pop_itstack();
  
  bool isRoot = false;
  theAttrContentStack.pop();
  expr* e = pop_stack(theConstructorsStack);
  ZORBA_ASSERT(e = &v);
  if (theConstructorsStack.empty() || is_enclosed_expr(theConstructorsStack.top())) {
    isRoot = true;
  }

  lAttrIter = new AttributeIterator(qloc, lQNameIter, lVarIter, isRoot);
  
  itstack.push(lAttrIter);
}


bool begin_visit (text_expr& v) {
  CODEGEN_TRACE_IN ("");

  theConstructorsStack.push(&v);
  theAttrContentStack.push(true);

	return true;
}


void end_visit (text_expr& v) {
  CODEGEN_TRACE_OUT ("");

  PlanIter_t content = pop_itstack ();

  bool isRoot = false;
  theAttrContentStack.pop();
  expr* e = pop_stack(theConstructorsStack);
  ZORBA_ASSERT(e = &v);
  if (theConstructorsStack.empty() || is_enclosed_expr(theConstructorsStack.top())) {
    isRoot = true;
  }

  switch (v.get_type ()) {
  case text_expr::text_constructor:
    itstack.push (new TextIterator(qloc, content, isRoot));
    break;

  case text_expr::comment_constructor:
    itstack.push (new CommentIterator (qloc, content, isRoot));
    break;

  default:
    break;
  }
}


bool begin_visit (pi_expr& v) {
  CODEGEN_TRACE_IN("");

  theConstructorsStack.push(&v);
  theAttrContentStack.push(true);

  return true;
}


void end_visit (pi_expr& v) {
  CODEGEN_TRACE_OUT("");

  bool isRoot = false;
  theAttrContentStack.pop();
  expr* e = pop_stack(theConstructorsStack);
  ZORBA_ASSERT(e = &v);
  if (theConstructorsStack.empty() || is_enclosed_expr(theConstructorsStack.top())) {
    isRoot = true;
  }

  PlanIter_t content = pop_itstack ();
  PlanIter_t target = pop_itstack ();
  itstack.push (new PiIterator (qloc, target, content, isRoot));
}


bool begin_visit(function_def_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (function_def_expr &v) {
  CODEGEN_TRACE_OUT("");
}


bool begin_visit (const_expr& v) {
  CODEGEN_TRACE_IN ("");
  return true;
}

void end_visit (const_expr& v) {
  CODEGEN_TRACE_OUT("");
  PlanIter_t it = new SingletonIterator (qloc, v.get_val ());
  itstack.push (it);
}


bool begin_visit (order_expr& v) {
  CODEGEN_TRACE_IN("");
  return true;
}

void end_visit (order_expr& v) {
  CODEGEN_TRACE_OUT("");
}


/*******************************************************************************


********************************************************************************/

/*..........................................
 :  end visit                              :
 :.........................................*/

void end_visit (ft_select_expr& v) {
  CODEGEN_TRACE_OUT("");
}

void end_visit (ft_contains_expr& v) {
  CODEGEN_TRACE_OUT("");
}

void end_visit (extension_expr& v) {
  CODEGEN_TRACE_OUT("");
}

PlanIter_t result()
{
    PlanIter_t res = pop_itstack();
    ZORBA_ASSERT(itstack.empty());
    return res;
}


};



/*******************************************************************************

********************************************************************************/
PlanIter_t codegen(
    const char *descr,
    expr *root,
    CompilerCB *ccb,
    hash64map<vector<LetVarIter_t> *> *param_var_map)
{
  plan_visitor c(ccb, param_var_map);
  root->accept (c);
  PlanIter_t result = c.result();

  // HACK: print only main query if no-tree-id's is active.
  // When that happens, we are comparing iterator plans.
  if (result != NULL && descr != NULL && Properties::instance()->printIteratorTree()
      && (xqp_string ("main query") == descr || ! Properties::instance()->noTreeIds())) 
  {
    cout << "Iterator tree for " << descr << ":\n";
    XMLIterPrinter vp (cout);
    print_iter_plan (vp, result);
    cout << endl;
  }
	
  return result;
}

vector<ForVarIter_t> plan_visitor::no_var_iters;


} /* namespace zorba */
