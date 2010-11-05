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

#include <stack>
#include <memory>

#include <zorba/config.h>

#include "zorbaerrors/Assert.h"
#include "zorbaerrors/error_manager.h"
#include "zorbatypes/URI.h"
#ifndef ZORBA_NO_FULL_TEXT
#include "zorbautils/tokenizer.h"
#endif /* ZORBA_NO_FULL_TEXT */

#include "store/api/copymode.h"
#include "store/naive/atomic_items.h"
#include "store/naive/node_items.h"
#include "store/naive/node_iterators.h"
#include "store/naive/simple_store.h"
#include "store/naive/simple_collection.h"
#include "store/naive/simple_item_factory.h"
#include "store/naive/qname_pool.h"
#include "store/naive/store_defs.h"
#include "store/naive/nsbindings.h"
#include "store/naive/item_iterator.h"
#include "store/naive/dataguide.h"
#include "store/naive/node_factory.h"

#include "util/string_util.h"


namespace zorba { namespace simplestore {


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  XmlTree                                                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************

********************************************************************************/
XmlTree::XmlTree()
{
}


XmlTree::XmlTree(XmlNode* root, ulong id)
  :
  theRefCount(0),
  theId(id),
  thePos(0),
  theCollection(NULL),
  theRootNode(root),
  theDataGuideRootNode(NULL),
  theIsValidated(false),
  theIsRecursive(false)
{
}


/*******************************************************************************

********************************************************************************/
void XmlTree::setCollection(SimpleCollection* collection, ulong pos)
{
  ZORBA_ASSERT(collection == NULL || theCollection == NULL);

  theCollection = collection;
  thePos = pos;

  if (collection != NULL)
    theId = collection->createTreeId();
}


/*******************************************************************************

********************************************************************************/
ulong XmlTree::getCollectionId() const
{
  if (theCollection != NULL)
    return theCollection->getId();
  else
    return 0;
}


/*******************************************************************************

********************************************************************************/
void XmlTree::free() throw()
{
  // std::cout << "Deleting Xml Tree: " << this << std::endl;

  if (theRootNode != 0)
  {
    theRootNode->destroy();
    theRootNode = NULL;
  }

  if (theDataGuideRootNode != NULL)
  {
    theDataGuideRootNode->deleteTree();
    theDataGuideRootNode = NULL;
  }

  delete this;
}


/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class XmlNode                                                              //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
  Create a new node C within a given tree T and compute its ordpath based on its
  given position among the attributes or children of a given node P, who will
  become the parent of C.

  If P is NULL, C becomes the root (and single node) of T. In this case, C's
  ordpath is the root one (= 1).

  If P != NULL and pos >= 0, C will become the "pos"-th child/attribute of P.
  In this case, the ordpath of C is computed based on the ordpaths of its left
  and right siblings (if any). Note: If "pos" >= P->numChildren, then C will
  be appended to P's children/attributes.

