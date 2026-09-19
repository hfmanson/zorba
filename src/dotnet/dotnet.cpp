#include <fstream>
#include <iostream>

#include <zorba/zorba.h>
#include <zorba/store_manager.h>

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
    static void runxq(const char* xquery)
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

        //lQuery->execute();
        std::cout << lQuery << std::endl;
    }
}

extern "C" EXPORT void RunXQuery(const char* xquery)
{
    zorba::runxq(xquery);
}
