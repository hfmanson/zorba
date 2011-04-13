#include <cassert>
#include <fstream>
#include <iostream>

#include <zorba/store_manager.h>
#include <zorba/zorba.h>
#include <zorba/xquery.h>
#include <zorba/xquery_exception.h>
#include <zorba/serializer.h>
#include <zorba/item_sequence.h>
#include <zorba/singleton_item_sequence.h>

using namespace zorba;

bool
invoke1(zorba::Zorba* z)
{
  try
  {
    StaticContext_t lSctx = z->createStaticContext();
    ItemFactory* lFac = z->getItemFactory();
    Item lQName = lFac->createQName(
        "http://www.w3.org/2005/xpath-functions",
        "current-dateTime");
    std::vector<ItemSequence_t> lArgs;

    ItemSequence_t lRes = lSctx->invoke(lQName, lArgs);

    Zorba_SerializerOptions_t lOpt;
    Serializer_t lSer = Serializer::createSerializer(lOpt);;
    lSer->serialize(lRes, std::cout);
    std::cout << std::endl;
  }
  catch (zorba::XQueryException &e)
  {
    std::cerr << "Error: " << e << std::endl;
    return false;
  }
  catch (zorba::ZorbaException &e)
  {
    std::cerr << "Error: " << e << std::endl;
    return false;
  }
  return true;
}

bool
invoke2(zorba::Zorba* z)
{
  try
  {
    // test QName not found
    StaticContext_t lSctx = z->createStaticContext();
    ItemFactory* lFac = z->getItemFactory();
    Item lQName = lFac->createQName(
        "http://www.w3.org/2005/xpath-functions",
        "foo");
    std::vector<ItemSequence_t> lArgs;

    ItemSequence_t lRes = lSctx->invoke(lQName, lArgs);

  }
  catch (zorba::XQueryException &e)
  {
    std::cerr << "Error: " << e << std::endl;
    return true;
  }
  catch (zorba::ZorbaException &e)
  {
    std::cerr << "Error: " << e << std::endl;
    return false;
  }
  return false;
}

bool
invoke3(zorba::Zorba* z)
{
  try
  {
    // test function with wrong given number of params
    StaticContext_t lSctx = z->createStaticContext();
    ItemFactory* lFac = z->getItemFactory();
    Item lQName = lFac->createQName(
        "http://www.w3.org/2005/xpath-functions",
        "round"); // round needs at least one param
    std::vector<ItemSequence_t> lArgs;

    ItemSequence_t lRes = lSctx->invoke(lQName, lArgs);

  }
  catch (zorba::XQueryException &e)
  {
    std::cerr << "Error: " << e << std::endl;
    return true;
  }
  catch (zorba::ZorbaException &e)
  {
    std::cerr << "Error: " << e << std::endl;
    return false;
  }
  return false;
}

bool
invoke4(zorba::Zorba* z)
{
  try
  {
    // test function with one param
    StaticContext_t lSctx = z->createStaticContext();
    ItemFactory* lFac = z->getItemFactory();
    Item lQName = lFac->createQName(
        "http://www.w3.org/2005/xpath-functions",
        "round"); // round needs at least one param

    std::vector<ItemSequence_t> lArgs;
    ItemSequence_t lSeq1 = new SingletonItemSequence(
        lFac->createDecimal("12678967.543233"));
    lArgs.push_back(lSeq1);

    ItemSequence_t lRes = lSctx->invoke(lQName, lArgs);
    Zorba_SerializerOptions_t lOpt;
    Serializer_t lSer = Serializer::createSerializer(lOpt);;
    lSer->serialize(lRes, std::cout);
    std::cout << std::endl;

  }
  catch (zorba::XQueryException &e)
  {
    std::cerr << "Error: " << e << std::endl;
    return false;
  }
  catch (zorba::ZorbaException &e)
  {
    std::cerr << "Error: " << e << std::endl;
    return false;
  }
  return true;
}

bool
invoke5(zorba::Zorba* z)
{
  try
  {
    // test function with one param
    StaticContext_t lSctx = z->createStaticContext();
    Zorba_CompilerHints_t lHints;

    std::ostringstream lProlog;
    lProlog
      << "import module namespace r = 'http://www.zorba-xquery.com/modules/random';";
    lSctx->loadProlog(lProlog.str(), lHints);

    ItemFactory* lFac = z->getItemFactory();
    Item lQName = lFac->createQName(
        "http://www.zorba-xquery.com/modules/random",
        "r",
        "random"); // round needs at least one param

    std::vector<ItemSequence_t> lArgs;

    ItemSequence_t lRes = lSctx->invoke(lQName, lArgs);

    Zorba_SerializerOptions_t lOpt;
    Serializer_t lSer = Serializer::createSerializer(lOpt);;
    lSer->serialize(lRes, std::cout);
    std::cout << std::endl;

  }
  catch (zorba::XQueryException &e)
  {
    std::cerr << "Error: " << e << std::endl;
    return false;
  }
  catch (zorba::ZorbaException &e)
  {
    std::cerr << "Error: " << e << std::endl;
    return false;
  }
  return true;
}

int
invoke(int argc, char* argv[])
{
  void* store = zorba::StoreManager::getStore();
  zorba::Zorba* z = zorba::Zorba::getInstance(store);

  std::cout << "executing example 1" << std::endl;
  if (!invoke1(z))
    return 1;
  std::cout << std::endl;

  std::cout << "executing example 2" << std::endl;
  if (!invoke2(z))
    return 2;
  std::cout << std::endl;

  std::cout << "executing example 3" << std::endl;
  if (!invoke3(z))
    return 3;
  std::cout << std::endl;

  std::cout << "executing example 4" << std::endl;
  if (!invoke4(z))
    return 4;
  std::cout << std::endl;

  std::cout << "executing example 5" << std::endl;
  if (!invoke5(z))
    return 5;
  std::cout << std::endl;
  return 0;
}
