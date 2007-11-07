/*
 *	Copyright 2006-2007 FLWOR Foundation.
 *  Author: David Graf (david.graf@28msec.com)
 *
 */

#ifndef XQP_SIMPLE_STORE_H
#define XQP_SIMPLE_STORE_H

#include "store/api/store.h"
#include "store/naive/basic_item_factory.h"

namespace xqp
{
template <class Object> class rchandle;

typedef rchandle<class Iterator> Iterator_t;
typedef rchandle<class Item> Item_t;
typedef rchandle<class Collection> Collection_t;
typedef rchandle<class TempSeq> TempSeq_t;
typedef rchandle<class PUL> PUL_t;

class Timetravel;
class Requester;

class SimpleStore : public Store
{
  friend class Store;

protected:
  BasicItemFactory  theItemFactory;

private:
  SimpleStore();
  virtual ~SimpleStore();

public:
  void init() { }

  void deinit() { }

  virtual ItemFactory& getItemFactory() { return theItemFactory; }

  virtual TempSeq_t createTempSeq();
  virtual TempSeq_t createTempSeq(Iterator_t iterator, bool lazy = true);

  virtual void setGarbageCollectionStrategy(const xqp_string& strategy);
  virtual void apply(PUL_t pendingUpdateList);
  virtual void apply(PUL_t pendingUpdateList, Requester requester);

  virtual Item_t getReference(Item_t);
  virtual Item_t getFixedReference(Item_t, Requester requester, Timetravel timetravel);
  virtual Item_t getNodeByReference(Item_t);
  virtual Item_t getNodeByReference(Item_t, Requester requester, Timetravel timetravel);

  virtual int32_t compare(Item_t item1, Item_t item2);
  virtual Iterator_t sort(Iterator_t iterator, bool ascendent, bool duplicateElemination);
  virtual Iterator_t distinctNodeStable(Iterator_t);

  virtual Collection_t getCollection(Item_t uri);
  virtual Collection_t createCollection(Item_t uri);
  virtual Collection_t createCollection();
  virtual void deleteCollection(Item_t uri);
  virtual Item_t createURI();
  
}; /* class SimpleStore */

} /* namespace xqp */

#endif /* XQP_SIMPLE_STORE_H */
