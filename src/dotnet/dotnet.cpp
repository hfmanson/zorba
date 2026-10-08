#include <fstream>
#include <iostream>

#include <zorba/zorba.h>
#include <zorba/store_manager.h>
#include <api/unmarshaller.h>
#include <store/naive/node_items.h>
#include <zorba/config.h>

#include "dotnet.h"

using namespace std;

std::string GetNativePackageDirectoryA();
std::string PathToURL(std::string path);

DomFacadeCallbacksNative g_facade;

static void* lStore = NULL;
static Zorba* lZorba = NULL;
static StaticContext_t sctx;
static std::string assetsDirectory;

extern "C" ZORBA_DLL_PUBLIC void InitEngine(DomFacadeCallbacksNative callbacks, const char* prolog)
{
    try
    {
        ShutdownEngine();
        g_facade = callbacks;
        lStore = StoreManager::getStore();
        lZorba = Zorba::getInstance(lStore);
        sctx = lZorba->createStaticContext();
        assetsDirectory = GetNativePackageDirectoryA() + "Assets\\";
        std::string baseURI = PathToURL(assetsDirectory);
        sctx->setBaseURI(baseURI);
        Zorba_CompilerHints_t hints;
        zstring zstr(prolog);
        sctx->loadProlog(zstr.str(), hints);
    } catch (ZorbaException& e) {
            cerr << e << std::endl;
    }
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

extern "C" ZORBA_DLL_PUBLIC void* LoadXML(const char* xmlFile, bool validate)
{
    Item* result = nullptr;
#ifndef ZORBA_NO_XMLSCHEMA
	try
    {
        std::vector<String> aURIPaths;
        ifstream is(assetsDirectory + xmlFile);
        XmlDataManager_t xmlMgr = lZorba->getXmlDataManager();
        if (validate)
        {
            Item doc(xmlMgr->parseXML(is));
            Item validated;
            bool b = sctx->validate(doc, validated);
            result = new Item(validated);
        }
        else
        {
            result = new Item((xmlMgr->parseXML(is)));
        }
    }
    catch (ZorbaException& e) {
        cerr << e << std::endl;
    }
#else
	std::stringstream query;

	query << "doc('" << xmlFile << "')";
	XQuery_t lQuery = lZorba->compileQuery(query, sctx);
	Iterator_t lIterator = lQuery->iterator();
	lIterator->open();
	Item lRes;
	if (lIterator->next(lRes))
	{
		result = new Item(lRes);
	}
#endif
	return result;
}

extern "C" ZORBA_DLL_PUBLIC void FreeXML(void* docItem)
{
    delete (Item*)docItem;
}

extern "C" ZORBA_DLL_PUBLIC char* RunXQuery(const char* xquery, void* docItem)
{
    char* buf = nullptr;
    try
    {
        XQuery_t lQuery = lZorba->compileQuery(xquery, sctx);
        if (docItem)
        {
            DynamicContext* const dctx = lQuery->getDynamicContext();
            dctx->setContextItem(*(Item*)docItem);
        }


        if (lQuery->isUpdating())
        {
            lQuery->execute();
        }
        else
        {
            std::stringstream ss;
            ss << lQuery << std::endl;

            std::string s = ss.str();
            buf = new char[s.size() + 1];
            memcpy(buf, s.c_str(), s.size() + 1);
        }
    }
    catch (ZorbaException& e) {
        std::stringstream ss;
        ss << e << std::endl;

        std::string s = ss.str();
        buf = new char[s.size() + 1];
        memcpy(buf, s.c_str(), s.size() + 1);
    }
    return buf;
}

extern "C" ZORBA_DLL_PUBLIC void FreeString(char* p)
{
    delete[]p;
}

extern "C" ZORBA_DLL_PUBLIC NodeHandle GetXDocumentHandle(void* docItem)
{
    zorba::simplestore::XmlNode* root = reinterpret_cast<zorba::simplestore::XmlNode*>(Unmarshaller::getInternalItem(*(Item*)docItem));
    return root->getNodeHandle();
}
