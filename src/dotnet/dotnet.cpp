#include <zorba/zorba.h>
#include <zorba/store_manager.h>

#include "dotnet.h"

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
        XQuery_t lQuery = lZorba->compileQuery(xquery);
        lQuery->execute();
        //std::cout << lQuery << std::endl;
    }
}

extern "C" EXPORT void RunXQuery(const char* xquery)
{
    zorba::runxq(xquery);
}
