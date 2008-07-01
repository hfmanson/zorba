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
#ifndef ZORBA_MINIMAL_STORE_NODE_ITEMS
#define ZORBA_MINIMAL_STORE_NODE_ITEMS

#include <zorba/error.h>
#include "common/shared_types.h"
#include "zorbamisc/config/platform.h"
#include "zorbautils/fatal.h"
#include "store/api/item.h"
#include "store/minimal/min_ordpath.h"
#include "store/minimal/min_node_vector.h"
#include "store/minimal/min_node_updates.h"
#include "store/minimal/min_loader.h"

namespace zorba 
{  
  
namespace store
{
class CopyMode;
}

namespace storeminimal 
{
class AttributeNode;
class NsBindingsContext;

typedef rchandle<NsBindingsContext> NsBindingsContext_t;

extern ConstrNodeVector dummyVector;


#define NODE_STOP \
  ZORBA_FATAL(0, "Invalid method invocation on " \
              << store::StoreConsts::toString(getNodeKind()))


/*******************************************************************************

********************************************************************************/
class XmlTree
{
protected:
  mutable long              theRefCount;
  SYNC_CODE(mutable RCLock  theRCLock;)

  ulong             theId;
  XmlNode         * theRootNode;

public:
  XmlTree(XmlNode* root, ulong id);

  ~XmlTree() { theRootNode = 0; }

  void free() throw();

  long getRefCount() const      { return theRefCount; }
  long& getRefCount()           { return theRefCount; }
  void addReference()           { ++theRefCount; }
  void removeReference()        { --theRefCount; }

  SYNC_CODE(RCLock& getRCLock() const { return theRCLock; })

  ulong getId() const           { return theId; }
  XmlNode* getRoot() const      { return theRootNode; }
  void setRoot(XmlNode* root)   { theRootNode = root; }
};



/*******************************************************************************

  Note: No synchronization is required for access to theTree and theOrdPath data
  members, because once set, their value cannot change again, and furthermore,
  the setting of these data members occurs within the thread that created the
  node, before the node becomes visible to any other thread.

********************************************************************************/
class XmlNode : public store::Item
{
  friend class DocumentNode;
  friend class ElementNode;
  friend class ElementTreeNode;
  friend class ElementDagNode;
  friend class AttributeNode;
  friend class TextNode;
  friend class PiNode;
  friend class CommentNode;
  friend class UpdDelete;
  friend class UpdInsertSiblings;
  friend class UpdReplaceChild;
  friend class UpdReplaceContent;
  friend class UpdReplaceAttribute;
  friend class UpdReplaceAttrValue;
  friend class UpdReplaceTextValue;
  friend class BasicItemFactory;

public:
  enum NodeFlags
  {
    HaveLocalBindings =   2,
    IsId              =   4,
    IsIdRefs          =   8,
    IsBaseUri         =  32,
    IsHidden          =  64
  };

protected:
  //XmlTree  * theTree;
  OrdPath      theOrdPath;
  XmlNode    * theParent;

public:
  XmlNode() : Item(), theParent(NULL) { }

  XmlNode(
        XmlTree*              tree,
        XmlNode*              parent,
        long                  pos,
        store::StoreConsts::NodeKind nodeKind);

  virtual ~XmlNode();

  void free() { if (getTree() != NULL) getTree()->free(); }

  //
  // Item methods
  //

  bool isNode() const       { return true; }
  bool isAtomic() const     { return false; }
  bool isPul() const        { return false; }

  store::Item* getParent() const   { return theParent; }

  bool equals(
        const store::Item* other,
        long timezone = 0,
        XQPCollator* aCollation = 0) const
  {
    return this == other;
  }

  uint32_t hash(
        long timezone = 0,
        XQPCollator* aCollation = 0) const;

  store::Item* copy(
        store::Item*           parent,
        long            pos,
        const store::CopyMode& copymode) const
  {
    return copy2(static_cast<XmlNode*>(parent),
                 static_cast<XmlNode*>(parent),
                 pos,
                 copymode);
  }

