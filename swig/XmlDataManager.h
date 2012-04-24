/*
 * Copyright 2006-2012 The FLWOR Foundation.
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
#ifndef API_XML_DATA_MANAGER_H
#define API_XML_DATA_MANAGER_H

class XmlDataManager
{
private:
  zorba::XmlDataManager* theManager;

public:
  XmlDataManager(const XmlDataManager& aMgr) : theManager(aMgr.theManager) {}
  XmlDataManager(zorba::XmlDataManager* aMgr) : theManager(aMgr) {}

  DocumentManager getDocumentManager();
  CollectionManager getCollectionManager();
  CollectionManager getW3CCollectionManager();
  Iterator parseXML(const std::string& aDoc);

};
#endif