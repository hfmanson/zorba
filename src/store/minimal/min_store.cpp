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

#include <climits>

#include "zorbautils/hashfun.h"
#include "zorbautils/fatal.h"
#include "zorbatypes/rchandle.h"
#include "zorbaerrors/error_manager.h"
#include "zorbaerrors/Assert.h"

#include "store/util/hashmap_stringp.h"

#include "store/api/pul.h"

#include "store/minimal/min_store.h"
#include "store/minimal/min_temp_seq.h"
#include "store/minimal/min_lazy_temp_seq.h"
#include "store/minimal/min_collection.h"
#include "store/minimal/min_loader.h"
#include "store/minimal/min_store_defs.h"
#include "store/minimal/min_node_items.h"
#include "store/minimal/min_atomic_items.h"
#include "store/minimal/min_node_iterators.h"
#include "store/minimal/min_item_factory.h"
#include "store/minimal/min_query_context.h"
#include "store/minimal/qname_pool.h"
#include "store/minimal/string_pool.h"
#include "store/minimal/min_iterator_factory.h"
#include "store/minimal/min_query_context.h"
#include "store/minimal/min_item_iterator.h"


namespace zorba 
{ 

namespace storeminimal 
{

typedef rchandle<store::TempSeq> TempSeq_t;

const ulong SimpleStore::NAMESPACE_POOL_SIZE = 32;//128;
const ulong SimpleStore::DEFAULT_COLLECTION_MAP_SIZE = 32;

const char* SimpleStore::XS_URI = "http://www.w3.org/2001/XMLSchema";
const char* SimpleStore::XML_URI = "http://www.w3.org/2001/XML/1998/namespace";
const char* SimpleStore::ZXSE_URI = "http://www.zorba-xquery.com/zorba/schema-extensions";

const ulong SimpleStore::XML_URI_LEN = sizeof(SimpleStore::XML_URI);

/*******************************************************************************

********************************************************************************/
SimpleStore*
SimpleStore::getInstance()
{
  static SimpleStore lInstance;

  if ( ! lInstance.theIsInitialized )
    lInstance.init();

  return &lInstance;
}
/*******************************************************************************

********************************************************************************/
SimpleStore::SimpleStore()
  :
  theIsInitialized(false),
  theUriCounter(0),
  theTreeCounter(1),
  theNamespacePool(NULL),
  theQNamePool(NULL),
  theItemFactory(NULL),
  theIteratorFactory(NULL),
  theDocuments(DEFAULT_COLLECTION_MAP_SIZE, true),
  theCollections(DEFAULT_COLLECTION_MAP_SIZE, true),
  theTraceLevel(0)
{
}


/*******************************************************************************

********************************************************************************/
void SimpleStore::init()
{
  if (!theIsInitialized)
  {
    theUriCounter = 0;
    theTreeCounter = 1;

    theNamespacePool = new StringPool(NAMESPACE_POOL_SIZE);

    theNamespacePool->insertc("", theEmptyNs);
    theNamespacePool->insertc(XS_URI, theXmlSchemaNs);

    theQNamePool = new QNamePool(QNamePool::MAX_CACHE_SIZE, theNamespacePool);

    initTypeNames();

    theItemFactory = new BasicItemFactory(theNamespacePool, theQNamePool);

    theIteratorFactory = new SimpleIteratorFactory();

    theTraceLevel = 0;//store::Properties::instance()->storeTraceLevel();

    theIsInitialized = true;
  }
}


/*******************************************************************************

********************************************************************************/
void SimpleStore::initTypeNames()
{
  theSchemaTypeNames.resize(XS_LAST);

  theSchemaTypeNames[0]  = theQNamePool->insert(XS_URI, "", "untyped");
  theSchemaTypeNames[1]  = theQNamePool->insert(XS_URI, "", "untypedAtomic");
  theSchemaTypeNames[2]  = theQNamePool->insert(XS_URI, "", "anyType");
  theSchemaTypeNames[3]  = theQNamePool->insert(XS_URI, "", "anySimpleType");
  theSchemaTypeNames[4]  = theQNamePool->insert(XS_URI, "", "anyAtomicType");

  theSchemaTypeNames[5]  = theQNamePool->insert(XS_URI, "", "string");
  theSchemaTypeNames[6]  = theQNamePool->insert(XS_URI, "", "normalizedString");
  theSchemaTypeNames[7]  = theQNamePool->insert(XS_URI, "", "language");
  theSchemaTypeNames[8]  = theQNamePool->insert(XS_URI, "", "token");
  theSchemaTypeNames[9]  = theQNamePool->insert(XS_URI, "", "NMTOKEN");

  theSchemaTypeNames[10] = theQNamePool->insert(XS_URI, "", "anyURI");
  theSchemaTypeNames[11] = theQNamePool->insert(XS_URI, "", "Name");
  theSchemaTypeNames[12] = theQNamePool->insert(XS_URI, "", "NCName");
  theSchemaTypeNames[13] = theQNamePool->insert(XS_URI, "", "QName");
  theSchemaTypeNames[14] = theQNamePool->insert(XS_URI, "", "notation");

  theSchemaTypeNames[15] = theQNamePool->insert(XS_URI, "", "ID");
  theSchemaTypeNames[16] = theQNamePool->insert(XS_URI, "", "IDREF");

  theSchemaTypeNames[17] = theQNamePool->insert(XS_URI, "", "ENTITY");

  theSchemaTypeNames[18] = theQNamePool->insert(XS_URI, "", "dateTime");
  theSchemaTypeNames[19] = theQNamePool->insert(XS_URI, "", "date");
  theSchemaTypeNames[20] = theQNamePool->insert(XS_URI, "", "time");
  theSchemaTypeNames[21] = theQNamePool->insert(XS_URI, "", "duration");
  theSchemaTypeNames[22] = theQNamePool->insert(XS_URI, "", "dayTimeDuration");
  theSchemaTypeNames[23] = theQNamePool->insert(XS_URI, "", "yearMonthDuration");

  theSchemaTypeNames[24] = theQNamePool->insert(XS_URI, "", "float");
  theSchemaTypeNames[25] = theQNamePool->insert(XS_URI, "", "double");
  theSchemaTypeNames[26] = theQNamePool->insert(XS_URI, "", "decimal");
  theSchemaTypeNames[27] = theQNamePool->insert(XS_URI, "", "integer");
  theSchemaTypeNames[28] = theQNamePool->insert(XS_URI, "", "nonPositiveInteger");
  theSchemaTypeNames[29] = theQNamePool->insert(XS_URI, "", "nonNegativeInteger");
  theSchemaTypeNames[30] = theQNamePool->insert(XS_URI, "", "negativeInteger");
  theSchemaTypeNames[31] = theQNamePool->insert(XS_URI, "", "positiveInteger");

  theSchemaTypeNames[32] = theQNamePool->insert(XS_URI, "", "long");
  theSchemaTypeNames[33] = theQNamePool->insert(XS_URI, "", "int");
  theSchemaTypeNames[34] = theQNamePool->insert(XS_URI, "", "short");
  theSchemaTypeNames[35] = theQNamePool->insert(XS_URI, "", "byte");
  theSchemaTypeNames[36] = theQNamePool->insert(XS_URI, "", "unsignedLong");
  theSchemaTypeNames[37] = theQNamePool->insert(XS_URI, "", "unsignedInt");
  theSchemaTypeNames[38] = theQNamePool->insert(XS_URI, "", "unsignedShort");
  theSchemaTypeNames[39] = theQNamePool->insert(XS_URI, "", "unsignedByte");

  theSchemaTypeNames[40] = theQNamePool->insert(XS_URI, "", "gYearMonth");
  theSchemaTypeNames[41] = theQNamePool->insert(XS_URI, "", "gYear");
  theSchemaTypeNames[42] = theQNamePool->insert(XS_URI, "", "gMonthDay");
  theSchemaTypeNames[43] = theQNamePool->insert(XS_URI, "", "gDay");
  theSchemaTypeNames[44] = theQNamePool->insert(XS_URI, "", "gMonth");

  theSchemaTypeNames[45] = theQNamePool->insert(XS_URI, "", "base64Binary");
  theSchemaTypeNames[46] = theQNamePool->insert(XS_URI, "", "hexBinary");
  theSchemaTypeNames[47] = theQNamePool->insert(XS_URI, "", "boolean");
  theSchemaTypeNames[48] = theQNamePool->insert(ZXSE_URI, "zxse", "tuple");

}

/*******************************************************************************

********************************************************************************/
SimpleStore::~SimpleStore()
{
  shutdown();
}


/*******************************************************************************

********************************************************************************/
void SimpleStore::shutdown()
{
  theItemUris.clear();

  theCollections.clear();

  theDocuments.clear();

  if (theItemFactory != NULL)
  {
    delete theItemFactory;
    theItemFactory = NULL;
  }

  if (theQNamePool != NULL)
  {
    ulong numTypes = theSchemaTypeNames.size();
    for (ulong i = 0; i < numTypes; i++)
      theSchemaTypeNames[i] = NULL;

    delete theQNamePool;
    theQNamePool = NULL;
  }

  if (theNamespacePool != NULL)
  {
    theEmptyNs = NULL;
    theXmlSchemaNs = NULL;

    delete theNamespacePool;
    theNamespacePool = NULL;
  }

  if (theIteratorFactory != NULL)
  {
    delete theIteratorFactory;
    theIteratorFactory = NULL;
  }

  theIsInitialized = false;
}


/*******************************************************************************

********************************************************************************/
ulong SimpleStore::getTreeId()
{
  SYNC_CODE(AutoMutex lock(&theTreeCounterMutex);)
  return theTreeCounter++;
}


/*******************************************************************************

********************************************************************************/
XmlLoader* SimpleStore::getXmlLoader(error::ErrorManager* aErrorManager)
{
  return new XmlLoader(theItemFactory, aErrorManager);
}


/*******************************************************************************
  Create an internal URI and return an rchandle to it. 
********************************************************************************/
store::Item_t SimpleStore::createUri()
{
  SYNC_CODE(theUriCounterMutex.lock();)
  std::ostringstream uristream;
  uristream << "zorba://internalURI-" << SimpleStore::theUriCounter++;
  SYNC_CODE(theUriCounterMutex.unlock();)

  store::Item_t val;
  theItemFactory->createAnyURI(val, uristream.str().c_str());
  return val;
}


/*******************************************************************************
  Create an index with a given URI and return an rchandle to the index object. 
  If an index with the given URI exists already and the index we want to create
  is not a temporary one, raise an error.
********************************************************************************/
store::Index_t SimpleStore::createIndex(
                  const xqpStringStore_t& uri,
                  const store::IndexSpecification& spec)
{
  ZORBA_ERROR(XQP0015_SYSTEM_NOT_YET_IMPLEMENTED);
  return NULL;
}

store::Index *SimpleStore::getIndex(const xqpStringStore_t& uri)
{
  if (uri == NULL)
    return NULL;

  ZORBA_ERROR(XQP0015_SYSTEM_NOT_YET_IMPLEMENTED);
}

/*******************************************************************************

********************************************************************************/
void SimpleStore::deleteIndex(const xqpStringStore_t& uri)
{
  if (uri == NULL)
    return;

  ZORBA_ERROR(XQP0015_SYSTEM_NOT_YET_IMPLEMENTED);
}

/*******************************************************************************
  Create a collection with a given URI and return an rchandle to the new
  collection object. If a collection with the given URI exists already, return
  NULL and register an error.
********************************************************************************/
store::Collection_t SimpleStore::createCollection(const xqpStringStore_t& uri)
{
  if (uri == NULL)
    return NULL;

  store::Item_t uriItem;
  xqpStringStore_t  tmpuri(uri.getp());
  theItemFactory->createAnyURI(uriItem, tmpuri);
  theItemUris.push_back(uriItem);

  store::Collection_t collection(new SimpleCollection(uriItem));

  const xqpStringStore* urip = collection->getUri()->getStringValueP();
  bool inserted = theCollections.insert(urip, collection);

  if (!inserted)
  {
    theItemUris.erase(theItemUris.end() - 1);
    ZORBA_ERROR_PARAM(API0005_COLLECTION_ALREADY_EXISTS, uri->c_str(), "");
    return NULL;
  }

  return collection;
}


/*******************************************************************************
  Create a collection in the store. The collection will be assigned an internal
  URI by the store.
********************************************************************************/
store::Collection_t SimpleStore::createCollection()
{
  store::Item_t uriItem = createUri();

  xqpStringStore_t uriStr = uriItem->getStringValue();

  return createCollection(uriStr);
}


/*******************************************************************************
  Return an rchandle to the Collection object corresponding to the given URI,
  or NULL if there is no collection with that URI.
********************************************************************************/
store::Collection_t SimpleStore::getCollection(const xqpStringStore_t& uri)
{
  if (uri == NULL)
    return NULL;

  store::Collection_t collection;
  if (theCollections.get(uri, collection) )
    return collection.getp();
  else
  {
    return NULL;
  }
}


/*******************************************************************************
  Delete the collection with the given URI. If there is no collection with
  that URI, this method is a NOOP.
********************************************************************************/
void SimpleStore::deleteCollection(const xqpStringStore_t& uri)
{
  if (uri == NULL)
    return;

  bool deleted = theCollections.remove(uri);

  if(deleted )
  {
    checked_vector<store::Item_t>::iterator it = theItemUris.begin();
    checked_vector<store::Item_t>::iterator end = theItemUris.end();

    for (; it != end; ++it)
    {
      if( (*it)->getStringValue()->compare(uri) == 0 )
      {
        theItemUris.erase(it);
        break;
      }
    }
  }
  else
    ZORBA_ERROR_PARAM(API0006_COLLECTION_NOT_FOUND, uri->c_str(), "");
}

/*******************************************************************************
  Resturn an iterator that lists the URI's of all the available collections.
********************************************************************************/
store::Iterator_t SimpleStore::listCollectionUris()
{
  return new ItemIterator(theItemUris, false);
}


/*******************************************************************************

********************************************************************************/
store::Item_t SimpleStore::loadDocument(
            const xqpStringStore_t& uri, 
            std::istream& stream,
            bool storeDocument)
{
  if (uri == NULL)
    return NULL;

  const xqpStringStore* urip = uri.getp();

  XmlNode_t root;
  bool found = theDocuments.get(urip, root);

  if (found)
  {
    //daniel heDocuments.remove(uri);
    return root.getp();
  }

  error::ErrorManager lErrorManager;
  //std::auto_ptr<XmlLoader> loader(getXmlLoader(&lErrorManager));
  XmlLoader_t   loader = new XmlLoader(theItemFactory, &lErrorManager);

  root = loader->loadXml(uri, stream);
  if (lErrorManager.hasErrors()) 
  {
    ZORBA_ERROR_PARAM(lErrorManager.getErrors().front().theErrorCode,
                      lErrorManager.getErrors().front().theDescription, "");
  }

  if (root != NULL && storeDocument)
    theDocuments.insert(urip, root);

  return root.getp();
}

/*******************************************************************************
For lazy loading...
Param stream is a heap pointer to an input stream. This is to be deallocated by Zorba.
********************************************************************************/
store::Item_t SimpleStore::loadDocument(
            const xqpStringStore_t& uri, 
            std::istream *stream,
            bool storeDocument)

{
  if (uri == NULL)
    return NULL;

  const xqpStringStore* urip = uri.getp();

  XmlNode_t root;
  bool found = theDocuments.get(urip, root);

  if (found)
    return root.getp();

  error::ErrorManager lErrorManager;
  //std::auto_ptr<XmlLoader> loader(getXmlLoader(&lErrorManager));
  XmlLoader_t   loader = new XmlLoader(theItemFactory, &lErrorManager);

  root = loader->startloadXml(uri, stream);
  if (lErrorManager.hasErrors()) 
  {
    ZORBA_ERROR_PARAM(lErrorManager.getErrors().front().theErrorCode,
                      lErrorManager.getErrors().front().theDescription, "");
  }

  if (root != NULL && storeDocument)
    theDocuments.insert(urip, root);

  return root.getp();
}

/*******************************************************************************
  Add the given node with the given uri to the store. Essentially, this method
  establishes an association between a uri and a node. If the given uri is 
  already associated to another node, the method raises an error. If the given
  uri is already associated to the given node, this method is a noop.
********************************************************************************/
void SimpleStore::addNode(const xqpStringStore_t& uri, const store::Item_t& node)
{
  ZORBA_ASSERT(uri != NULL);

	if(node == NULL || !node->isNode())
  {
    ZORBA_ERROR_PARAM( API0021_ITEM_TO_LOAD_IS_NOT_NODE, uri, "");
  }

  XmlNode_t root = reinterpret_cast<XmlNode*>(node.getp());
  bool inserted = theDocuments.insert(uri, root);

  if (!inserted && node.getp() != root.getp())
  {
    ZORBA_ERROR_PARAM(API0020_DOCUMENT_ALREADY_EXISTS, uri, "");
  }
  else if (inserted)
  {
    root->setDocUri(uri);
  }

  ZORBA_FATAL(node.getp() == root.getp(), "");
}


/*******************************************************************************
  Return an rchandle to the root node of the document corresponding to the given
  URI, or NULL if there is no document with that URI.
********************************************************************************/
store::Item_t SimpleStore::getDocument(const xqpStringStore_t& uri)
{
  if (uri == NULL)
    return NULL;

  XmlNode_t root;
  bool found = theDocuments.get(uri, root);
  if (found)
    return root.getp();

  return NULL;
}


/*******************************************************************************
  Delete the document with the given URI. If there is no document with that
  URI, this method is a NOOP.
********************************************************************************/
void SimpleStore::deleteDocument(const xqpStringStore_t& uri)
{
  if (uri == NULL)
    return;

  theDocuments.remove(uri);
}


/*******************************************************************************
  Delete all the documents.
********************************************************************************/
void SimpleStore::deleteAllDocuments()
{
  theDocuments.clear();
}



/*******************************************************************************
  Compare two nodes, based on their node id. Return -1 if node1 < node2, 0, if
  node1 == node2, or 1 if node1 > node2.
********************************************************************************/
short SimpleStore::compareNodes(store::Item* node1, store::Item* node2) const
{
  ZORBA_FATAL(node1->isNode(), "");
  ZORBA_FATAL(node2->isNode(), "");

  if (node1 == node2)
    return 0;

  XmlNode* n1 = static_cast<XmlNode*>(node1);
  XmlNode* n2 = static_cast<XmlNode*>(node2);

  
  if ((n1->getTreeId() < n2->getTreeId()) ||
      ((n1->getTreeId() == n2->getTreeId()) && (n1->getOrdPath() < n2->getOrdPath())))
    return -1;

  return 1;
}


/*******************************************************************************
  Sorts the items of the passed iterator

  @param iterator to sort
  @param ascendent true for ascendent and false for descendant
  @param duplicate duplicate elemination should be applied
  @return iterator which produces the sorted items
********************************************************************************/
store::Iterator_t SimpleStore::sortNodes(
    store::Iterator* input,
    bool ascendent,
    bool duplicateElemination,
    bool aAllowAtomics)
{
  if (aAllowAtomics)
    return new StoreNodeSortOrAtomicIterator(input, ascendent, duplicateElemination);
  else
    return  new StoreNodeSortIterator(input, ascendent, duplicateElemination);
}


/*******************************************************************************
  Create an iterator that eliminates the duplicate nodes in the set of items
  which is produced by the passed iterator
********************************************************************************/
store::Iterator_t SimpleStore::distinctNodes(store::Iterator* input, bool aAllowAtomics)
{
  if (aAllowAtomics)
    return new StoreNodeDistinctOrAtomicIterator(input);
  else
    return new StoreNodeDistinctIterator(input);
}


/*******************************************************************************
  Computes the URI for the given node.
********************************************************************************/
bool SimpleStore::getReference(store::Item_t& result, const store::Item* node)
{
  std::ostringstream stream;

  const XmlNode* n = reinterpret_cast<const XmlNode*>(node);

  if (n->getNodeKind() == store::StoreConsts::attributeNode)
  {
    stream << "zorba://node_reference/" << n->getTreeId() << "/a/"
           << n->getOrdPath().serialize();
  }
  else
  {
    stream << "zorba://node_reference/" << n->getTreeId() << "/c/"
           << n->getOrdPath().serialize();
  }

  xqpStringStore_t str(new xqpStringStore(stream.str()));

  result = new AnyUriItemImpl(str);
  return true;
}


/*******************************************************************************
  Returns Item which is identified by a reference

  @param uri Has to be an xs:URI item
  @returns referenced item if it exists, otherwise NULL
********************************************************************************/
bool SimpleStore::getNodeByReference(store::Item_t& result, const store::Item* uri)
{
  xqpStringStore* str = uri->getStringValueP();

  ulong prefixlen = strlen("zorba://node_reference/");

  if (strncmp(str->c_str(), "zorba://node_reference/", prefixlen))
    ZORBA_ERROR_PARAM_OSS(API0028_INVALID_NODE_URI, str->c_str(), "");

  //
  // Decode tree id
  //
  const char* start = str->c_str() + prefixlen;
  char* next = const_cast<char*>(start);

  long tmp = 0;
  tmp = strtol(start, &next, 10);

  if (tmp <= 0 || tmp == LONG_MAX)
    ZORBA_ERROR_PARAM_OSS(API0028_INVALID_NODE_URI, str->c_str(), "");

  start = next;

  if (*start != '/')
    ZORBA_ERROR_PARAM_OSS(API0028_INVALID_NODE_URI, str->c_str(), "");

  ulong treeid = (ulong)tmp;

  //
  // Check if the uri specifies attribute node or not
  //
  bool attributeNode;

  start++;
  if (*start == 'a')
    attributeNode = true;
  else if (*start == 'c')
    attributeNode = false;
  else
    ZORBA_ERROR_PARAM_OSS(API0028_INVALID_NODE_URI, str->c_str(), "");

  start++;
  if (*start != '/')
    ZORBA_ERROR_PARAM_OSS(API0028_INVALID_NODE_URI, str->c_str(), "");

  start++;

  //
  // Search for the tree
  //
  XmlNode* rootNode = NULL;
  DocumentSet::iterator it = theDocuments.begin();
  DocumentSet::iterator end = theDocuments.end();

  for (; it != end; ++it)
  {
    rootNode = (*it).second.getp();

    if (rootNode->getTreeId() == treeid)
      break;
  }

  if (rootNode == NULL)
  {
    CollectionSet::iterator it = theCollections.begin();
    CollectionSet::iterator end = theCollections.end();

    for (; it != end; ++it)
    {
      store::Collection_t col = (*it).second.getp();

      store::Iterator_t colIter = col->getIterator(true);

      colIter->open();

      store::Item_t rootItem;
      if (!colIter->next(rootItem)) 
      {
        rootItem = NULL;
      }

      while (rootItem != NULL)
      {
        rootNode = BASE_NODE(rootItem);
        if (rootNode->getTreeId() == treeid)
          break;

        if (!colIter->next(rootItem))
          break;
      }

      colIter->close();

      if (rootNode != NULL)
        break;
    }

    if (rootNode == NULL) 
    {
      result = NULL;
      return false;
    }
  }

  //
  // Search for node in the tree
  //
  
  OrdPath op((unsigned char*)start, strlen(start));

  if (rootNode->getOrdPath() == op) 
  {
    result = rootNode;
    return true;
  }

  XmlNode* parent = rootNode;

  while (1)
  {
    ulong i;

    if (parent->getNodeKind() != store::StoreConsts::documentNode &&
        parent->getNodeKind() != store::StoreConsts::elementNode)
    {
      result = NULL;
      return false;
    }

    if (attributeNode && parent->getNodeKind() == store::StoreConsts::elementNode)
    {
      ElementNode* elemParent = reinterpret_cast<ElementNode*>(parent);

      ulong numAttrs = elemParent->numAttributes();
      for (i = 0; i < numAttrs; i++)
      {
        XmlNode* child = elemParent->getAttr(i);

        OrdPath::RelativePosition pos =  child->getOrdPath().getRelativePosition(op);

        if (pos == OrdPath::SELF)
        {
          result = child;
          return true;
        }
        else if (pos == OrdPath::DESCENDANT)
        {
          parent = child;
          break;
        }
        else if (pos !=  OrdPath::FOLLOWING)
        {
          result = NULL;
          return false;
        }
      }

    }
    InternalNode* parent2 = reinterpret_cast<InternalNode*>(parent);

    ulong numChildren = parent2->numChildren();
    for (i = 0; i < numChildren; i++)
    {
      XmlNode* child = parent2->getChild(i);

      OrdPath::RelativePosition pos =  child->getOrdPath().getRelativePosition(op);

      if (pos == OrdPath::SELF)
      {
        result = child;
        return true;
      }
      else if (pos == OrdPath::DESCENDANT)
      {
        parent = child;
        break;
      }
      else if (pos !=  OrdPath::FOLLOWING)
      {
        result = NULL;
        return false;
      }
    }

    if (i == numChildren)
    {
      result = NULL;
      return false;
    }
  }
}

/*******************************************************************************

********************************************************************************/
bool SimpleStore::getPathInfo(
  const store::Item*               docUri,
  std::vector<const store::Item*>& contextPath,
  std::vector<const store::Item*>& relativePath,
  bool                             isAttrPath,
  bool&                            found,
  bool&                            unique)
{
  ZORBA_ERROR( XQP0015_SYSTEM_NOT_YET_IMPLEMENTED );
}


/*******************************************************************************
  Creates a new TempSeq. The instance can be used, e.g. for variable bindings

  @param iterator   The source for the XMDInstance
  @param copyNodes  If true, all nodes are copied before they are saved in the
                    temp sequence.
  @param lazy		    Hint for the store. If possible a XMDInstance should be
                    evaluated lazily. For XQueryP it might be necassary to set
                    this to false.
********************************************************************************/
TempSeq_t SimpleStore::createTempSeq(
  store::Iterator* iterator,
  bool copyNodes, 
  bool lazy)
{
  TempSeq_t tempSeq;
  if(lazy){
    tempSeq = new SimpleTempSeq(iterator, copyNodes);
    //tempSeq = new SimpleLazyTempSeq(iterator, copyNodes);
  }else{
    tempSeq = new SimpleTempSeq(iterator, copyNodes);
  }
  return tempSeq;
}


/*******************************************************************************
  Creates an empty TempSeq.
********************************************************************************/
TempSeq_t SimpleStore::createTempSeq()
{
  TempSeq_t tempSeq = new SimpleTempSeq();
  return tempSeq;
}

/*******************************************************************************
  Creates a temp seq initialized by the given vector.
  @param item_v - The vector to use to initialize the seq.
********************************************************************************/
TempSeq_t SimpleStore::createTempSeq(const std::vector<store::Item_t>& item_v)
{
  TempSeq_t tempSeq = new SimpleTempSeq(item_v);
  return tempSeq;
}


} // namespace storeminimal
} // namespace zorba