  store::Item_t getEBV() const;

  xqpStringStore_t getBaseURI() const
  {
    bool local = false;
    return getBaseURIInternal(local);
  }

  virtual xqpStringStore* getDocumentURI() const { return 0; }

  virtual store::Item_t getNilled() const { return 0; }

  virtual void finalizeNode() { return; }

  //
  // SimpleStore Methods
  //

  void setParent(XmlNode* p)        { theParent = p; }

  void setTree(const XmlTree* t);
  XmlTree* getTree() const          { return (XmlTree*)theTreeRCPtr; }
  ulong getTreeId() const           { return getTree()->getId(); }
  const OrdPath& getOrdPath() const { return theOrdPath; }
  OrdPath& getOrdPath()             { return theOrdPath; }

  void setId(XmlTree* tree, const OrdPathStack* op);

  void setOrdPath(
    XmlNode* parent, 
    long pos,
    store::StoreConsts::NodeKind nodeKind);

  void switchTree(
        XmlNode*        parent,
        ulong           pos,
        const store::CopyMode& copymode) throw();

  void deleteTree() throw();

  void setToUntyped();
  void removeType(TypeUndoList& undoList);
  void restoreType(const TypeUndoList& undoList);
  void revalidate();

  void removeChildren(
        ulong  pos,
        ulong  numChildren);

  void insertChildren(
        std::vector<store::Item_t>& newChildren,
        ulong                pos,
        bool                 copy,
        const store::CopyMode&      copymode);
  
  void insertChildrenFirst(
        std::vector<store::Item_t>& newChildren,
        bool                 copy,
        const store::CopyMode&      copymode);

  void insertChildrenLast(
        std::vector<store::Item_t>& newChildren,
        bool                 copy,
        const store::CopyMode&      copymode);

  void insertSiblingsBefore(
        std::vector<store::Item_t>& siblings,
        bool                 copy,
        const store::CopyMode&      copymode);

  void insertSiblingsAfter(
        std::vector<store::Item_t>& siblings,
        bool                 copy,
        const store::CopyMode&      copymode);

  void replaceChild(
        std::vector<store::Item_t>& newChildren,
        ulong                pos,
        bool                 copy,
        const store::CopyMode&      copymode);

  virtual XmlNode* copy2(
        XmlNode*        rootParent,
        XmlNode*        parent,
        long            pos,
        const store::CopyMode& copyMode) const = 0;

  virtual ulong numAttributes() const          { return 0; }
  virtual AttributeNode* getAttr(ulong i) const{ NODE_STOP; return NULL; }
  virtual NodeVector& attributes()             { NODE_STOP; return dummyVector; }
  virtual const NodeVector& attributes() const { NODE_STOP; return dummyVector; }

  virtual ulong numChildren() const            { return 0; }
  virtual XmlNode* getChild(ulong i) const     { NODE_STOP; return NULL; }
  virtual NodeVector& children()               { NODE_STOP; return dummyVector; }
  virtual const NodeVector& children() const   { NODE_STOP; return dummyVector; }

  virtual NsBindingsContext* getNsContext() const   { NODE_STOP; return NULL; }
  virtual void setNsContext(NsBindingsContext* ctx) { NODE_STOP; }

  virtual bool haveLocalBindings() const { NODE_STOP; return false; }

  virtual bool isId() const              { NODE_STOP; return false; }
  virtual bool isIdRefs() const          { NODE_STOP; return false; }

  virtual XmlLoader_t hasLoaderAttached() const         {return NULL;}
  virtual unsigned int        getDepth()         {return 0;}//depth is usefull only for loaded element nodes
protected:
  virtual xqpStringStore_t getBaseURIInternal(bool& local) const;