  If P != NULL and "pos" < 0, C will be appended to P's children/attributes,
  and C's ordpath is computed based on the current number of children/attributes
  in P. This ordpath evaluation assumes that P never had a child that was later
  deleted. The assumption holds because this case is used only during the
  evaluation of a node-constructor expression, or during the copying of a
  subtree S, and if the node to be created is not the copy of the root of S.
********************************************************************************/
XmlNode::XmlNode(
    XmlTree* tree,
    InternalNode* parent,
    long pos,
    store::StoreConsts::NodeKind nodeKind)
  :
  theParent(parent),
  theFlags(0)
{
  assert(tree || parent);
  assert(parent == NULL || parent->getTree() != NULL);
  assert(tree == NULL || parent == NULL || parent->getTree() == tree);

  theFlags = (uint32_t)nodeKind;

  if (parent == NULL)
  {
    setTree(tree);
    tree->setRoot(this);
    theOrdPath.setAsRoot();

    if (nodeKind != store::StoreConsts::documentNode)
      theOrdPath.appendComp(1);
  }
  else
  {
    setTree(parent->getTree());
    setOrdPath(parent, pos, nodeKind);
  }

#ifndef ZORBA_NO_FULL_TEXT
  initTokens();
#endif /* ZORBA_NO_FULL_TEXT */
}


/*******************************************************************************

********************************************************************************/
#ifndef NDEBUG
XmlNode::~XmlNode()
{
  NODE_TRACE1("Deleted " << store::StoreConsts::toString(getNodeKind()) << this);
}
#endif


/*******************************************************************************

********************************************************************************/
void XmlNode::setTree(const XmlTree* t)
{
  theTreeRCPtr = (long*)t;
  SYNC_CODE(theRCLockPtr = &(t->getRCLock());)
}


/*******************************************************************************

********************************************************************************/
void XmlNode::setOrdPath(
    InternalNode* parent,
    long pos,
    store::StoreConsts::NodeKind nodeKind)
{
  if (parent == NULL)
  {
    theOrdPath.setAsRoot();
    return;
  }

  if (!parent->theOrdPath.isValid())
    return;

  ulong numChildren = parent->numChildren();
  ulong numAttrs = 0;
  ElementNode* elemParent = NULL;

  if (parent->getNodeKind() == store::StoreConsts::elementNode)
  {
    elemParent = reinterpret_cast<ElementNode*>(parent);
    numAttrs =  elemParent->numAttributes();
  }

  if (nodeKind == store::StoreConsts::attributeNode)
  {
    if (pos < 0)
    {
      theOrdPath = parent->theOrdPath;
      theOrdPath.appendComp(2 * numAttrs + 1);
    }
    else if (numAttrs > 0)
    {
      ulong upos = (ulong)pos;

      if (upos == 0)
      {
        OrdPath::insertBefore(elemParent->theOrdPath,
                              elemParent->getAttr(0)->theOrdPath,
                              theOrdPath);
      }
      else if (upos == numAttrs && numChildren > 0)
      {
        OrdPath::insertInto(elemParent->theOrdPath,
                            elemParent->getAttr(upos-1)->theOrdPath,
                            elemParent->getChild(0)->theOrdPath,
                            theOrdPath);
      }
      else if (upos >= numAttrs)
      {
        OrdPath::insertAfter(elemParent->theOrdPath,
                             elemParent->getAttr(upos-1)->theOrdPath,
                             theOrdPath);
      }
      else
      {
        OrdPath::insertInto(elemParent->theOrdPath,
                            elemParent->getAttr(upos-1)->theOrdPath,
                            elemParent->getAttr(upos)->theOrdPath,
                            theOrdPath);
      }
    }
    else if (numChildren > 0)
    {
      OrdPath::insertBefore(elemParent->theOrdPath,
                            elemParent->getChild(0)->getOrdPath(),
                            theOrdPath);
    }
    else
    {
      theOrdPath = elemParent->theOrdPath;
      theOrdPath.appendComp(1);
    }
  }
  else if (elemParent != NULL) // not attribute node with element parent
  {
    if (pos < 0)
    {
      theOrdPath = elemParent->theOrdPath;
      theOrdPath.appendComp(2 * (numAttrs + numChildren) + 1);
    }
    else if (numChildren > 0)
    {
      ulong upos = (ulong)pos;

      if (upos == 0 && numAttrs > 0)
      {
        OrdPath::insertInto(elemParent->theOrdPath,
                            elemParent->getAttr(numAttrs-1)->theOrdPath,
                            elemParent->getChild(0)->theOrdPath,
                            theOrdPath);
      }
      else if (upos == 0)
      {
        OrdPath::insertBefore(elemParent->theOrdPath,
                              elemParent->getChild(0)->theOrdPath,
                              theOrdPath);
      }
      else if (upos >= numChildren)
      {
        OrdPath::insertAfter(elemParent->theOrdPath,
                             elemParent->getChild(numChildren-1)->theOrdPath,
                             theOrdPath);
      }
      else
      {
        OrdPath::insertInto(elemParent->theOrdPath,
                            elemParent->getChild(upos-1)->theOrdPath,
                            elemParent->getChild(upos)->theOrdPath,
                            theOrdPath);
      }
    }
    else if (numAttrs > 0)
    {
      OrdPath::insertAfter(elemParent->theOrdPath,
                           elemParent->getAttr(numAttrs-1)->theOrdPath,
                           theOrdPath);
    }
    else
    {
      theOrdPath = elemParent->theOrdPath;
      theOrdPath.appendComp(1);
    }
  }
  else // not attribute node with document parent
  {
    if (pos < 0)
    {
      theOrdPath = parent->theOrdPath;
      theOrdPath.appendComp(2 * (numAttrs + numChildren) + 1);
    }
    else if (numChildren > 0)
    {
      ulong upos = (ulong)pos;

      if (upos == 0)
      {
        OrdPath::insertBefore(parent->theOrdPath,
                              parent->getChild(0)->theOrdPath,
                              theOrdPath);
      }
      else if (upos >= numChildren)
      {
        OrdPath::insertAfter(parent->theOrdPath,
                             parent->getChild(numChildren-1)->theOrdPath,
                             theOrdPath);
      }
      else
      {
        OrdPath::insertInto(parent->theOrdPath,
                            parent->getChild(upos-1)->theOrdPath,
                            parent->getChild(upos)->theOrdPath,
                            theOrdPath);
      }
    }
    else
    {
      theOrdPath = parent->theOrdPath;
      theOrdPath.appendComp(1);
    }
  }
}


/*******************************************************************************

********************************************************************************/
void XmlNode::setId(XmlTree* tree, const OrdPathStack* op)
{
  ZORBA_ASSERT(getTree() == NULL);

  setTree(tree);

  if (op != NULL)
    theOrdPath = *op;
  else
    theOrdPath.setAsRoot();
}


/*******************************************************************************
  Return a hash value based on the id of the node.
********************************************************************************/
uint32_t XmlNode::hash(long timezone, const XQPCollator* aCollation) const
{
  ulong tid = getTree()->getId();

  return hashfun::h32((void*)(&tid), sizeof(ulong), theOrdPath.hash());
}


/*******************************************************************************

********************************************************************************/
store::Item_t XmlNode::getEBV() const
{
  store::Item_t bVal;
  GET_FACTORY().createBoolean(bVal, true);
  return bVal;
}


/*******************************************************************************
  Document and element methods redefine this method.
********************************************************************************/
void XmlNode::getBaseURIInternal(zstring& uri, bool& local) const
{
  local = false;

  if (theParent)
    theParent->getBaseURI(uri);
}


/*******************************************************************************
  Make a copy of the xml tree rooted at this node and place the copied tree at
  a given position under a given node. Return a pointer to the root node of the
  copied tree.

  parent   : The node P under which the copied tree is to be placed. P may be
             NULL, in which case the copied tree becomes a new standalone tree.
  pos      : The position under P where the copied tree is to be placed. If
             "this" is an attribute node, pos is a position among the attributes
             of P; otherwise it is a position among the children of P. If pos
             is greater or equal to the current number of attrs/children in P,
             then the copied tree is appended to P's attributes/children.
  copymode : Encapsulates the construction-mode and copy-namespace-mode
             components of the query's static context.
********************************************************************************/
store::Item* XmlNode::copy(
    store::Item* inParent,
    long pos,
    const store::CopyMode& copymode) const
{
  InternalNode* parent = NULL;

  if (inParent)
  {
    parent = reinterpret_cast<InternalNode*>(inParent);
    ZORBA_ASSERT(inParent->getNodeKind() == store::StoreConsts::elementNode ||
                 inParent->getNodeKind() == store::StoreConsts::documentNode);
  }

  if (getNodeKind() == store::StoreConsts::attributeNode)
  {
    if (parent)
    {
      ElementNode* pnode = reinterpret_cast<ElementNode*>(parent);
      pnode->checkUniqueAttr(getNodeName());
    }
  }

  return copyInternal(parent, parent, (long)pos, NULL, copymode);
}


/*******************************************************************************
  Connect "this" to the given parent at the given position.
********************************************************************************/
void XmlNode::connect(InternalNode* parent, ulong pos) throw()
{
  ZORBA_FATAL(theParent == NULL, "");

  theParent = parent;

  if (getNodeKind() == store::StoreConsts::attributeNode)
  {
    parent->attributes().insert(this, pos);
  }
  else
  {
    parent->children().insert(this, pos);
  }
}


/*******************************************************************************
  Disconnect "this" node from its parent, if any.
********************************************************************************/
long XmlNode::disconnect() throw()
{
  if (theParent == NULL)
    return -1;

  try
  {
    ulong pos;

    if (getNodeKind() == store::StoreConsts::attributeNode)
    {
      pos = theParent->attributes().find(this);
      ZORBA_FATAL(pos < theParent->numAttributes(), "");
      theParent->attributes().remove(pos);
    }
    else
    {
      pos = theParent->children().find(this);
      ZORBA_FATAL(pos < theParent->numChildren(), "");
      theParent->children().remove(pos);
    }

    theParent = NULL;

    return pos;
  }
  catch(...)
  {
    ZORBA_FATAL(false, "Unexpected exception");
  }
}


/*******************************************************************************
  Deallocate all nodes in the subtree rooted at "this".
********************************************************************************/
void XmlNode::destroy() throw()
{
  try
  {
    disconnect();

    destroyInternal();
  }
  catch (...)
  {
    ZORBA_FATAL(false, "Unexpectd exception");
  }
}


void XmlNode::destroyInternal() throw()
{
  if (getNodeKind() == store::StoreConsts::elementNode)
  {
    ElementNode* elemNode = reinterpret_cast<ElementNode*>(this);

    ulong numChildren = elemNode->numChildren();
    ulong numAttrs = elemNode->numAttributes();

    for (ulong i = 0; i < numChildren; i++)
    {
      elemNode->getChild(i)->destroyInternal();
    }

    for (ulong i = 0; i < numAttrs; i++)
    {
      elemNode->getAttr(i)->destroyInternal();
    }
  }
  else if (getNodeKind() == store::StoreConsts::documentNode)
  {
    DocumentNode* docNode = reinterpret_cast<DocumentNode*>(this);

    ulong numChildren = docNode->numChildren();

    for (ulong i = 0; i < numChildren; i++)
    {
      XmlNode* child = docNode->getChild(i);
      if (child->theParent == this)
      {
        child->destroyInternal();
      }
    }
  }

  delete this;
}


/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class InternalNode                                                         //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************

********************************************************************************/
void InternalNode::insertChild(XmlNode* child, ulong pos)
{
  children().insert(child, pos);

  assert(child->theParent == NULL);

  child->theParent = this;
}


/*******************************************************************************
  Remove the i-th child of "this" (it is assumed that i < numChildren).
********************************************************************************/
bool InternalNode::removeChild(ulong pos)
{
  bool found = (pos < numChildren());

  if (found)
  {
    XmlNode* child = getChild(pos);
    assert(child->theParent == this);
    children().remove(pos);
    child->theParent = NULL;
  }

  return found;
}


/*******************************************************************************
  If the given node N is a child of "this", disconnect N from "this". Return
  true if N was a child of "this"; false otherwise.
********************************************************************************/
bool InternalNode::removeChild(XmlNode* child)
{
  bool found = children().remove(child);

  if (found)
  {
    assert(child->theParent == this);
    child->theParent = NULL;
  }

  return found;
}


/*******************************************************************************
  Remove the i-th attribute of "this" (it is assumed that i < numAttributes).
********************************************************************************/
void InternalNode::removeAttr(ulong i)
{
  XmlNode* attr = getAttr(i);

  attributes().remove(i);

  if (attr->theParent == this)
    attr->theParent = NULL;
}


/*******************************************************************************
  If the given node N is an attribute of "this", remove it as an attribute of
  "this". Return true if N was an attribute of "this"; false otherwise.
********************************************************************************/
bool InternalNode::removeAttr(XmlNode* attr)
{
  bool found = attributes().remove(attr);

  if (found)
  {
    assert(attr->theParent == this);
    attr->theParent = NULL;
  }

  return found;
}


/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class DocumentNode                                                         //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
  Node constructor used by FastXmlLoader
********************************************************************************/
DocumentNode::DocumentNode()
  :
  InternalNode(store::StoreConsts::documentNode)
{
  NODE_TRACE1("Loaded doc node " << this);
}


/*******************************************************************************

********************************************************************************/
DocumentNode::DocumentNode(
    XmlTree* tree,
    zstring& baseUri,
    zstring& docUri)
  :
  InternalNode(tree, NULL, 0, store::StoreConsts::documentNode)
{
  if (!baseUri.empty())
    tree->setBaseUri(baseUri);

  tree->setDocUri(docUri);

  NODE_TRACE1("{\nConstructing doc node " << this << " tree = "
              << getTree()->getId() << ":" << getTree()
              << " doc uri = " << docUri);
}


/*******************************************************************************

********************************************************************************/
XmlNode* DocumentNode::copyInternal(
    InternalNode*          rootParent,
    InternalNode*          parent,
    long                   pos,
    const XmlNode*         rootCopy,
    const store::CopyMode& copymode) const
{
  ZORBA_ASSERT(rootParent == NULL && parent == NULL);

  XmlTree* tree = NULL;
  InternalNode* copyNode = NULL;

  try
  {
    tree = NodeFactory::instance().createXmlTree();

    zstring baseuri = getBaseUri();
    zstring docuri = getDocUri();

    copyNode = NodeFactory::instance().createDocumentNode(tree, baseuri, docuri);

    ulong numChildren = this->numChildren();
    for (ulong i = 0; i < numChildren; i++)
    {
      XmlNode* child = getChild(i);

      child->copyInternal(rootParent, copyNode, -1, NULL, copymode);
    }
  }
  catch (...)
  {
    // Note: no rchandles to copied nodes should be used during the copy, so
    // that we can call tree->free() and destroy all the nodes in the copied
    // tree.
    if (tree)
      tree->free();

    throw;
  }

  NODE_TRACE1("}");
  NODE_TRACE1("Copied doc node " << this << " to node " << copyNode);

  return copyNode;
}


/*******************************************************************************

********************************************************************************/
void DocumentNode::getBaseURIInternal(zstring& uri, bool& local) const
{
  local = true;
  uri = getBaseUri();
}


/*******************************************************************************

********************************************************************************/
store::Iterator_t DocumentNode::getChildren() const
{
  ChildrenIteratorImpl* res = new ChildrenIteratorImpl();
  res->init((InternalNode*)this);
  return res;
}


/*******************************************************************************

********************************************************************************/
store::Item* DocumentNode::getType() const
{
  // ???? should return NULL?
  return GET_STORE().theSchemaTypeNames[XS_UNTYPED];
}


/*******************************************************************************

********************************************************************************/
void DocumentNode::getTypedValue(store::Item_t& val, store::Iterator_t& iter) const
{
  zstring rch;
  getStringValue2(rch);
  GET_STORE().getItemFactory()->createUntypedAtomic(val, rch);
  iter = NULL;
}


/*******************************************************************************

********************************************************************************/
zstring DocumentNode::getStringValue() const
{
  zstring strval;
  getStringValue2(strval);
  return strval;
}


void DocumentNode::getStringValue2(zstring& val) const
{
  ulong numChildren = this->numChildren();

  store::StoreConsts::NodeKind kind;

  if (numChildren == 1)
  {
    kind = getChild(0)->getNodeKind();

    if (kind != store::StoreConsts::commentNode &&
        kind != store::StoreConsts::piNode)
    {
      getChild(0)->getStringValue2(val);
    }
  }
  else
  {
    for (ulong i = 0; i < numChildren; ++i)
    {
      kind = getChild(i)->getNodeKind();
      
      if (kind != store::StoreConsts::commentNode &&
          kind != store::StoreConsts::piNode)
        getChild(i)->appendStringValue(val);
    }
  }
}


void DocumentNode::appendStringValue(zstring& buf) const
{
  ulong numChildren = this->numChildren();

  for (ulong i = 0; i < numChildren; ++i)
  {
    store::StoreConsts::NodeKind kind = getChild(i)->getNodeKind();

    if (kind != store::StoreConsts::commentNode &&
        kind != store::StoreConsts::piNode)
      getChild(i)->appendStringValue(buf);
  }
}


/*******************************************************************************

********************************************************************************/
zstring DocumentNode::show() const
{
  std::stringstream strStream;

  strStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl
            << "<document";
  strStream << " baseUri = \"" << getBaseUri();
  strStream << " docUri = \"" << getDocUri();
  strStream << "\">" << std::endl;

  store::Iterator_t iter = getChildren();
  store::Item_t item;
  while (iter->next(item))
  {
    strStream << item->show();
  }

  strStream << std::endl << "</document>";

  return strStream.str();
}



/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class ElementNode                                                          //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
  Node constructor used during loading of an xml doc
********************************************************************************/
ElementNode::ElementNode(
    store::Item_t& nodeName,
    ulong          numBindings,
    ulong          numAttributes)
  :
  InternalNode(store::StoreConsts::elementNode)
{
  theName.transfer(nodeName);
  setHaveValue();
  resetRecursive();

  if (numBindings > 0)
  {
    theNsContext = new NsBindingsContext(numBindings);
    theFlags |= HaveLocalBindings;
  }

  if (numAttributes > 0)
    theAttributes.resize(numAttributes);

  NODE_TRACE1("Loaded elem node " << this << " name = " << theName->show()
              << " num bindings = " << numBindings << " num attributes = "
              << numAttributes << std::endl);
}


/*******************************************************************************
  Note: localBindings will be NULL if this contructor is called from the copy()
  method (because in that case, the in-scope bindings must be computed by the
  copy() method based on the copy mode). Otherwise, localBindings should not be
  NULL (but may be empty).
********************************************************************************/
ElementNode::ElementNode(
    XmlTree*                    tree,
    InternalNode*               parent,
    long                        pos,
    store::Item_t&              nodeName,
    store::Item_t&              typeName,
    bool                        haveTypedValue,
    bool                        haveEmptyValue,
    bool                        isInSubstGroup,
    const store::NsBindings*    localBindings,
    zstring&                    baseUri)
  :
  InternalNode(tree, parent, pos, store::StoreConsts::elementNode)
{
  try
  {
    theName.transfer(nodeName);
    theTypeName.transfer(typeName);

    if (haveTypedValue)
    {
      setHaveValue();

      if (haveEmptyValue)
        setHaveEmptyValue();
    }

    if (isInSubstGroup)
      setInSubstGroup();

    resetRecursive();

    if (localBindings)
    {
      if (!localBindings->empty())
      {
        theNsContext = new NsBindingsContext(*localBindings);
        theFlags |= HaveLocalBindings;
      }

      if (parent && parent->getNodeKind() == store::StoreConsts::elementNode)
      {
        setNsContext(reinterpret_cast<ElementNode*>(parent)->getNsContext());
      }
      else if (theNsContext == NULL)
      {
        // There is no parent or the parent is a doc node. Create an enpty
        // NsContext so that the (future) children of this node can inherit
        // from this NsContext. This way, if "this" node gets later inserted
        // under another node N and has to inherit the ns bindings of N, all
        // descendants of "this" will also inherit N's bindings by simply
        // setting this->theNsCntext->theParent to N->theNsContext.
        theNsContext = new NsBindingsContext();
        theFlags |= HaveLocalBindings;
      }

      addBindingForQName(theName, false, true);
    }

    // Note: Setting the base uri property of "this" involves the creation of an
    // attribute node having "this" as the parent. So, if addBaseUriProperty()
    // fails, we must manually remove that attribute to avoid memory leak.
    if (!baseUri.empty())
    {
      zstring dummy;
      addBaseUriProperty(baseUri, dummy);
    }

    // Connect this new node to its parent. Do this here (at the end of the try
    // stmt), so that we don't have to undo it inside the catch stmt.
    if (parent)
    {
      parent->children().insert(this, pos);

      XmlNode* ancestor = parent;

      while (ancestor != NULL &&
             ancestor->getNodeKind() == store::StoreConsts::elementNode)
      {
        ElementNode* elemAncestor = reinterpret_cast<ElementNode*>(ancestor);
        if (elemAncestor->theName->equals(theName))
        {
          elemAncestor->setRecursive();
          break;
        }

        ancestor = ancestor->theParent;
      }
    }
  }
  catch (...)
  {
    theName = NULL;
    theTypeName = NULL;
    theNsContext = NULL;

    if (numAttributes() != 0)
    {
      ulong pos = 0;
      XmlNode* attr = getAttr(pos);
      removeAttr(pos);
      delete attr;
    }

    throw;
  }

  NODE_TRACE1("Constructed element node " << this << " parent = "
              << std::hex << (parent ? (ulong)parent : 0) << " pos = " << pos
              << " tree = " << getTree()->getId() << ":" << getTree()
              << " ordpath = " << theOrdPath.show()
              << " name = " << theName->getStringValue()
              << " type = " << (theTypeName ?
                                theTypeName->getStringValue().c_str() :
                                "untyped"));
}


/*******************************************************************************
  This is a recursive function that copies a subtree rooted at node N and places
  the copied subtree under the given "rootParent" node, making it the "pos"-th
  child of "rootParent".

  N is "this" node during the 1st (non-recursive) invocation of this method.
  During this 1st invocation, "rootParent" and "parent" point to the same node
  (which may be NULL), "pos" is the position under rootParent where the copied
  subtree will be placed at, and "rootCopy" is NULL.

  During a recursive invocation, "parent" is the copy of the parent of "this",
  and "pos" is always -1, indicating that the copy of "this" will be appended
  to the children's list of "parent".

  "rootCopy" is the first copied node, i.e., the copy of node N.
********************************************************************************/
XmlNode* ElementNode::copyInternal(
    InternalNode*          rootParent,
    InternalNode*          parent,
    long                   pos,
    const XmlNode*         rootCopy,
    const store::CopyMode& copymode) const
{
  assert(parent != NULL || rootParent == NULL);

  XmlTree* tree = NULL;
  ElementNode* copyNode = NULL;

  store::Item_t nodeName = theName;
  store::Item_t typeName;
  bool haveValue;
  bool haveEmptyValue;
  bool inSubstGroup;

  NsBindingsContext* myParentNsContext = NULL;
  NsBindingsContext* copyParentNsContext = NULL;
  NsBindingsContext* rootNsContext = NULL;

  if (theParent && theParent->getNodeKind() == store::StoreConsts::elementNode)
    myParentNsContext = static_cast<ElementNode*>(theParent)->getNsContext();

  if (parent && parent->getNodeKind() == store::StoreConsts::elementNode)
    copyParentNsContext = static_cast<ElementNode*>(parent)->getNsContext();

  if (rootParent && rootParent->getNodeKind() == store::StoreConsts::elementNode)
    rootNsContext = reinterpret_cast<ElementNode*>(rootParent)->getNsContext();

  if (copymode.theTypePreserve)
  {
    typeName = theTypeName;
    haveValue = this->haveValue();
    haveEmptyValue = this->haveEmptyValue();
    inSubstGroup = this->isInSubstitutionGroup();
  }
  else
  {
    typeName = NULL;
    haveValue = true;
    haveEmptyValue = false;
    inSubstGroup = false;
  }

  zstring baseUri;

  try
  {
    if (parent == NULL)
      tree = NodeFactory::instance().createXmlTree();

    pos = (parent == rootParent ? pos : -1);

    copyNode = NodeFactory::instance().createElementNode(
                 tree, parent, pos, nodeName, typeName,
                 haveValue, haveEmptyValue, inSubstGroup,
                 NULL, // local bindings
                 baseUri);

    if (rootCopy == NULL)
      rootCopy = copyNode;

    if (copymode.theNsPreserve)
    {
      // If we are copying the root of an xml subtree, or a node that does
      // not inherit ns bindings directly from its parent (but may inherit
      // from another ancestor).
      if (parent == rootParent ||
          (haveLocalBindings() && theNsContext->getParent() != myParentNsContext) ||
          (!haveLocalBindings() && theNsContext != myParentNsContext))
      {
        store::NsBindings bindings;
        getNamespaceBindings(bindings);

        if (!bindings.empty())
        {
          copyNode->theNsContext = new NsBindingsContext();
          copyNode->theNsContext->getBindings().swap(bindings);
          copyNode->theFlags |= HaveLocalBindings;
        }

        if (rootParent &&
            rootParent->getNodeKind() == store::StoreConsts::elementNode &&
            copymode.theNsInherit)
        {
          // If "this" does not belong to any namespace and the root parent
          // has a default ns binding, then undeclare this default binding.
          const zstring& prefix = theName->getPrefix();
          const zstring& nsuri = theName->getNamespace();
          if (prefix.empty() &&
              nsuri.empty() &&
              rootParent->getNodeKind() == store::StoreConsts::elementNode)
          {
            zstring ns;
            if (reinterpret_cast<ElementNode*>(rootParent)->findBinding(prefix, ns))
              copyNode->addLocalBinding(prefix, nsuri);
          }

          copyNode->setNsContext(rootNsContext);
        }
      }

      // Else the node we are copying is not the root of the xml subtree and
      // it inherits ns bindings directly from its parent.
      else
      {
        if (haveLocalBindings())
        {
          copyNode->theNsContext = new NsBindingsContext(getLocalBindings());
          copyNode->theFlags |= HaveLocalBindings;
        }

        if (parent && parent->getNodeKind() == store::StoreConsts::elementNode)
          copyNode->setNsContext(copyParentNsContext);
      }

      if (copyNode->theNsContext == NULL)
      {
        copyNode->theNsContext = new NsBindingsContext();
        copyNode->theFlags |= HaveLocalBindings;
      }
    }
    else // ! nsPreserve
    {
      if (copymode.theTypePreserve &&
          theTypeName != NULL &&
          (theTypeName->equals(GET_STORE().theSchemaTypeNames[XS_QNAME]) ||
           theTypeName->equals(GET_STORE().theSchemaTypeNames[XS_NOTATION])))
      {
        ZORBA_ERROR(XQTY0086);
      }

      const zstring& prefix = theName->getPrefix();
      zstring ns;
 
      bool found = getNsContext()->findBinding(prefix, ns);

      // binding may be absent only if the prefix was empty and there was no
      // default namespace declaration in scope.
      ZORBA_ASSERT(prefix.empty() || found);

      std::auto_ptr<NsBindingsContext> ctx(new NsBindingsContext);

      if (found)
      {
        zstring ns2;
        bool found2 = false;

        if (rootParent &&
            rootParent->getNodeKind() == store::StoreConsts::elementNode &&
            copymode.theNsInherit)
        {
          found2 = rootNsContext->findBinding(prefix, ns2);
        }

        if (!found2 || ns != ns2)
          ctx->addBinding(prefix, ns);
      }

      ulong numAttrs = numAttributes();

      for (ulong i = 0; i < numAttrs; i++)
      {
        const zstring& prefix = getAttr(i)->getNodeName()->getPrefix();
        bool found = getNsContext()->findBinding(prefix, ns);

        ZORBA_ASSERT(prefix.empty() || prefix == "xml" || found);

        if (found)
        {
          zstring ns2;
          bool found2 = false;

          if (rootParent &&
              rootParent->getNodeKind() == store::StoreConsts::elementNode &&
              copymode.theNsInherit)
          {
            found2 = rootNsContext->findBinding(prefix, ns2);
          }

          if (!found2 || ns != ns2)
            ctx->addBinding(prefix, ns);
        }
      }

      if (!ctx->empty())
      {
        copyNode->theNsContext = ctx.release();
        copyNode->theFlags |= HaveLocalBindings;
      }

      if (copymode.theNsInherit && rootParent)
      {
        copyNode->setNsContext(rootNsContext);
      }

      if (copyNode->theNsContext == NULL)
      {
        copyNode->theNsContext = new NsBindingsContext();
        copyNode->theFlags |= HaveLocalBindings;
      }
    }

    // Copy the attributes of this node
    AttributeNode* baseUriAttr = NULL;
    AttributeNode* hiddenBaseUriAttr = NULL;
    ulong numAttrs = this->numAttributes();
    for (ulong i = 0; i < numAttrs; i++)
    {
      AttributeNode* attr = getAttr(i);

      if (attr->isBaseUri())
      {
        if (attr->isHidden())
        {
          hiddenBaseUriAttr = attr;
          continue;
        }
        else
        {
          baseUriAttr = attr;
        }
      }

      attr->copyInternal(rootParent, copyNode, -1, rootCopy, copymode);
    }

    if (hiddenBaseUriAttr)
    {
      if (parent == 0)
      {
        zstring absuri;
        hiddenBaseUriAttr->getStringValue2(absuri);
        zstring reluri;
        copyNode->addBaseUriProperty(absuri, reluri);
      }
      else if (baseUriAttr)
      {
        zstring absuri;
        zstring reluri;
        parent->getBaseURI(absuri);
        baseUriAttr->getBaseURI(reluri);

        copyNode->addBaseUriProperty(absuri, reluri);
      }
      else
      {
        // do not add explicit base uri property in the copy ==> copy inherits
        // the base-uri property of its parent.
      }
    }

    // Copy the children of this node
    ulong numChildren = this->numChildren();
    for (ulong i = 0; i < numChildren; i++)
    {
      XmlNode* child = getChild(i);

      // If we are copying a subtree into its own containing tree, then avoid
      // copying the root of this subtree again.
      if (child == rootCopy)
        continue;

      child->copyInternal(rootParent, copyNode, -1, rootCopy, copymode);
    }
  }
  catch (...)
  {
    if (tree)
    {
      tree->free();
    }
    else if (copyNode && (parent == rootParent))
    {
      copyNode->destroy();
    }

    throw;
  }

  NODE_TRACE1("Copied elem node " << this << " to node " << copyNode
              << " name = " << theName->getStringValue() << " parent = "
              << (parent ? parent : 0x0)
              << " pos = " << pos << " copy mode = " << copymode.toString());

  return copyNode;
}


/*******************************************************************************

********************************************************************************/
store::Item* ElementNode::getType() const
{
  return (theTypeName != NULL ?
          theTypeName.getp() :
          GET_STORE().theSchemaTypeNames[XS_UNTYPED].getp());
}


/*******************************************************************************

********************************************************************************/
bool ElementNode::haveTypedTypedValue() const
{
  if (numChildren() == 1 &&
      getChild(0)->getNodeKind() == store::StoreConsts::textNode)
  {
    if (reinterpret_cast<TextNode*>(getChild(0))->isTyped())
      return true;
  }

  return false;
}


/*******************************************************************************

********************************************************************************/
bool ElementNode::isId() const
{	
  if (numChildren() == 1 &&
      getChild(0)->getNodeKind() == store::StoreConsts::textNode)
  {
		if (reinterpret_cast<TextNode*>(getChild(0))->isIdInternal())
      return true;
	}
  
  return false;
}


/*******************************************************************************

********************************************************************************/
bool ElementNode::isIdRefs() const
{
  if (numChildren() == 1 &&
      getChild(0)->getNodeKind() == store::StoreConsts::textNode)
  {
    if (reinterpret_cast<TextNode*>(getChild(0))->isIdRefsInternal())
      return true;
  }

  return false;
}


/*******************************************************************************

********************************************************************************/
void ElementNode::getTypedValue(store::Item_t& val, store::Iterator_t& iter) const
{
  if (haveValue())
  {
    if (haveEmptyValue())
    {
      val = NULL;
      iter = NULL;
    }
    else if (haveTypedTypedValue())
    {
      const TextNode* child = reinterpret_cast<const TextNode*>(getChild(0));

      if (child->haveListValue())
      {
        ItemVector* vec = reinterpret_cast<ItemVector*>(child->getValue());
        iter = new ItemIterator(vec->getItems(), true);
        val = NULL;
      }
      else
      {
        val = child->getValue();
        iter = NULL;
      }
    }
    else
    {
      zstring rch;
      getStringValue2(rch);
      GET_STORE().getItemFactory()->createUntypedAtomic(val, rch);
    }
  }
  else
  {
    ZORBA_ERROR_DESC_OSS(FOTY0012,
                         "The element node " << theName->getStringValue()
                         << " with type "
                         << (theTypeName ?
                             theTypeName->getStringValue().c_str() : "untyped") 
                         << " does not have a typed value");
  }
}


/*******************************************************************************

********************************************************************************/
zstring ElementNode::getStringValue() const
{
  zstring strval;
  getStringValue2(strval);
  return strval;
}


/*******************************************************************************

********************************************************************************/
void ElementNode::getStringValue2(zstring& val) const
{
  ulong numChildren = this->numChildren();

  store::StoreConsts::NodeKind kind;

  if (numChildren == 1)
  {
    kind = getChild(0)->getNodeKind();

    if (kind != store::StoreConsts::commentNode &&
        kind != store::StoreConsts::piNode)
    {
      getChild(0)->getStringValue2(val);
    }
  }
  else
  {
    for (ulong i = 0; i < numChildren; ++i)
    {
      kind = getChild(i)->getNodeKind();
      
      if (kind != store::StoreConsts::commentNode &&
          kind != store::StoreConsts::piNode)
        getChild(i)->appendStringValue(val);
    }
  }
}


/*******************************************************************************

********************************************************************************/
void ElementNode::appendStringValue(zstring& buf) const
{
  ulong numChildren = this->numChildren();

  for (ulong i = 0; i < numChildren; ++i)
  {
    store::StoreConsts::NodeKind kind = getChild(i)->getNodeKind();

    if (kind != store::StoreConsts::commentNode &&
        kind != store::StoreConsts::piNode)
      getChild(i)->appendStringValue(buf);
  }
}


/*******************************************************************************

********************************************************************************/
store::Item_t ElementNode::getNilled() const
{
  store::Item_t val;

  if (theTypeName == NULL ||
      theTypeName->equals(GET_STORE().theSchemaTypeNames[XS_UNTYPED])) 
  {
    GET_STORE().getItemFactory()->createBoolean(val, false);
    return val;
  }

  bool nilled = true;
  ulong numChildren = this->numChildren();
  for (ulong i = 0; i < numChildren; i++)
  {
    if (getChild(i)->getNodeKind() == store::StoreConsts::elementNode ||
        getChild(i)->getNodeKind() == store::StoreConsts::textNode)
    {
      nilled = false;
      break;
    }
  }

  if (!nilled) 
  {
    GET_STORE().getItemFactory()->createBoolean(val, false);
    return val;
  }

  nilled = false;

  //const char* xsi = "http://www.w3.org/2001/XMLSchema-instance";
  //ulong xsilen = strlen(xsi);

  ulong numAttrs = this->numAttributes();
  for (ulong i = 0; i < numAttrs; i++)
  {
    AttributeNode* attr = getAttr(i);
    if (zorba::equals(attr->getNodeName()->getNamespace(), "xsi", 3) &&
        zorba::equals(attr->getNodeName()->getLocalName(), "nil", 3))
    {
      nilled = true;
      break;
    }
  }

  GET_STORE().getItemFactory()->createBoolean(val, nilled);
  return val;
}


/*******************************************************************************

********************************************************************************/
store::Iterator_t ElementNode::getAttributes() const
{
  AttributesIteratorImpl* res = new AttributesIteratorImpl();
  res->init(this);
  return res;
}


/*******************************************************************************

********************************************************************************/
store::Iterator_t ElementNode::getChildren() const
{
  ChildrenIteratorImpl* res = new ChildrenIteratorImpl();
  res->init((XmlNode*)this);
  return res;
}


/*******************************************************************************
  Carefull with this function: it generates the namespaces in the reverse order.
  The higher parent gives the latest namespaces, instead of first.
********************************************************************************/
void ElementNode::getNamespaceBindings(
    store::NsBindings&            bindings,
    store::StoreConsts::NsScoping ns_scoping) const
{
  assert(bindings.empty());
  assert(theNsContext != NULL);

  if (ns_scoping != store::StoreConsts::ONLY_PARENT_NAMESPACES)
  {
    bindings = theNsContext->getBindings();
  }

  if (ns_scoping == store::StoreConsts::ONLY_LOCAL_NAMESPACES)
    return;

  const NsBindingsContext* parentContext = theNsContext->getParent();

  while (parentContext != NULL)
  {
    const store::NsBindings& parentBindings = parentContext->getBindings();
    ulong parentSize = parentBindings.size();
    ulong currSize = bindings.size();

    // for each parent binding, add it to the result, if it doesn't have the
    // same prefix as another binding that is already in the result.
    for (ulong i = 0; i < parentSize; ++i)
    {
      ulong j;
      for (j = 0; j < currSize; ++j)
      {
        if (bindings[j].first == parentBindings[i].first)
          break;
      }

      if (j == currSize)
      {
        bindings.push_back(parentBindings[i]);
      }
    }

    parentContext = parentContext->getParent();
  }

  return;
}


/*******************************************************************************
  Make "this" inherit the ns bindings of its parent.
********************************************************************************/
void ElementNode::setNsContext(NsBindingsContext* parentCtx)
{
  assert(parentCtx != NULL);

  if (theNsContext == NULL)
  {
    theNsContext = parentCtx;
  }
  else if (theNsContext.getp() != parentCtx)
  {
    theNsContext->setParent(parentCtx);
  }
}


/*******************************************************************************
  Search all the in-scope ns bindings of "this" to find the ns uri associated
  with the given prefix. Return NULL if no binding exists for the given prefix.
********************************************************************************/
bool ElementNode::findBinding(const zstring& prefix, zstring& uri) const
{
  assert(theNsContext != NULL);

  return theNsContext->findBinding(prefix, uri);
}


/*******************************************************************************

********************************************************************************/
const store::NsBindings& ElementNode::getLocalBindings() const
{
  ZORBA_ASSERT(haveLocalBindings());
  return theNsContext->getBindings();
}


/*******************************************************************************
  Add the ns binding that is implied by the given qname, if such a binding does
  not exist already among the bindings of "this" node. The method returns true
  if a binding was added, or false otherwise.

  The method also checks if the given binding conflicts with the current bindings
  of "this" node. If a conflict exists and replacePrefix is false, an error is
  thrown. Else, if a conflict exists and replacePrefix is true, then the method
  creates and returns a qname with the same local name and ns uri as the given
  qname, but with a prefix that is artificially generated so that the conflict
  is resolved.

  This method is used by the ElementNode and AttributeNode constructors with
  replacePrefix set to true. It is also used by updating methods with
  replacePrefix set to false.
********************************************************************************/
bool ElementNode::addBindingForQName(
    store::Item_t& qname,
    bool           isAttr,
    bool           replacePrefix)
{
  const zstring& prefix = qname->getPrefix();
  const zstring& ns = qname->getNamespace();

  // If ns is empty, then prefix must be empty
  ZORBA_FATAL(!ns.empty() || prefix.empty(),
              "prefix = " << prefix << " ns = " << ns);

  // No ns binding is implied by an attribute qname whose ns uri is empty
  if (ns.empty() && isAttr)
    return false;

  if (prefix != "xml")
  {
    zstring ns2;
    bool found = findBinding(prefix, ns2);

    if (!found)
    {
      if (!ns.empty())
      {
        addLocalBinding(prefix, ns);
        return true;
      }
    }
    else if (ns2 != ns)
    {
      if (ns2.empty())
      {
        if (!haveLocalBindings())
        {
          theNsContext = new NsBindingsContext(theNsContext.getp());
        }

        theNsContext->updateBinding(prefix, ns);
      }

      if (replacePrefix)
      {
        //std::cout << "Prefix: " << prefix << " ns: " << ns << " ns2: " << ns2 << " local: " << qname->getLocalName() << "\n";
        ZORBA_FATAL(!ns.empty(), "");

        zstring prefix("XXX");
        zstring dummy;

        while (findBinding(prefix, dummy))
          prefix += "X";

        GET_FACTORY().createQName(qname, ns, prefix, qname->getLocalName());
        addLocalBinding(prefix, ns);
        return true;
      }
      else
      {
        ZORBA_ERROR_DESC_OSS(XUDY0024,
                             "The implied namespace binding of "
                             << qname->show()
                             << " conflicts with namespace binding ["
                             << prefix << ", " << ns2 << "]");
      }
    }
  }

  return false;
}


/*******************************************************************************
  Same as above, but used when we know that no ns binding conflict exists
********************************************************************************/
void ElementNode::addBindingForQName2(const store::Item* qname)
{
  const zstring& prefix = qname->getPrefix();
  const zstring& ns = qname->getNamespace();

  // If ns is empty, then prefix must be empty
  ZORBA_FATAL(!ns.empty() || prefix.empty(),
              "prefix = " << prefix << "ns = " << ns);

  if (prefix != "xml")
  {
    zstring ns2;
    bool found = findBinding(prefix, ns2);

    if (!found)
    {
      if (!ns.empty())
        addLocalBinding(prefix, ns);
    }
    else if (ns2 != ns)
    {
      ZORBA_FATAL(0, "");
    }
  }
}


/*******************************************************************************
  Add a given ns binding to the local ns bindings of "this", if it's not already
  there. It is assumed that the given binding does not conflit with the other
  local bindings of "this" (ZORBA_FATAL is called if this condition is not true).

  Note: it is possible to add the binding (empty --> empty); this happens when we
  need to delete the default binding (empty --> ns) from the bindings of "this".
********************************************************************************/
void ElementNode::addLocalBinding(const zstring& prefix, const zstring& ns)
{
  if (!haveLocalBindings())
  {
    NsBindingsContext* parent = theNsContext;
    theNsContext = new NsBindingsContext();
    theNsContext->setParent(parent);
  }

  theNsContext->addBinding(prefix, ns);

  theFlags |= HaveLocalBindings;
}


/*******************************************************************************
  Remove the given ns binding from the local ns bindings of "this", if it is
  there.
********************************************************************************/
void ElementNode::removeLocalBinding(const zstring& prefix, const zstring& ns)
{
  if (haveLocalBindings())
    theNsContext->removeBinding(prefix, ns);
}

#if 1
/*******************************************************************************

********************************************************************************/
void ElementNode::uninheritBinding(
    NsBindingsContext* rootNSCtx,
    const zstring& prefix)
{
  if (theNsContext.getp() == rootNSCtx || theNsContext->getParent() == rootNSCtx)
  {
    if (theNsContext.getp() == rootNSCtx)
    {
      theNsContext = new NsBindingsContext;
    }

    zstring emptyStr;

    theNsContext->addBinding(prefix, emptyStr, true);
  }

  ulong numChildren = this->numChildren();

  for (ulong i = 0; i < numChildren; ++i)
  {
    if (getChild(i)->getNodeKind() == store::StoreConsts::elementNode)
    {
      static_cast<ElementNode*>(getChild(i))->uninheritBinding(rootNSCtx, prefix);
    }
  }
}
#endif

/*******************************************************************************
  Check if the ns binding implied by the given qname conflicts with the current
  ns bindings of "this" node.
********************************************************************************/
void ElementNode::checkNamespaceConflict(
    const store::Item*  qname,
    XQUERY_ERROR        ecode) const
{
  const QNameItem* qn = reinterpret_cast<const QNameItem*>(qname);

  const zstring& prefix = qn->getPrefix();
  const zstring& ns = qn->getNamespace();

  // Nothing to do if the qname does not imply any ns binding
  if (prefix.empty() && ns.empty())
    return;

  zstring ns2;
  bool found = findBinding(prefix, ns2);

  if (found && ns2 != ns)
  {
    ZORBA_ERROR_DESC_OSS(ecode,
                         "The implied namespace binding of " << qname->show()
                         << " conflicts with namespace binding ["
                         << prefix << ", " << ns2 << "]");
  }
}


/*******************************************************************************
  Check that "this" does not have an attr with the same name as the given name.
********************************************************************************/
void ElementNode::checkUniqueAttr(const store::Item* attrName) const
{
  ulong numAttrs = numAttributes();
  for (ulong i = 0; i < numAttrs; ++i)
  {
    AttributeNode* attr = getAttr(i);
    if (!attr->isHidden() && attr->getNodeName()->equals(attrName))
    {
      ZORBA_ERROR_PARAM_OSS(XQDY0025, attrName->getStringValue(), "");
    }
  }
}


/*******************************************************************************
  Check that "this" does not have any attrs with the same name.
********************************************************************************/
void ElementNode::checkUniqueAttrs() const
{
  ulong numAttrs = numAttributes();
  for (ulong i = 0; i < numAttrs; ++i)
  {
    AttributeNode* attr = getAttr(i);

    if (attr->isHidden())
      continue;

    const store::Item* attrName = attr->theName.getp();

    for (ulong j = i+1; j < numAttrs; ++j)
    {
      AttributeNode* otherAttr = getAttr(j);

      if (!otherAttr->isHidden() && otherAttr->getNodeName()->equals(attrName))
      {
        ZORBA_ERROR_PARAM_OSS(XUDY0021, attrName->getStringValue(), "");
      }
    }
  }
}


/*******************************************************************************
  Compute the base-uri property of this element node and store it as a a special
  (hidden) attribute of the node. In general, the base-uri property is computed
  by resolving the "relUri" based on the absolute "absUri". However, if "relUri"
  is an absolute uri already, the base-uri property of the node is set to "relUri".
  It is also possible that "relUri" is empty, in which case, the base-uri property
  of the node is set to "absUri".

  It is assumed that both "absUri" and "relUri" (if not empty) are well-formed
  uri strings.
********************************************************************************/
void ElementNode::addBaseUriProperty(
    zstring& absUri,
    zstring& relUri)
{
  ZORBA_FATAL(!absUri.empty(), "");

  const SimpleStore& store = GET_STORE();

  store::Item_t qname = store.getQNamePool().insert(store.XML_URI, "xml", "base");
  store::Item_t typeName = store.theSchemaTypeNames[XS_ANY_URI];

  store::Item_t typedValue;

  if (relUri.empty())
  {
    GET_STORE().getItemFactory()->createAnyURI(typedValue, absUri);
  }
  else
  {
    zstring resolvedUriString;

    try
    {
      URI absoluteURI(absUri);
      URI resolvedURI(absoluteURI, relUri);
      resolvedUriString = resolvedURI.toString();
    }
    catch (error::ZorbaError&)
    {
      resolvedUriString = relUri;
    }

    GET_STORE().getItemFactory()->createAnyURI(typedValue, resolvedUriString);
  }

  checkUniqueAttr(qname.getp());

  // this might look like a nop but it isn't
  // the this pointer is used in the constructor to
  // attach it to the parent
  GET_STORE().getNodeFactory().createAttributeNode(
    NULL,  // xml tree
    this,  // parent
    0,     // position
    qname,
    typeName,
    typedValue,
    false, // isListVale
    true); // hidden

  setHaveBaseUri();
}


/*******************************************************************************
  Change the base-uri property of this node. The new base-uri value is computed
  by resolving the "relUri" based on the "absUri". If "relUri" is NULL, then the
  new base-uri value is set to "absUri". If "relUri" is already an absolute uri,
  then the new base-uri value is set to "relUri". "attr" is the hidden attribute
  that stores the base-uri value.
********************************************************************************/
void ElementNode::adjustBaseUriProperty(
    AttributeNode* attr,
    zstring& absUri,
    zstring& relUri)
{
  ZORBA_FATAL(!absUri.empty(), "");

  store::Item_t typedValue;
  if (relUri == NULL)
  {
    GET_STORE().getItemFactory()->createAnyURI(typedValue, absUri);
  }
  else
  {
    zstring resolvedUriString;

    try
    {
      URI lAbsoluteUri(absUri);
      URI lResolvedUri(lAbsoluteUri, relUri);
      resolvedUriString = lResolvedUri.toString();
    }
    catch (error::ZorbaError& e)
    {
      ZORBA_FATAL(e.theErrorCode, e.theDescription);
    }

    GET_STORE().getItemFactory()->createAnyURI(typedValue, resolvedUriString);
  }

  attr->setTypedValue(typedValue);
}


/*******************************************************************************

********************************************************************************/
void ElementNode::getBaseURIInternal(zstring& uri, bool& local) const
{
  ulong numAttrs = numAttributes();
  for (ulong i = 0; i < numAttrs; ++i)
  {
    AttributeNode* attr = getAttr(i);
    if (attr->isBaseUri() && attr->isHidden())
    {
      local = true;
      attr->getStringValue2(uri);
      return;
    }
  }

  local = false;

  if (theParent)
    theParent->getBaseURI(uri);
}


/*******************************************************************************

********************************************************************************/
zstring ElementNode::show() const
{
  std::stringstream str;

  str <<  "<" << theName->getStringValue() << "-elem";

  str << " nid=\"" << theOrdPath.show() << "\"";

  store::NsBindings nsBindings;
  getNamespaceBindings(nsBindings);

  for (ulong i = 0; i < nsBindings.size(); i++)
  {
    str << " xmlns:" <<  nsBindings[i].first << "=\""
        << nsBindings[i].second << "\"";
  }

  store::Iterator_t iter = getAttributes();
  store::Item_t item;
  while (iter->next(item))
  {
    str << " " << item->show();
  }

  str << ">";

  iter = getChildren();
  iter->open();
  while (iter->next(item))
  {
    str << item->show();
  }

  str << "</" << theName->getStringValue() << "-elem>";
  return str.str();
}



/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class AttributeNode                                                        //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
  Node constructor used by FastXmlLoader only.
********************************************************************************/
AttributeNode::AttributeNode(store::Item_t& attrName)
  :
  XmlNode(store::StoreConsts::attributeNode)
{
  theName.transfer(attrName);

  QNameItem* qn = reinterpret_cast<QNameItem*>(theName.getp());

  if (qn->isBaseUri())
    theFlags |= IsBaseUri;

  NODE_TRACE1("Loaded attr node " << this << " name = " << theName->getStringValue());
}


/*******************************************************************************
  Note: Constructor does NOT check whether the the attr node to create has the
  same name as an existing attr of the parent node. This is so because sometimes
  we know that a name conflict cannot arise (e.g. when the attr is created as
  part of a subtree copy, and the root node of that subtree is not an attr node),
  so we save the overhead of checking. When a check is required, it is the
  responsibility of the caller to call checkUniqueAttr() on the parent before
  invoking this constructor.
********************************************************************************/
AttributeNode::AttributeNode(
    XmlTree*          tree,
    ElementNode*      parent,
    long              pos,
    store::Item_t&    attrName,
    store::Item_t&    typeName,
    store::Item_t&    typedValue,
    bool              isListValue,
    bool              hidden)
  :
  XmlNode(tree, parent, pos, store::StoreConsts::attributeNode)
{
  // Normally, no exceptions are expected by the rest of the code here, but
  // just to be safe, we use a try-catch.
  try
  {
    if (attrName->getPrefix().empty() && !attrName->getNamespace().empty())
    {
      zstring prefix;
      const zstring& lAttrNs = attrName->getNamespace();
      if (lAttrNs == XML_NS)
        prefix = "xml";
      else if (lAttrNs == XMLNS_NS)
        prefix = "xmlns";
      else
        prefix = "XXX";

      GET_FACTORY().createQName(attrName,
                                attrName->getNamespace(),
                                prefix,
                                attrName->getLocalName());
    }

    theName.transfer(attrName);
    theTypeName.transfer(typeName);
    theTypedValue.transfer(typedValue);

    if (isListValue)
      setHaveListValue();

    QNameItem* qn = reinterpret_cast<QNameItem*>(theName.getp());

    if (qn->isBaseUri())
      theFlags |= IsBaseUri;

    if (hidden)
      setHidden();

    if (parent)
    {
      // If this is an explicit base uri attribute, (a) set or update the base-uri
      // property of the parent, and (b) create an additional hidden base-uri attr
      // with the resolved version of the given uri.
      if (isBaseUri() && !isHidden())
      {
        zstring parentBaseUri;
        parent->getBaseURI(parentBaseUri);
        zstring baseUri;
        this->getStringValue2(baseUri);

        if (parent->haveBaseUri())
        {
          ulong numAttrs = parent->numAttributes();
          for (ulong i = 0; i < numAttrs; i++)
          {
            AttributeNode* attr = parent->getAttr(i);
            if (attr->isBaseUri() && attr->isHidden())
            {
              attr->destroy();
              break;
            }
          }
        }

        if (parentBaseUri.empty())
          parent->addBaseUriProperty(baseUri, parentBaseUri);
        else
          parent->addBaseUriProperty(parentBaseUri, baseUri);
      }

      // Else, add the ns binding implied by the attr name into the in-scope ns
      // bindings of the parent (if this ns binding is not there already)
      else if (!isHidden())
      {
        parent->addBindingForQName(theName, true, true);
      }

      // Connect "this" to its parent. We do this at the end of this method
      // so that we don't have to undo it inside the catch clause below.
      parent->attributes().insert(this, pos);
    }
  }
  catch (...)
  {
    theName = NULL;
    theTypeName = NULL;
    theTypedValue = NULL;

    throw;
  }

  NODE_TRACE1("Constructed attribute node " << this << " parent = "
              << std::hex << (parent ? (ulong)parent : 0) << " pos = " << pos
              << " tree = " << getTree()->getId() << ":" << getTree()
              << " ordpath = " << theOrdPath.show()
              << " name = " << theName->getStringValue()
              << " value = " << getStringValue());
}


/*******************************************************************************

********************************************************************************/
XmlNode* AttributeNode::copyInternal(
    InternalNode*          rootParent,
    InternalNode*          parent,
    long                   pos,
    const XmlNode*         rootCopy,
    const store::CopyMode& copymode) const
{
  assert(parent != NULL || rootParent == NULL);
  ZORBA_FATAL(!isHidden(), "");

  SimpleStore& store = GET_STORE();

  XmlTree* tree = NULL;
  AttributeNode* copyNode = NULL;
  store::Item_t nodeName = theName;
  store::Item_t typeName;
  store::Item_t typedValue;

  bool isListValue;

  if (copymode.theTypePreserve)
  {
    typeName = theTypeName;
    typedValue = theTypedValue;

    isListValue = haveListValue();
  }
  else
  {
    isListValue = false;

    typeName = NULL;

    if (!haveListValue() &&
        theTypedValue->getType()->equals(store.theSchemaTypeNames[XS_UNTYPED_ATOMIC]))
    {
      typedValue = theTypedValue;
    }
    else
    {
      zstring rch;
      getStringValue2(rch);
      GET_STORE().getItemFactory()->createUntypedAtomic(typedValue, rch);
    }
  }

  try
  {
    if (parent == NULL)
      tree = GET_STORE().getNodeFactory().createXmlTree();

    else if (parent == rootParent)
      reinterpret_cast<ElementNode*>(parent)->checkUniqueAttr(nodeName);

    copyNode = GET_STORE().getNodeFactory().createAttributeNode(
                 tree, static_cast<ElementNode*>(parent),
                 pos,
                 nodeName,
                 typeName, typedValue, isListValue,
                 false); // hidden
  }
  catch (...)
  {
    delete tree;
    throw;
  }

  NODE_TRACE1("Copied attribute node " << this << " to node " << copyNode
              << " name = " << theName->show() << " parent = "
              << std::hex << (parent ? (ulong)parent : 0) << " pos = " << pos
              << " copy mode = " << copymode.toString());

  return copyNode;
}


/*******************************************************************************

********************************************************************************/
store::Item* AttributeNode::getType() const
{
  return (theTypeName != NULL ?
          theTypeName.getp() :
          GET_STORE().theSchemaTypeNames[XS_UNTYPED_ATOMIC].getp());
}


/*******************************************************************************

********************************************************************************/
void AttributeNode::setTypedValue(store::Item_t& value)
{
  resetHaveListValue();
  theTypedValue.transfer(value);
}


/*******************************************************************************

********************************************************************************/
void AttributeNode::getTypedValue(store::Item_t& val, store::Iterator_t& iter) const
{
  if (haveListValue())
  {
    iter = new ItemIterator(getValueVector().getItems(), true);
    val = NULL;
  }
  else
  {
    val = theTypedValue;
    iter = NULL;
  }
}


/*******************************************************************************

********************************************************************************/
bool AttributeNode::isId() const
{
  QNameItem* qn = reinterpret_cast<QNameItem*>(theName.getp());

  if (qn->isIdQName())
    return true;

  if (dynamic_cast<IDItemImpl*>(theTypedValue.getp()) != NULL)
    return true;

  return false;
}


/*******************************************************************************

********************************************************************************/
bool AttributeNode::isIdRefs() const
{
  if (haveListValue())
  {
    const ItemVector& values = getValueVector();
    ulong numValues = values.size();

    for (ulong i = 0; i < numValues; ++i)
    {
      if (dynamic_cast<IDREFItemImpl*>(values.getItem(i)) != NULL)
      {
        return true;
      }
      else if (dynamic_cast<UserTypedAtomicItemImpl*>(values.getItem(i)) != NULL)
      {
        UserTypedAtomicItemImpl* utai = dynamic_cast<UserTypedAtomicItemImpl*>(values.getItem(i));
        if ( utai->getTypeCode() == XS_IDREF )
          return true;
        if ( (dynamic_cast<AtomicItem*>(utai->getBaseItem()))->getTypeCode() == XS_IDREF )
          return true;
      }
    }
  }
  else if (dynamic_cast<IDREFItemImpl*>(theTypedValue.getp()) != NULL)
  {
    return true;
  }
  else if (dynamic_cast<UserTypedAtomicItemImpl*>(theTypedValue.getp()) != NULL)
  {
    UserTypedAtomicItemImpl* utai = dynamic_cast<UserTypedAtomicItemImpl*>(theTypedValue.getp());
    if ( utai->getTypeCode() == XS_IDREF )
      return true;
    if ( (dynamic_cast<AtomicItem*>(utai->getBaseItem()))->getTypeCode() == XS_IDREF )
      return true;
  }

  return false;
}


/*******************************************************************************

********************************************************************************/
zstring AttributeNode::getStringValue() const
{
  zstring strval;
  getStringValue2(strval);
  return strval;
}


void AttributeNode::getStringValue2(zstring& val) const
{
  if (haveListValue())
  {
    const std::vector<store::Item_t>& items = getValueVector().getItems();

    ulong size = items.size();

    if (size == 1)
    {
      items[0]->getStringValue2(val);
    }
    else if (size > 0)
    {
      items[0]->appendStringValue(val);

      for (ulong i = 1; i < size; ++i)
      {
        val += " ";
        items[i]->appendStringValue(val);
      }
    }
  }
  else
  {
    theTypedValue->getStringValue2(val);
  }
}


void AttributeNode::appendStringValue(zstring& buf) const
{
  if (haveListValue())
  {
    const std::vector<store::Item_t>& items = getValueVector().getItems();

    ulong size = items.size();

    if (size > 0)
    {
      items[0]->appendStringValue(buf);

      for (ulong i = 1; i < size; i++)
      {
        buf += " ";
        items[i]->appendStringValue(buf);
      }
    }
  }
  else
  {
    theTypedValue->appendStringValue(buf);
  }
}


/*******************************************************************************

********************************************************************************/
zstring AttributeNode::show() const
{
  return theName->getStringValue() + "=\"" + getStringValue() + "\"";
}


/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class TextNode                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
  Node constructor used by FastXmlLoader
********************************************************************************/
TextNode::TextNode(zstring& content)
  :
  XmlNode(store::StoreConsts::textNode)
{
  setText(content);

  NODE_TRACE1("Loaded text node " << this << " content = " << getText());
}


/*******************************************************************************
  Note: constructor does NOT check if the new text node will be adjacent to an
  existing text node of the parent. This is so because sometimes we know that
  this case cannot arise (e.g. when the text node is created as part of a subtree
  copy, and the root node of that subtree is not a text node), so we save the
  overhead of checking. When a check is required, it is the responsibility of
  the caller to merge the new text content into the existing text node and skip
  invoking this constructor.
********************************************************************************/
TextNode::TextNode(
    XmlTree*      tree,
    InternalNode* parent,
    long          pos,
    zstring&      content)
  :
  XmlNode(tree, parent, pos, store::StoreConsts::textNode)
{
  setText(content);

  if (parent)
  {
    if (parent->getNodeKind() == store::StoreConsts::elementNode &&
        parent->numChildren() == 1 &&
        parent->getChild(0)->getNodeKind() == store::StoreConsts::textNode)
    {
      TextNode* textChild = reinterpret_cast<TextNode*>(parent->getChild(0));
      ZORBA_ASSERT(!textChild->isTyped());
    }

    parent->children().insert(this, pos);
  }

  NODE_TRACE1("Constructed text node " << this << " parent = "
              << std::hex << (parent ? (ulong)parent : 0) << " pos = " << pos
              << " tree = " << getTree()->getId() << ":" << getTree()
              << " ordpath = " << theOrdPath.show() << " content = "
              << getText());
}


/*******************************************************************************

********************************************************************************/
TextNode::TextNode(
    InternalNode*     parent,
    store::Item_t&    content,
    bool              isListValue)
  :
  XmlNode(NULL, parent, -1, store::StoreConsts::textNode)
{
  assert(parent != NULL);

  ZORBA_ASSERT(parent->getNodeKind() == store::StoreConsts::elementNode);

  ElementNode* p = reinterpret_cast<ElementNode*>(parent);

  ZORBA_ASSERT(p->numChildren() == 0);
  ZORBA_ASSERT(p->haveValue() && !p->haveEmptyValue());

  setTypedValue(content);
  if (isListValue)
    setHaveListValue();

  p->children().push_back(this);

  NODE_TRACE1("Constructed text node " << this << " parent = "
              << std::hex << (parent ? (ulong)parent : 0)
              << " ordpath = " << theOrdPath.show() << " content = "
              << getValue()->getStringValue());
}


/*******************************************************************************

********************************************************************************/
XmlNode* TextNode::copyInternal(
    InternalNode*          rootParent,
    InternalNode*          parent,
    long                   pos,
    const XmlNode*         rootCopy,
    const store::CopyMode& copymode) const
{
  assert(parent != NULL || rootParent == NULL);

  NodeFactory& factory = NodeFactory::instance();

  XmlTree* tree = NULL;
  TextNode* copyNode = NULL;
  store::Item_t typedContent;

  try
  {
    if (parent == NULL)
    {
      tree = factory.createXmlTree();

      zstring content = getText();
      copyNode = factory.createTextNode(tree, NULL, pos, content);
    }
    else
    {
      if (parent == rootParent)
      {
        // We are copying a subtree consisting of a single text node ("this").
        // The text node may or may not be typed, but the copied node will
        // always be untyped.

        // Merge adjacent text nodes.
        ulong numChildren = parent->numChildren();
        ulong pos2 = (pos >= 0 ? pos : numChildren);

        XmlNode* lsib = (pos2 > 0 ? parent->getChild(pos2-1) : NULL);
        XmlNode* rsib = (pos2 < numChildren ? parent->getChild(pos2) : NULL);

        if (lsib != NULL &&
            lsib->getNodeKind() == store::StoreConsts::textNode)
        {
          TextNode* textSibling = reinterpret_cast<TextNode*>(lsib);
          ZORBA_ASSERT(!textSibling->isTyped());

          zstring content = textSibling->getText();
          appendStringValue(content);

          textSibling->setText(content);
          copyNode = textSibling;
        }
        else if (rsib != NULL &&
                 rsib->getNodeKind() == store::StoreConsts::textNode)
        {
          TextNode* textSibling = reinterpret_cast<TextNode*>(rsib);
          ZORBA_ASSERT(!textSibling->isTyped());

          zstring content;
          getStringValue2(content);
          content += textSibling->getText();

          textSibling->setText(content);
          copyNode = textSibling;
        }
        else if (isTyped())
        {
          zstring content;
          getValue()->getStringValue2(content);
          copyNode = factory.createTextNode(NULL, parent, pos, content);
        }
        else
        {
          zstring content = getText();
          copyNode = factory.createTextNode(NULL, parent, pos, content);
        }
      }
      else if (isTyped())
      {
        if (copymode.theTypePreserve)
        {
          typedContent = getValue();
          copyNode = factory.createTextNode(parent, typedContent, haveListValue());
        }
        else
        {
          zstring content;
          getValue()->getStringValue2(content);
          copyNode = factory.createTextNode(NULL, parent, pos, content);
        }
      }
      else
      {
        zstring content = getText();
        copyNode = factory.createTextNode(NULL, parent, pos, content);
      }
    }
  }
  catch (...)
  {
    delete tree;
    throw;
  }

  NODE_TRACE1("Copied text node " << this << " to node " << copyNode
              << " parent = " << std::hex << (parent ? (ulong)parent : 0)
              << " pos = " << pos);

  return copyNode;
}



/*******************************************************************************

********************************************************************************/
bool TextNode::isTyped() const
{
  return (theFlags & IsTyped) != 0;
}


/*******************************************************************************

********************************************************************************/
void TextNode::setTypedValue(store::Item_t& value)
{
  if (!isTyped())
  {
    destroyText();

    setValue(value);

    theFlags |= IsTyped;
  }
  else
  {
    setValue(value);
  }
}


/*******************************************************************************

********************************************************************************/
void TextNode::revertToTextContent()
{
  if (isTyped())
  {
    zstring textValue;
    getStringValue2(textValue);

    setValue(NULL);

    setText(textValue);

    theFlags &= ~IsTyped;
  }
}


/*******************************************************************************

********************************************************************************/
store::Item* TextNode::getType() const
{
  return GET_STORE().theSchemaTypeNames[XS_UNTYPED_ATOMIC];
}


/*******************************************************************************

********************************************************************************/
void TextNode::getTypedValue(store::Item_t& val, store::Iterator_t& iter) const
{
  zstring rch;

  if (isTyped())
  {
    getValue()->getStringValue2(rch);
    GET_STORE().getItemFactory()->createUntypedAtomic(val, rch);
  }
  else
  {
    rch = getText();
    GET_STORE().getItemFactory()->createUntypedAtomic(val, rch);
  }
  iter = NULL;
}


/*******************************************************************************

********************************************************************************/
bool TextNode::isIdInternal() const
{
  if (isTyped() && 
    (static_cast<AtomicItem*>(getValue()))->isAtomic() && 
    (static_cast<AtomicItem*>(getValue()))->getTypeCode() == XS_ID)
    return true;
		
  return false;
}


/*******************************************************************************

********************************************************************************/
bool TextNode::isIdRefsInternal() const
{
  if (isTyped())
  {
    store::Item* value = getValue();

    if (haveListValue())
    {
      const ItemVector& values = *reinterpret_cast<ItemVector*>(value);
      ulong numValues = values.size();

      for (ulong i = 0; i < numValues; ++i)
      {
        if (dynamic_cast<IDREFItemImpl*>(values.getItem(i)) != NULL)
        {
          return true;
        }
        else if (dynamic_cast<UserTypedAtomicItemImpl*>(values.getItem(i)) != NULL)
        {
          UserTypedAtomicItemImpl* utai = dynamic_cast<UserTypedAtomicItemImpl*>(values.getItem(i));
          if ( utai->getTypeCode() == XS_IDREF )
            return true;
          if ( (dynamic_cast<AtomicItem*>(utai->getBaseItem()))->getTypeCode() == XS_IDREF )
            return true;
        }
      }
    }
    else if (dynamic_cast<IDREFItemImpl*>(value) != NULL)
    {
      return true;
    }
    else if (dynamic_cast<UserTypedAtomicItemImpl*>(value) != NULL)
    {
      UserTypedAtomicItemImpl* utai = dynamic_cast<UserTypedAtomicItemImpl*>(value);
      if ( utai->getTypeCode() == XS_IDREF )
        return true;
      if ( (dynamic_cast<AtomicItem*>(utai->getBaseItem()))->getTypeCode() == XS_IDREF )
        return true;
    }
  }

  return false;
}


/*******************************************************************************

********************************************************************************/
zstring TextNode::getStringValue() const
{
  if (isTyped())
  {
    return getValue()->getStringValue();
  }
  else
  {
    return getText();
  }
}


void TextNode::getStringValue2(zstring& val) const
{
  if (isTyped())
  {
    getValue()->getStringValue2(val);
  }
  else
  {
    val = getText();
  }
}


void TextNode::appendStringValue(zstring& buf) const
{
  if (isTyped())
  {
    getValue()->appendStringValue(buf);
  }
  else
  {
    buf += getText();
  }
}


/*******************************************************************************

********************************************************************************/
zstring TextNode::show() const
{
  return "<text nid=\"" + theOrdPath.show() + "\">"
    + getStringValue() + "</text>";
}


/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class PiNode                                                               //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
  Node constructor used by FastXmlLoader
********************************************************************************/
PiNode::PiNode(zstring& target, zstring& content)
  :
  XmlNode(store::StoreConsts::piNode)
{
  QNamePool& qnpool = GET_STORE().getQNamePool();

  theTarget.take(target);
  theContent.take(content);

  theName = qnpool.insert(zstring(), zstring(), theTarget);

  NODE_TRACE1("Loaded pi node " << this << " target = " << theTarget
              << std::endl);
}


/*******************************************************************************

********************************************************************************/
PiNode::PiNode(
    XmlTree*      tree,
    InternalNode* parent,
    long          pos,
    zstring&      target,
    zstring&      content)
  :
  XmlNode(tree, parent, pos, store::StoreConsts::piNode)
{
  QNamePool& qnpool = GET_STORE().getQNamePool();

  theTarget.take(target);
  theContent.take(content);

  theName = qnpool.insert(zstring(), zstring(), theTarget);

  if (parent)
  {
    if (pos < 0)
      parent->children().push_back(this);
    else
      parent->children().insert(this, pos);
  }

  NODE_TRACE1("Constructed pi node " << this << " parent = "
              << std::hex << (parent ? (ulong)parent : 0) << " pos = " << pos
              << " tree = " << getTree()->getId() << ":" << getTree()
              << " ordpath = " << theOrdPath.show() << " target = " << theTarget);
}


/*******************************************************************************

********************************************************************************/
XmlNode* PiNode::copyInternal(
    InternalNode*          rootParent,
    InternalNode*          parent,
    long                   pos,
    const XmlNode*         rootCopy,
    const store::CopyMode& copymode) const
{
  assert(parent != NULL || rootParent == NULL);

  PiNode* copyNode = NULL;
  XmlTree* tree = NULL;
  zstring content;
  zstring target;

  try
  {
    target = theTarget;
    content = theContent;

    if (parent == NULL)
    {
      tree = NodeFactory::instance().createXmlTree();
    }
 
    copyNode = NodeFactory::instance().createPiNode(tree,
                                                    parent,
                                                    pos,
                                                    target,
                                                    content);
  }
  catch (...)
  {
    delete tree;
    throw;
  }

  NODE_TRACE1("Copied pi node " << this << " to node " << copyNode
              << " parent = " << std::hex << (parent ? (ulong)parent : 0)
              << " pos = " << pos);

  return copyNode;
}


/*******************************************************************************

********************************************************************************/
store::Item* PiNode::getType() const
{
  return GET_STORE().theSchemaTypeNames[XS_UNTYPED_ATOMIC];
}


void PiNode::getTypedValue(store::Item_t& val, store::Iterator_t& iter) const
{
  zstring rch = theContent;
  GET_STORE().getItemFactory()->createString(val, rch);
  iter = NULL;
}


/*******************************************************************************

********************************************************************************/
zstring PiNode::show() const
{
  return "<?" + theTarget + " " + theContent + "?>";
}


/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class CommentNode                                                          //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
  Node constructor used during loading of an xml doc
********************************************************************************/
CommentNode::CommentNode(zstring& content)
  :
  XmlNode(store::StoreConsts::commentNode)
{
  theContent.take(content);

  NODE_TRACE1("Loaded comment node " << this << " content = " << theContent);
}


/*******************************************************************************

********************************************************************************/
CommentNode::CommentNode(
    XmlTree*      tree,
    InternalNode* parent,
    long          pos,
    zstring&      content)
  :
  XmlNode(tree, parent, pos, store::StoreConsts::commentNode)
{
  theContent.take(content);

  if (parent)
  {
    if (pos < 0)
      parent->children().push_back(this);
    else
      parent->children().insert(this, pos);
  }

  NODE_TRACE1("Constructed comment node " << this << " parent = "
              << std::hex << (parent ? (ulong)parent : 0) << " pos = " << pos
              << " tree = " << getTree()->getId() << ":" << getTree()
              << " ordpath = " << theOrdPath.show() << " content = "
              << theContent);
}


/*******************************************************************************

********************************************************************************/
XmlNode* CommentNode::copyInternal(
    InternalNode*          rootParent,
    InternalNode*          parent,
    long                   pos,
    const XmlNode*         rootCopy,
    const store::CopyMode& copymode) const
{
  assert(parent != NULL || rootParent == NULL);

  CommentNode* copyNode = NULL;
  XmlTree* tree = NULL;
  zstring content;

  try
  {
    if (parent == NULL)
    {
      tree = GET_STORE().getNodeFactory().createXmlTree();

      content = theContent;
      copyNode = GET_STORE().getNodeFactory().createCommentNode(
                   tree, NULL, pos, content);
    }
    else
    {
      content = theContent;
      copyNode = GET_STORE().getNodeFactory().createCommentNode(
                   tree, parent, pos, content);
    }
  }
  catch (...)
  {
    delete tree;
    throw;
  }

  NODE_TRACE1("Copied coment node " << this << " to node " << copyNode
              << " parent = " << std::hex << (parent ? (ulong)parent : 0)
              << " pos = " << pos);

  return copyNode;
}


/*******************************************************************************

********************************************************************************/
store::Item* CommentNode::getType() const
{
  return NULL;
  // return GET_STORE().theSchemaTypeNames[XS_UNTYPED_ATOMIC];
}


void CommentNode::getTypedValue(store::Item_t& val, store::Iterator_t& iter) const
{
  zstring rch = theContent;
  GET_STORE().getItemFactory()->createString(val, rch);
  iter = NULL;
}


/*******************************************************************************

********************************************************************************/
zstring CommentNode::show() const
{
  return "<!--" + theContent + "-->";
}

#ifndef ZORBA_NO_FULL_TEXT

/******************************************************************************
 *
 *  Full-text
 *
 ******************************************************************************/

inline void XmlNodeTokenizer::beginTokenization( XmlNode &node ) {
  node.theBeginTokenIndex = tokens_.size();
}

inline void XmlNodeTokenizer::endTokenization( XmlNode &node ) {
  node.theEndTokenIndex = tokens_.size();
}

void XmlNodeTokenizer::operator()( char const *utf8_s, int utf8_len,
                                   int pos, int sent, int para,
                                   void *payload ) {
  store::Item const *const item = static_cast<store::Item*>( payload );
  FTToken t( utf8_s, utf8_len, pos, sent, para, item, get_lang() );
  tokens_.push_back( t );
}


inline void XmlNodeTokenizer::tokenize( char const *utf8_s, int len ) 
{
  tokenizer_.tokenize(
    utf8_s, len, get_lang(), *this,
    element_stack_.empty() ? NULL : static_cast<void*>( get_element() )
  );
}


void XmlNode::tokenize( XmlNodeTokenizer& ) 
{
  // do nothing
}


void InternalNode::tokenize( XmlNodeTokenizer& tokenizer ) 
{
  tokenizer.inc_para();
  tokenizer.beginTokenization( *this );
  for ( ulong i = 0; i < numChildren(); ++i )
    getChild( i )->tokenize( tokenizer );
  tokenizer.endTokenization( *this );
}


void ElementNode::tokenize( XmlNodeTokenizer& tokenizer ) 
{
  store::Iterator_t attributes = getAttributes();
  attributes->open();
  store::Item_t at;
  bool found_lang = false;
  while ( !found_lang && attributes->next( at ) ) 
  {
    Item const *const name = at->getNodeName();
    if ( name->getLocalName() == "lang" && name->getNamespace() == XML_NS ) 
    {
      tokenizer.push_lang( locale::find_lang( at->getStringValue().c_str() ) );
      found_lang = true;
    }
  }
  attributes->close();

  tokenizer.push_element( this );
  InternalNode::tokenize( tokenizer );
  tokenizer.pop_element();
  if ( found_lang )
    tokenizer.pop_lang();
}


void TextNode::tokenize( XmlNodeTokenizer &tokenizer ) 
{
  tokenizer.beginTokenization( *this );
  const zstring& xText = getText();
  tokenizer.tokenize( xText.c_str(), xText.size() );
  tokenizer.endTokenization( *this );
}


FTTokenIterator_t
XmlNode::getDocumentTokens( locale::iso639_1::type lang ) const 
{
  XmlTree::FTTokens &tokens = getTree()->getTokens();
  if ( !hasTokens() ) 
  {
    std::auto_ptr<Tokenizer> tokenizer( Tokenizer::create() );
    XmlNodeTokenizer xml_tokenizer( *tokenizer, tokens, lang );
    getRoot()->tokenize( xml_tokenizer );
  }
  return FTTokenIterator_t(
    new NaiveFTTokenIterator( tokens, theBeginTokenIndex, theEndTokenIndex )
  );
}

#endif /* ZORBA_NO_FULL_TEXT */

} // namespace store
} // namespace zorba
/* vim:set et sw=2 ts=2: */
