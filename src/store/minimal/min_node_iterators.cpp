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
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or  implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "zorbaerrors/Assert.h"

#include "store/minimal/min_node_items.h"
#include "store/minimal/min_node_iterators.h"
#include "store/minimal/min_store_defs.h"
#include <algorithm>

namespace zorba 
{ 
  namespace storeminimal 
  {

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class ChildrenIterator                                                     //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

bool ChildrenIteratorImpl::next(store::Item_t& result)
{
  if (theCurrentPos >= theNumChildren) 
  {
    result = NULL;
    return false;
  }

  result = theParentNode->getChild(theCurrentPos);

  theCurrentPos++;

  return true;
}



/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class ChildrenIteratorLazy                                                 //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

ChildrenIteratorLazy::ChildrenIteratorLazy(InternalNode* parent)
  :
  theParentNode(parent),
  theCurrentPos(0)
{
  ZORBA_ASSERT(theParentNode->getNodeKind() == store::StoreConsts::documentNode ||
               theParentNode->getNodeKind() == store::StoreConsts::elementNode);

  //theNumChildren = parent->numChildren();
}

void ChildrenIteratorLazy::open()
{
  theCurrentPos = 0;
}


bool ChildrenIteratorLazy::next(store::Item_t& result)
{
  XmlLoader_t   xmlloader;
  while(!theParentNode->isFullLoaded())
  {
    if(theCurrentPos >= theParentNode->numChildren())
    {
      if(xmlloader == NULL)
        xmlloader = theParentNode->getTree()->attachedloader;
      xmlloader->continueloadXml(XmlLoader::UNTIL_START_ELEMENT, theParentNode->getDepth()+1);
    }
    else
      break;
  }

  if(theCurrentPos >= theParentNode->numChildren())
  {
    result = NULL;
    return false;
  }


  result = theParentNode->getChild(theCurrentPos);

  theCurrentPos++;

  return true;
}


void ChildrenIteratorLazy::reset()
{
  theCurrentPos = 0;
}


void ChildrenIteratorLazy::close()
{
  theCurrentPos = 0;
  theParentNode = NULL;
}


/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class AttributesIterator                                                   //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

bool AttributesIteratorImpl::next(store::Item_t& result)
{
  if (theCurrentPos >= theNumAttributes) 
  {
    result = NULL;
    return false;
  }

  AttributeNode* cnode =
    reinterpret_cast<AttributeNode*>(theParentNode->getAttr(theCurrentPos));

  while (cnode->isHidden())
  {
    theCurrentPos++;

    if (theCurrentPos >= theNumAttributes) 
    {
      result = NULL;
      return false;
    }

    cnode = reinterpret_cast<AttributeNode*>(theParentNode->getAttr(theCurrentPos));
  }

  theCurrentPos++;

  result = cnode;
  return true;
}



/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class NodeDistinctIterator                                                 //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

void StoreNodeDistinctIterator::open()
{
}


bool StoreNodeDistinctIterator::next(store::Item_t& result)
{
  while (true)
  {
    if (!theInput->next(result))
      return false;

    ZORBA_ASSERT(result->isNode());

    if (theNodeSet.insert(result))
      return true;
  }
}


void StoreNodeDistinctIterator::reset()
{
  // Do not reset the input. This is done by the runtime NodeDistinctIterator,
  // which wraps this store iterator.

  theNodeSet.clear();
}


void StoreNodeDistinctIterator::close()
{
  // Do not close the input. This is done by the runtime NodeDistinctIterator,
  // which wraps this store iterator.

  theNodeSet.clear();
  theInput = NULL;
}


bool StoreNodeDistinctOrAtomicIterator::next(store::Item_t& result)
{
  store::Item_t contextNode;

  if (theAtomicMode) 
  {
    if (!theInput->next(result))
      return false;

    if (!result->isAtomic())
      ZORBA_ERROR (XPTY0018);

    return true;
  }

  if (!theInput->next(result))
    return false;

  if (result->isAtomic())
  {
    if (theNodeMode)
      ZORBA_ERROR (XPTY0018);

    theAtomicMode = true;
    return true;
  }
  else
  {
    theNodeMode = true;

    while (true)
    {
      if (theNodeSet.insert(result))
        return true;

      if (!theInput->next(result))
        return false;

      if (result->isAtomic())
        ZORBA_ERROR (XPTY0018);
    }
  }
}


/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class StoreNodeSortIterator                                                //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

void StoreNodeSortIterator::open()
{
  theCurrentNode = -1;
}


bool StoreNodeSortIterator::next(store::Item_t& result)
{
  if (theCurrentNode < 0)
  {
    theCurrentNode = 0;

    while (true)
    {
      store::Item_t contextNode;
      if (!theInput->next(contextNode))
        break;

      ZORBA_ASSERT(contextNode->isNode());

      theNodes.push_back(reinterpret_cast<XmlNode*>(contextNode.transfer()));
    }

    ComparisonFunction cmp(theAscending);

    std::sort(theNodes.begin(), theNodes.end(), cmp);
  }

  if (theCurrentNode < (long)theNodes.size())
  {
    if (theDistinct)
    {
      result = theNodes[theCurrentNode++];

      while (theCurrentNode < (long)theNodes.size() &&
             theNodes[theCurrentNode] == result)
      {
        theCurrentNode++;
      }

      return true;
    }
    else
    {
      result = theNodes[theCurrentNode++];
      return true;
    }
  }
  else
  {
    reset();
    result = NULL;
    return false;
  }
}


void StoreNodeSortIterator::reset()
{
  // Do not reset the input. This is done by the runtime NodeSortIterator,
  // which wraps this store iterator.

  ulong numNodes = theNodes.size();
  for (ulong i = 0; i < numNodes; i++)
  {
    XmlNode* n = theNodes[i];
    n->removeReference(n->getSharedRefCounter() SYNC_PARAM2(n->getRCLock()));
  }

  theNodes.clear();
  theCurrentNode = -1;
}


void StoreNodeSortIterator::close()
{
  // Do not close the input. This is done by the runtime NodeSortIterator,
  // which wraps this store iterator.

  ulong numNodes = theNodes.size();
  for (ulong i = 0; i < numNodes; i++)
  {
    XmlNode* n = theNodes[i];
    n->removeReference(n->getSharedRefCounter() SYNC_PARAM2(n->getRCLock()));
  }

  theNodes.clear();
  theCurrentNode = -1;
  theInput = NULL;
}


/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//  class StoreNodeSortOrAtomicIterator                                        //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

bool StoreNodeSortOrAtomicIterator::next(store::Item_t& result)
{
  if (theAtomicMode)
  {
    if (!theInput->next(result))
      return false;

    if (!result->isAtomic())
      ZORBA_ERROR (XPTY0018);

    return true;
  }

  if (theCurrentNode < 0)
  {
    theCurrentNode = 0;

    while (true)
    {
      if (!theInput->next(result))
        break;

      if (result->isAtomic())
      {
        if (theNodeMode)
          ZORBA_ERROR (XPTY0018);

        theAtomicMode = true;
        return true;
      }
      else
      {
        theNodeMode = true;
      }

      theNodes.push_back(reinterpret_cast<XmlNode*>(result.transfer()));
    }

    ComparisonFunction cmp;

    std::sort(theNodes.begin(), theNodes.end(), cmp);
  }

  if (theCurrentNode < (long)theNodes.size())
  {
    if (theDistinct)
    {
      result = theNodes[theCurrentNode++];

      while (theCurrentNode < (long)theNodes.size() &&
             theNodes[theCurrentNode] == result)
      {
        theCurrentNode++;
      }

      return true;
    }
    else
    {
      result = theNodes[theCurrentNode++];
      return true;
    }
  }
  else
  {
    reset();
    result = NULL;
    return false;
  }
 
}
} // namespace storeminimal
} // namespace zorba