  ulong disconnect() throw();
  void connect(XmlNode* node, ulong pos) throw();
  void removeChild(ulong pos);
  bool removeChild(XmlNode* child);
  void removeAttr(ulong pos);
  bool removeAttr(XmlNode* attr);
};


/*******************************************************************************

********************************************************************************/
class DocumentNode : public XmlNode
{
protected:
  xqpStringStore_t    theBaseUri;
  xqpStringStore_t    theDocUri;

public:
  DocumentNode(
        xqpStringStore_t& baseUri,
        xqpStringStore_t& docUri);

  DocumentNode(
        XmlTree*          tree,
        xqpStringStore_t& baseUri,
        xqpStringStore_t& docUri);

  virtual ~DocumentNode();

  //
  // Item methods
  //

  store::StoreConsts::NodeKind getNodeKind() const 
  {
    return store::StoreConsts::documentNode;
  }

  store::Item* getType() const; 

  xqpStringStore* getDocumentURI() const { return theDocUri.getp(); }

  store::Iterator_t getChildren() const;

  store::Iterator_t getTypedValue() const;
  store::Item_t getAtomizationValue() const;
  xqpStringStore_t getStringValue() const;

  xqp_string show() const;

  //
  // SimpleStore Methods
  // 

  XmlNode* copy2(
        XmlNode*        rootParent,
        XmlNode*        parent,
        long            pos,
        const store::CopyMode& copyMode) const;

  NsBindingsContext* getNsContext() const                { return NULL; }
  xqpStringStore* findBinding(xqpStringStore* pre) const { return NULL; }

protected:
  xqpStringStore_t getBaseURIInternal(bool& local) const;
};


/*******************************************************************************

********************************************************************************/
class DocumentTreeNode : public DocumentNode
{
private:
  LoadedNodeVector theChildren;
public:
  XmlLoader_t   attachedloader;//for documents and elements might be false. Means it is full loaded

  DocumentTreeNode(
        xqpStringStore_t& baseUri,
        xqpStringStore_t& docUri);

  DocumentTreeNode(
        XmlTree*          tree,
        xqpStringStore_t& baseUri,
        xqpStringStore_t& docUri);

  ulong numChildren() const          { return theChildren.size(); }
  NodeVector& children()             { return theChildren; }
  const NodeVector& children() const { return theChildren; }
  XmlNode* getChild(ulong i) const   { return theChildren.get(i); }

  void finalizeNode()                { theChildren.compact(); }
  virtual XmlLoader_t hasLoaderAttached() const          {return attachedloader;}
};


/*******************************************************************************

********************************************************************************/
class DocumentDagNode : public DocumentNode
{
protected:
  ConstrNodeVector theChildren;

public:
  DocumentDagNode(
        XmlTree*          tree,
        xqpStringStore_t& baseUri,
        xqpStringStore_t& docUri);
        
  ulong numChildren() const          { return theChildren.size(); }
  NodeVector& children()             { return theChildren; }
  const NodeVector& children() const { return theChildren; }
  XmlNode* getChild(ulong i) const   { return theChildren.get(i); }

  void finalizeNode()                { theChildren.compact(); }
};


/*******************************************************************************

********************************************************************************/
class ElementNode : public XmlNode
{
  friend class XmlNode;
  friend class ElementTreeNode;
  friend class AttributeNode;
public:
  store::Item_t                theName;
protected:
  store::Item_t                theTypeName;
  NsBindingsContext_t   theNsContext;
  uint32_t              theFlags;

public:
  ElementNode(
        store::Item_t& nodeName,
        store::Item_t& typeName);

  ElementNode(
        XmlTree*          tree,
        XmlNode*          parent,
        long              pos,
        store::Item_t&           nodeName,
        store::Item_t&           typeName,
        const store::NsBindings* localBindings);

  virtual ~ElementNode();

  //
  // Item methods
  //

  store::StoreConsts::NodeKind getNodeKind() const 
  {
    return store::StoreConsts::elementNode;
  }
  store::Item* getType() const                     { return theTypeName.getp(); }
  store::Item* getNodeName() const                 { return theName.getp(); }

