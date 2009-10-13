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
#ifndef ZORBA_RUNTIME_MISC_IMPL_H
#define ZORBA_RUNTIME_MISC_IMPL_H
 
#include "common/shared_types.h"

#include "runtime/base/narybase.h"
#include "runtime/api/plan_iterator_wrapper.h"
#include "runtime/util/flowctl_exception.h"


namespace zorba {
// 3 The Error Function
NARY_ITER(FnErrorIterator);


// 8.1 fn:resolve-uri
NARY_ITER(FnResolveUriIterator);


class SequentialIterator : public NaryBaseIterator<SequentialIterator, PlanIteratorState >
{
private:
  bool theUpdating;

public:
  SERIALIZABLE_CLASS(SequentialIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(
  SequentialIterator,
  NaryBaseIterator<SequentialIterator, PlanIteratorState >);

  void serialize(::zorba::serialization::Archiver &ar)
  {
    serialize_baseclass(ar,
    (NaryBaseIterator<SequentialIterator, PlanIteratorState >*)this);

    ar & theUpdating;
  }

public:
  SequentialIterator(
        static_context* sctx,
        const QueryLoc& loc,
        std::vector<PlanIter_t>& aChildren,
        bool aUpdating) 
    :
    NaryBaseIterator<SequentialIterator, PlanIteratorState>(sctx, loc, aChildren),
    theUpdating(aUpdating)
  {}


  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;

  bool isUpdating() const { return theUpdating; }
};


class FlowCtlIterator : public NaryBaseIterator<FlowCtlIterator, PlanIteratorState>
{
private:
  enum FlowCtlException::action act;

public:
  SERIALIZABLE_CLASS(FlowCtlIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(
  FlowCtlIterator,
  NaryBaseIterator<FlowCtlIterator, PlanIteratorState >);

  void serialize(::zorba::serialization::Archiver &ar);

public:
  FlowCtlIterator(
        static_context* sctx,
        const QueryLoc& loc,
        std::vector<PlanIter_t>& aChildren,
        enum FlowCtlException::action act_)
    :
    NaryBaseIterator<FlowCtlIterator, PlanIteratorState>(sctx, loc, aChildren),
    act (act_)
  {}

  void accept(PlanIterVisitor& v) const;

  bool nextImpl(store::Item_t& result, PlanState& aPlanState) const; 
};


NARY_ITER (LoopIterator);


NARY_ITER (FnReadStringIterator);


class FnPrintIterator : public NaryBaseIterator<FnPrintIterator, PlanIteratorState>
{
private:
	bool m_printToConsole;

public:
  SERIALIZABLE_CLASS(FnPrintIterator);

  SERIALIZABLE_CLASS_CONSTRUCTOR2T(
  FnPrintIterator,
  NaryBaseIterator<FnPrintIterator, PlanIteratorState >);

  void serialize(::zorba::serialization::Archiver& ar)
  {
    serialize_baseclass(ar,
    (NaryBaseIterator<FnPrintIterator, PlanIteratorState >*)this);

    ar & m_printToConsole;
  }

public:
	FnPrintIterator(
        static_context* sctx,
        const QueryLoc& loc,
        std::vector<PlanIter_t>& aChildren,
        bool printToConsole = true)
    :
	  NaryBaseIterator<FnPrintIterator, PlanIteratorState >(sctx, loc, aChildren),
    m_printToConsole(printToConsole)
  {
  }

  void accept(PlanIterVisitor& v) const;

	bool nextImpl(store::Item_t& result, PlanState& aPlanState) const;
};

} /* namespace zorba */

#endif /* ZORBA_MISC_IMPL_H */

/*
 * Local variables:
 * mode: c++
 * End:
 */
