///Created: Daniel Turcanu @ IPDevel 

#include <zorba/common/common.h>
#include <zorba/zorba_api.h>

#include "store/api/item_factory.h"
#include "store/api/store.h"

#include "errors/error_messages.h"
#include "errors/error_manager.h"

#include "system/globalenv.h"
#include "system/zorba_engine.h"
#include "system/zorba.h"

#include "api/external/xquery.h"
#include "api/external/static_context_wrapper.h"
#include "api/external/dynamic_context_wrapper.h"
#include "api/serialization/serializer.h"

#include "context/dynamic_context.h"
#include "context/static_context.h"
#include "api/external/xmldatamanager_impl.h"
#include "compiler/api/compiler_api.h"
#include "types/typemanager.h"

//#include "zorba/common/libxerces.h"

//XERCES_CPP_NAMESPACE_USE

namespace zorba{


rchandle<ZorbaEngineImpl>	 globalZorbaEngine = NULL;


void library_init();


ZorbaEngine_t ZorbaEngine::getInstance()
{
	if((globalZorbaEngine == NULL) || (globalZorbaEngine->is_shutdown))
	{
		globalZorbaEngine = new ZorbaEngineImpl(false);
		globalZorbaEngine->initialize();
	}
	return &*globalZorbaEngine;
}

ZorbaSingleThread_t ZorbaSingleThread::getInstance()
{
	if((globalZorbaEngine == NULL) || (globalZorbaEngine->is_shutdown))
	{
		globalZorbaEngine = new ZorbaEngineImpl(true);//single thread
		globalZorbaEngine->initialize();
		globalZorbaEngine->initThread();
	}
	return &*globalZorbaEngine;
}

ZorbaSingleThread::~ZorbaSingleThread()
{
}

Zorba* ZORBA_FOR_CURRENT_THREAD()
{
	if(globalZorbaEngine == NULL)
		return NULL;
	return globalZorbaEngine->getZorbaForCurrentThread();
}

ZorbaEngineImpl::ZorbaEngineImpl(bool single_thread)
{
  in_destructor = false;
	for_single_thread_api = single_thread;
	theSingleThreadZorba = NULL;
	xml_data_manager = new XmlDataManager_Impl;
	xml_data_manager->addReference(xml_data_manager->getRefCounter(),
                                 xml_data_manager->getSync());
}


ZorbaEngineImpl::~ZorbaEngineImpl()
{
	try{
  in_destructor = true;
  shutdown();

	xml_data_manager->removeReference(xml_data_manager->getRefCounter(),
                                    xml_data_manager->getSync());
//  assert(globalZorbaEngine == NULL);
	delete theSingleThreadZorba;

	}CATCH_ALL_NO_RETURN(;);
}


void ZorbaEngineImpl::initialize()
{
	try{
	if(!for_single_thread_api)
	{
#ifdef WIN32
  theThreadData = TlsAlloc();

#elif defined ZORBA_USE_PTHREAD_LIBRARY
  pthread_key_create(&theThreadData, NULL);

#elif defined ZORBA_USE_BOOST_THREAD_LIBRARY

#elif defined ZORBA_FOR_ONE_THREAD_ONLY
  theThreadData = NULL;//new Zorba();

#else
  #error Unsupported thread system

#endif
	}//end if(for_single_thread_api)

	dynamic_context::init();
	library_init();

	///optimize access to store
  Zorba::theStore = &GENV.getStore();
  Zorba::theItemFactory = Zorba::theStore->getItemFactory();
  GlobalEnvironment::getInstance();
  //init xerces
//  XMLPlatformUtils::Initialize();


  is_shutdown = false;

	}CATCH_ALL_NO_RETURN(;);
}


void ZorbaEngineImpl::shutdown()
{
	try
  {
  if (!is_shutdown)
  {
    is_shutdown = true;

    GlobalEnvironment::destroy();
		Zorba::theStore = NULL;
    Zorba::theItemFactory = NULL;

    if(!for_single_thread_api)
		{
#ifdef WIN32
    TlsFree(theThreadData);
    theThreadData = 0;

#elif defined ZORBA_USE_PTHREAD_LIBRARY
    pthread_key_delete(theThreadData);

#elif defined ZORBA_USE_BOOST_THREAD_LIBRARY

#elif defined ZORBA_FOR_ONE_THREAD_ONLY
    delete theThreadData;
    theThreadData = 0;

#else
    #error Unsupported thread system

#endif
		}//end if (for_single_thread_api)

    if(!in_destructor)
      globalZorbaEngine = NULL;//also deletes globalZorbaEngine
    //close xerces
//    XMLPlatformUtils::Terminate();
  }
	}CATCH_ALL_NO_RETURN(;);
}


void ZorbaEngineImpl::initThread()
{
	try{
	if(for_single_thread_api && theSingleThreadZorba)
		return;//ignore for single threaded

	Zorba* zorba = this->getZorbaForCurrentThread();

	if(zorba == 0)
  {
		zorba = new Zorba();

		if(!for_single_thread_api)
		{
#ifdef WIN32
    TlsSetValue(theThreadData, zorba);

#elif defined ZORBA_USE_PTHREAD_LIBRARY
    pthread_setspecific(theThreadData, zorba);

#elif defined ZORBA_USE_BOOST_THREAD_LIBRARY
    theThreadData = zorba;

#elif defined ZORBA_FOR_ONE_THREAD_ONLY
    //Assert(0);
		theThreadData = zorba;

#else
      #error Unsupported thread system
#endif
		}
		else//if (!for_single_thread_api)
		{
      delete theSingleThreadZorba;
			theSingleThreadZorba = zorba;
		}
  }
	}CATCH_ALL_NO_RETURN(;);
}


void ZorbaEngineImpl::uninitThread()
{
	try{
	if(for_single_thread_api)
		return;//ignore for single threaded

	Zorba* zorba = getZorbaForCurrentThread();

	if(zorba)
		delete zorba;

#ifdef WIN32
  TlsSetValue(theThreadData, NULL);

#elif defined ZORBA_USE_PTHREAD_LIBRARY
  pthread_setspecific(theThreadData, NULL);

#elif defined ZORBA_USE_BOOST_THREAD_LIBRARY
  theThreadData.release();

#elif defined ZORBA_FOR_ONE_THREAD_ONLY
	theThdreadData = NULL;
#else
  #error Unsupported thread system
#endif
	}CATCH_ALL_NO_RETURN(;);
}


Zorba* ZorbaEngineImpl::getZorbaForCurrentThread()
{
	if(for_single_thread_api)
		return theSingleThreadZorba;

#ifdef WIN32
  return (Zorba*)TlsGetValue(theThreadData);

#elif defined ZORBA_USE_PTHREAD_LIBRARY
  return (Zorba*)pthread_getspecific(theThreadData);

#elif defined ZORBA_USE_BOOST_THREAD_LIBRARY
  return theThreadData.get();

#elif defined ZORBA_FOR_ONE_THREAD_ONLY
  return theThreadData;

#else
#error Unsupported thread system
#endif
}


XQuery_t ZorbaEngineImpl::createQuery(
    xqp_string aQueryString,
    StaticQueryContext_t sctx, 
    xqp_string	xquery_source_uri,
    bool routing_mode,
    XQueryTreePlans_t planprint)
{
	try{
  rchandle<Zorba_XQueryBinary> xq(new Zorba_XQueryBinary(xquery_source_uri,
                                                         aQueryString));
	if (!xq->compile(sctx.getp(), routing_mode, planprint))
	{
		return NULL;
	}

		return &*xq;//.release();
	}CATCH_ALL_RETURN_NULL;
}

XQuery_t ZorbaEngineImpl::createQueryFromFile(
      xqp_string xquery_file,
      StaticQueryContext_t sctx,
      bool routing_mode,
      XQueryTreePlans_t planprint)
{/*
	try{
	FILE	*fquery;
	size_t	fsize;
	char	*xquerydata;
	XQuery_t	result_query;

	fquery = fopen(xquery_file.c_str(), "rb");
	if(!fquery)
	{
		ZORBA_ERROR_ALERT(ZorbaError::API0015_CANNOT_OPEN_FILE, NULL, DONT_CONTINUE_EXECUTION, xquery_file);
		return NULL;
	}
	if(fseek(fquery, 0, SEEK_END))
	{
		fclose(fquery);
		ZORBA_ERROR_ALERT(ZorbaError::API0018_CANNOT_ACCESS_FILE, NULL, DONT_CONTINUE_EXECUTION, xquery_file);
		return NULL;
	}
	fsize = ftell(fquery);
	if(fsize <= 0)
	{
		fclose(fquery);
		ZORBA_ERROR_ALERT(ZorbaError::API0018_CANNOT_ACCESS_FILE, NULL, DONT_CONTINUE_EXECUTION, xquery_file);
		return NULL;
	}
	fseek(fquery, 0, SEEK_SET);
	xquerydata = (char*)malloc(fsize+1);
  memset(xquerydata, 0, fsize+1);
	if(fread(xquerydata, 1, fsize, fquery) < fsize)
	{
		::free(xquerydata);
		fclose(fquery);
		ZORBA_ERROR_ALERT(ZorbaError::API0018_CANNOT_ACCESS_FILE, NULL, DONT_CONTINUE_EXECUTION, xquery_file);
		return NULL;
	}
	fclose(fquery);

	result_query = createQuery(xquerydata, sctx, xquery_file, routing_mode);
	::free(xquerydata);
	
	return result_query;

	}CATCH_ALL_RETURN_NULL;
 */
  return createQuery("", sctx, xquery_file, routing_mode, planprint);
}

XQuery_t ZorbaEngineImpl::createQueryFromStream(
			std::istream		&is,
      StaticQueryContext_t sctx,
      xqp_string xquery_source_uri,
      bool routing_mode,
      XQueryTreePlans_t planprint)
{
	xqp_string		xquery_string;
	char					*temp_str;

	temp_str = new char[1000];
	try{
		while(1)
		{
			memset(temp_str, 0, 1000);
			is.read(temp_str, 999);
			xquery_string += temp_str;
			if(is.gcount() <= 0)
				break;
		}
	}catch(...)
	{
	}
	delete[] temp_str;
	return createQuery(xquery_string, sctx, xquery_source_uri, routing_mode, planprint);
}

ZorbaAlertsManager_t ZorbaEngineImpl::getAlertsManagerForCurrentThread()
{
	Zorba* z = getZorbaForCurrentThread();

	try{
		if(!z)
		{
			ZORBA_ERROR_ALERT(ZorbaError::API0009_THREAD_NOT_INITIALIZED);
		}

		return &*z->getErrorManager();
	}CATCH_ALL_RETURN_NULL;
}


void ZorbaEngineImpl::setDefaultCollation(
    std::string  coll_string,
    ::Collator::ECollationStrength coll_strength)
{
	try{

	getZorbaForCurrentThread()->setDefaultCollation(coll_string, coll_strength);
	}CATCH_ALL_NO_RETURN(;);
}


void ZorbaEngineImpl::setDefaultCollation(::Collator *default_coll)
{
	try{
	getZorbaForCurrentThread()->setDefaultCollation(default_coll);
	}CATCH_ALL_NO_RETURN(;);
}


void ZorbaEngineImpl::getDefaultCollation(
    std::string  *coll_string,
    ::Collator::ECollationStrength *coll_strength,
    ::Collator **default_coll)
{
	try{
	getZorbaForCurrentThread()->
    getDefaultCollation(coll_string, coll_strength, default_coll);
	}CATCH_ALL_NO_RETURN(;);
}


void	ZorbaEngineImpl::setItemSerializerParameter(
    xqp_string parameter_name,
    xqp_string value)
{
	try{
	getZorbaForCurrentThread()->
    getItemSerializer()->set_parameter(parameter_name, value);
	}CATCH_ALL_NO_RETURN(;);
}


void	ZorbaEngineImpl::setDocSerializerParameter(
    xqp_string parameter_name,
    xqp_string value)
{
	try{
	getZorbaForCurrentThread()->
    getDocSerializer()->set_parameter(parameter_name, value);
	}CATCH_ALL_NO_RETURN(;);
}


StaticQueryContext_t ZorbaEngineImpl::createStaticContext()
{
	return new StaticContextWrapper;
}


DynamicQueryContext_t ZorbaEngineImpl::createDynamicContext()
{
	return new DynamicContextWrapper;
}

XmlDataManager_t		ZorbaEngineImpl::getXmlDataManager()
{
	return xml_data_manager;
}

XQueryTreePlans_t ZorbaEngineImpl::createDebugPlanPrintObject()
{
  return new XQueryPlanPrinterConfig;
}

bool		ZorbaSingleThread::execute(xqp_string xquery_text, std::ostream &os)
{
	ZorbaSingleThread_t		zorba_engine = ZorbaSingleThread::getInstance();
	XQuery_t				xquery;

	//create and compile a query
	xquery = zorba_engine->createQuery(xquery_text);
	if(xquery == NULL)
		return false;
	//init execution
	if(!xquery->initExecution())
		return false;
	//do actual execution
	if(!xquery->serializeXML(os))
		return false;

	return true;
}

}//end namespace zorba