  store::Iterator_t getTypedValue() const;
  store::Item_t getAtomizationValue() const;
  xqpStringStore_t getStringValue() const;
  store::Item_t getNilled() const;

  store::Iterator_t getAttributes() const;
  store::Iterator_t getChildren() const;

  void getNamespaceBindings(
        store::NsBindings& bindings,
        store::StoreConsts::NsScoping scope = store::StoreConsts::ALL_NAMESPACES) const;

  xqp_string show() const;

  //
  // SimpleStore Methods
  // 

  bool isId() const             { return (theFlags & XmlNode::IsId) != 0; }
  void resetIsId()              { theFlags &= ~XmlNode::IsId; }
  bool isIdRefs() const         { return (theFlags & XmlNode::IsIdRefs) != 0; }
  void resetIsIdRefs()          { theFlags &= ~XmlNode::IsIdRefs; }
  bool haveBaseUri() const      { return (theFlags & XmlNode::IsBaseUri); }
  void setHaveBaseUri()         { theFlags |= XmlNode::IsBaseUri; }
  void resetHaveBaseUri()       { theFlags &= ~XmlNode::IsBaseUri; }

  bool haveLocalBindings() const{ return (theFlags & XmlNode::HaveLocalBindings) != 0; }

  NsBindingsContext* getNsContext() const { return theNsContext.getp(); }

  void setNsContext(NsBindingsContext* ctx);
  xqpStringStore* findBinding(const xqpStringStore* prefix) const;
  const store::NsBindings& getLocalBindings() const;
  void addLocalBinding(xqpStringStore* prefix, xqpStringStore* ns);
  void addBindingForQName(store::Item_t& qname);
  void addBindingForQName2(const store::Item* qname);

  void checkNamespaceConflict(
        const store::Item*           qname,
        XQUERY_ERROR ecode) const;

  void checkUniqueAttr(const store::Item* attrName) const;

  XmlNode* copy2(
        XmlNode*        rootParent,
        XmlNode*        parent,
        long            pos,
        const store::CopyMode& copymode) const;

  void removeAttributes(
        ulong  pos,
        ulong  numAttributes);

  void insertAttributes(
        std::vector<store::Item_t>&  newAttrs,
        bool                  copy,
        const store::CopyMode&       copymode);

  void replaceAttribute(
        std::vector<store::Item_t>& newAttrs,
        ulong                pos,
        bool                 copy,
        const store::CopyMode&      copymode);

  void replaceContent(
        XmlNode*          newText,
        ConstrNodeVector& oldChildren,
        bool              copy,
        const store::CopyMode&   copymode);

  void rename(store::Item_t& newname, store::Item_t& oldName);

protected:
  void addBaseUriProperty(
        xqpStringStore_t& absUri,
        xqpStringStore_t& relUri);

  void adjustBaseUriProperty(
        AttributeNode*    attr,
        xqpStringStore_t& absUri,
        xqpStringStore_t& relUri);
};


/*******************************************************************************

********************************************************************************/
class ElementTreeNode : public ElementNode
{
  friend class ElementNode;
  friend class UpdReplaceContent;

protected:
  LoadedNodeVector  theChildren;
  LoadedNodeVector  theAttributes;

public:
  XmlLoader_t   attachedloader;//for documents and elements might be false. Means it is full loaded
  unsigned int           depth;
public:
  ElementTreeNode(
        store::Item_t& nodeName,
        store::Item_t& typeName,
        ulong          numBindings,
        ulong          numAttributes);

  ElementTreeNode(
        XmlTree*          tree,
        XmlNode*          parent,
        long              pos,
        store::Item_t&           nodeName,
        store::Item_t&           typeName,
        const store::NsBindings* localBindings,
        xqpStringStore_t& baseUri);

  ulong numAttributes() const          { return theAttributes.size(); }
  NodeVector& attributes()             { return theAttributes; }
  const NodeVector& attributes() const { return theAttributes; }

