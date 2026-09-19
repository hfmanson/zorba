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
extern "C" EXPORT void SetDomFacade(DomFacadeCallbacksNative callbacks)
{
    g_facade = callbacks;
}

namespace zorba
{
    static NodeHandle runxq(const char* xquery)
    {
        void* lStore = StoreManager::getStore();
        Zorba* lZorba = Zorba::getInstance(lStore);
        StaticContext_t sctx = lZorba->createStaticContext();
        sctx->setBaseURI(getCurrentDirectoryFileURL());
        XQuery_t lQuery = lZorba->compileQuery(xquery, sctx);
        DynamicContext* const dctx = lQuery->getDynamicContext();
        ifstream is("henri.xml");
        XmlDataManager_t xmlMgr = lZorba->getXmlDataManager();
        Item doc(xmlMgr->parseXML(is));
        dctx->setContextItem(doc);
        zorba::simplestore::XmlNode* root = reinterpret_cast<zorba::simplestore::XmlNode*>(Unmarshaller::getInternalItem(doc));
        NodeHandle handle = root->getNodeHandle();

        lQuery->execute();
        //std::cout << lQuery << std::endl;
        return handle;
    }
}

extern "C" EXPORT NodeHandle RunXQuery(const char* xquery)
{
    return zorba::runxq(xquery);
}
