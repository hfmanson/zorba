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
void* lStore = NULL;
Zorba* lZorba = NULL;
StaticContext_t sctx;

extern "C" ZORBA_DLL_PUBLIC void InitEngine(DomFacadeCallbacksNative callbacks)
{
    ShutdownEngine();
    g_facade = callbacks;
    lStore = StoreManager::getStore();
    lZorba = Zorba::getInstance(lStore);
    sctx = lZorba->createStaticContext();
    sctx->setBaseURI(getCurrentDirectoryFileURL());
}

extern "C" ZORBA_DLL_PUBLIC void ShutdownEngine()
{
    sctx = nullptr;
    if (lZorba)
    {
        lZorba->shutdown();
        lZorba = nullptr;
    }
    if (lStore)
    {
        StoreManager::shutdownStore(lStore);
        lStore = nullptr;
    }
}

extern "C" ZORBA_DLL_PUBLIC void* LoadXML(const char* xmlFile)
{
    ifstream is(xmlFile);
    XmlDataManager_t xmlMgr = lZorba->getXmlDataManager();
    return new Item(xmlMgr->parseXML(is));
}

extern "C" ZORBA_DLL_PUBLIC void FreeXML(void* docItem)
{
        delete (Item*)docItem;
}

extern "C" ZORBA_DLL_PUBLIC void RunXQuery(const char* xquery, void* docItem)
{
    XQuery_t lQuery = lZorba->compileQuery(xquery, sctx);
    if (docItem)
    {
        DynamicContext* const dctx = lQuery->getDynamicContext();
        dctx->setContextItem(*(Item*)docItem);
    }

    lQuery->execute();
    //std::cout << lQuery << std::endl;
}

extern "C" ZORBA_DLL_PUBLIC NodeHandle GetXDocumentHandle(void* docItem)
{
    zorba::simplestore::XmlNode* root = reinterpret_cast<zorba::simplestore::XmlNode*>(Unmarshaller::getInternalItem(*(Item*)docItem));
    return root->getNodeHandle();
}