  AttributeNode* getAttr(ulong i) const
  {
    return reinterpret_cast<AttributeNode*>(theAttributes.get(i));
  }

  ulong numChildren() const            { return theChildren.size(); }
  NodeVector& children()               { return theChildren; }
  const NodeVector& children() const   { return theChildren; }
  XmlNode* getChild(ulong i) const     { return theChildren.get(i); }

  void finalizeNode()
  {
    theChildren.compact();
    theAttributes.compact();
  }

  virtual XmlLoader_t hasLoaderAttached() const        {return attachedloader;}
  virtual unsigned int        getDepth()         {return depth;}

protected:
  xqpStringStore_t getBaseURIInternal(bool& local) const;

private:
  //disable default copy constructor
  ElementTreeNode(const ElementTreeNode& src);

};


/*******************************************************************************

********************************************************************************/
class ElementDagNode : public ElementNode
{
  friend class UpdReplaceContent;

protected:
  ConstrNodeVector  theChildren;
  ConstrNodeVector  theAttributes;

public:
  ElementDagNode(
        XmlTree*          tree,
        XmlNode*          parent,
        long              pos,
        store::Item_t&           nodeName,
        store::Item_t&           typeName,
        const store::NsBindings* localBindings,
        xqpStringStore_t& baseUri);

  ulong numAttributes() const          { return theAttributes.size(); }
  NodeVector& attributes()             { return theAttributes; }
  const NodeVector& attributes() const { return theAttributes; }

  AttributeNode* getAttr(ulong i) const
  {
    return reinterpret_cast<AttributeNode*>(theAttributes.get(i)); 
  }

  ulong numChildren() const            { return theChildren.size(); }
  NodeVector& children()               { return theChildren; }
  const NodeVector& children() const   { return theChildren; }
  XmlNode* getChild(ulong i) const     { return theChildren.get(i); }

  void finalizeNode()
  {
    theChildren.compact();
    theAttributes.compact();
  }

protected:
  xqpStringStore_t getBaseURIInternal(bool& local) const;

private:
  //disable default copy constructor
  ElementDagNode(const ElementDagNode& src);
};


/*******************************************************************************

********************************************************************************/
class AttributeNode : public XmlNode
{
  friend class XmlNode;
  friend class ElementNode;
  friend class ConstrElementNode;

protected:
  store::Item_t   theName;
  store::Item_t   theTypeName;
  store::Item_t   theTypedValue;
  uint32_t theFlags;

public:
  AttributeNode(
        store::Item_t&  attrName,
        store::Item_t&  typeName,
        bool            isIdrefs);

  AttributeNode(
        XmlTree*  tree,
        XmlNode*  parent,
        long      pos,
        store::Item_t&   attrName,
        store::Item_t&   typeName,
        store::Item_t&   typedValue,
        bool      hidden = false);

  virtual ~AttributeNode();


  XmlNode* copy2(
        XmlNode*        rootParent,
        XmlNode*        parent,
        long            pos,
        const store::CopyMode& copymode) const;

  store::StoreConsts::NodeKind getNodeKind() const 
  { 
    return store::StoreConsts::attributeNode; 
  }

  store::Item* getType() const       { return theTypeName.getp(); }

  store::Item* getNodeName() const   { return theName.getp(); }

  bool isId() const           { return (theFlags & XmlNode::IsId) != 0; }
  void resetIsId()            { theFlags &= ~XmlNode::IsId; }

  bool isIdRefs() const       { return (theFlags & XmlNode::IsIdRefs) != 0; }
  void resetIsIdRefs()        { theFlags &= ~XmlNode::IsIdRefs; }

  bool isHidden() const       { return (theFlags & XmlNode::IsHidden) != 0; }
  void setHidden()            { theFlags |= XmlNode::IsHidden; }

  bool isBaseUri() const      { return (theFlags & XmlNode::IsBaseUri) != 0; }

