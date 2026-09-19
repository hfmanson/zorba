#include <fstream>
#include <iostream>

#include <zorba/zorba.h>
#include <zorba/store_manager.h>
#include <api/unmarshaller.h>
#include <store/naive/node_items.h>

#include "dotnet.h"

using namespace std;

std::string getCurrentDirectoryFileURL();

DomFacadeCallbacksNative g_facade;
void* lStore;
Zorba* lZorba;
StaticContext_t sctx;
Item* docItem = NULL;

extern "C" ZORBA_DLL_PUBLIC void InitEngine(DomFacadeCallbacksNative callbacks, const char* xmlFile)
{
    g_facade = callbacks;
    lStore = StoreManager::getStore();
    lZorba = Zorba::getInstance(lStore);
    sctx = lZorba->createStaticContext();
    sctx->setBaseURI(getCurrentDirectoryFileURL());

    if (xmlFile)
    {
        ifstream is(xmlFile);
        XmlDataManager_t xmlMgr = lZorba->getXmlDataManager();
        docItem = new Item(xmlMgr->parseXML(is));
    }
}

extern "C" ZORBA_DLL_PUBLIC void ShutdownEngine()
{
    delete docItem;
    sctx = nullptr;
    lZorba->shutdown();
    StoreManager::shutdownStore(lStore);

    lZorba = nullptr;
    lStore = nullptr;
}

extern "C" ZORBA_DLL_PUBLIC void RunXQuery(const char* xquery)
{
    XQuery_t lQuery = lZorba->compileQuery(xquery, sctx);
    if (docItem)
    {
        DynamicContext* const dctx = lQuery->getDynamicContext();
        dctx->setContextItem(*(Item*)docItem);
    }

    //lQuery->execute();
    std::cout << lQuery << std::endl;
}

extern "C" ZORBA_DLL_PUBLIC NodeHandle GetDocument()
{
    NodeHandle nodeHandle = NULL;
    if (docItem)
    {
        zorba::simplestore::XmlNode* root = reinterpret_cast<zorba::simplestore::XmlNode*>(Unmarshaller::getInternalItem(*(Item*)docItem));
        nodeHandle = root->getNodeHandle();
    }
    return nodeHandle;
}