  store::Iterator_t getTypedValue() const;
  store::Item_t getAtomizationValue() const;
  xqpStringStore_t getStringValue() const;

  xqp_string show() const;

  void replaceValue(xqpStringStore_t& newValue, xqpStringStore_t& oldValue);

  void rename(store::Item_t& newname, store::Item_t& oldName);
};
   

/*******************************************************************************

********************************************************************************/
class TextNode : public XmlNode
{
  friend class XmlNode;
  friend class DocumentDagNode;
  friend class ConstrElementNode;
  friend class BasicItemFactory;

protected:
  xqpStringStore_t theContent;

public:
  TextNode(xqpStringStore_t& content);

  TextNode(
        XmlTree*          tree,
        XmlNode*          parent,
        long              pos,
        xqpStringStore_t& content);

  virtual ~TextNode();

  XmlNode* copy2(
        XmlNode*        rootParent,
        XmlNode*        parent,
        long            pos,
        const store::CopyMode& copymode) const;

  store::StoreConsts::NodeKind getNodeKind() const 
  { 
    return store::StoreConsts::textNode;
  }

  store::Item* getType() const;

  store::Iterator_t getTypedValue() const;
  store::Item_t getAtomizationValue() const;
  xqpStringStore_t getStringValue() const   { return theContent; }
  xqpStringStore* getStringValueP() const   { return theContent.getp(); }
			
  xqp_string show() const;

  void replaceValue(xqpStringStore_t& newValue, xqpStringStore_t& oldValue);
};


/*******************************************************************************

********************************************************************************/
class PiNode : public XmlNode
{
  friend class XmlNode;

 protected:
  xqpStringStore_t theTarget;
  xqpStringStore_t theContent;

public:
  PiNode(xqpStringStore_t& target, xqpStringStore_t& content);

  PiNode(
        XmlTree*          tree,
        XmlNode*          parent,
        long              pos,
        xqpStringStore_t& target,
        xqpStringStore_t& content);

  ~PiNode();

  XmlNode* copy2(
        XmlNode*        rootParent,
        XmlNode*        parent,
        long            pos,
        const store::CopyMode& copymode) const;

  store::StoreConsts::NodeKind getNodeKind() const 
  { 
    return store::StoreConsts::piNode; 
  }

  store::Item* getType() const;

  store::Iterator_t getTypedValue() const;
  store::Item_t getAtomizationValue() const;
  xqpStringStore_t getStringValue() const    { return theContent; }
  xqpStringStore* getStringValueP() const    { return theContent.getp(); }

  xqpStringStore* getTarget() const { return theTarget.getp(); }

  xqp_string show() const;

  void replaceValue(xqpStringStore_t& newValue, xqpStringStore_t& oldValue);

  void rename(xqpStringStore_t& newName, xqpStringStore_t& oldName);
};


/*******************************************************************************

********************************************************************************/
class CommentNode : public XmlNode
{
  friend class XmlNode;

protected:
  xqpStringStore_t theContent;

public:
  CommentNode(xqpStringStore_t& content);

  CommentNode(
        XmlTree*          tree,
        XmlNode*          parent,
        long              pos,
        xqpStringStore_t& content);

  ~CommentNode();

  XmlNode* copy2(
        XmlNode*        rootParent,
        XmlNode*        parent,
        long            pos,
        const store::CopyMode& copymode) const;

  store::StoreConsts::NodeKind getNodeKind() const 
  { 
    return store::StoreConsts::commentNode; 
  }

  store::Item* getType() const;

  store::Iterator_t getTypedValue() const;
  store::Item_t getAtomizationValue() const;
  xqpStringStore_t getStringValue() const   { return theContent; }
  xqpStringStore* getStringValueP() const   { return theContent.getp(); }

  xqp_string show() const;

  void replaceValue(xqpStringStore_t& newValue, xqpStringStore_t& oldValue);
};


} // namespace storeminimal
} // namespace zorba

#endif
