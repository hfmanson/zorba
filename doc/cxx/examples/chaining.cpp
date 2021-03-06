/*
 * Copyright 2006-2016 zorba.io
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

#include <iostream>
#include <fstream>
#include <string>

#include <zorba/zorba.h>
#include <zorba/iterator.h>
#include <zorba/diagnostic_handler.h>
#include <zorba/store_manager.h>
#include <zorba/zorba_exception.h>

using namespace zorba;

/**
 * Example to show query chaining at work.
 * In this example, we will execute query 1 and chain the results
 * of that query as in input to another query (query 2) by means
 * of an external variable.
 */
bool
chaining_example_1(Zorba* aZorba)
{
  try {
    /* Query 1 */
    XQuery_t lQuery1 = aZorba->compileQuery("for $i in (1 to 20) return $i");

    Iterator_t lIterator = lQuery1->iterator();

    /* Query 2 */
    XQuery_t lQuery2 = aZorba->compileQuery("declare variable $x external; for $i in $x return $i * $i");

    DynamicContext* lCtx = lQuery2->getDynamicContext();

    /* We bind the iterator representing the output of query 1 as the value of
     * external variable (x) in query 2
     */
    lCtx->setVariable("x", lIterator);

    std::cout << lQuery2 << std::endl;

  } catch (ZorbaException &e) {
    std::cerr << e << std::endl;
    return false;
  }

  return true;
}

class MyChainingDiagnosticHandler  : public DiagnosticHandler 
{
public:
  MyChainingDiagnosticHandler(const std::string& aIdentifier) 
    : theIdentifier(aIdentifier) {}

  void error(const ZorbaException& e) 
  { 
    std::cerr << "error handler: " << theIdentifier << " error " << e << std::endl;
  }

  void warning(const XQueryWarning& w)
  { 
    std::cerr << "warning handler: " << theIdentifier << " warning " << w << std::endl;
  }

protected:
  std::string theIdentifier;
};

/**
 * Example to show the use of error handlers to obtain errors from queries
 * instead of the executor throwing exceptions.
 * Other than that change in behavior, this example is functionally
 * equivalent to the above example.
 */
bool
chaining_example_2(Zorba* aZorba)
{
  MyChainingDiagnosticHandler lDiagnosticHandler1("handler 1");
  MyChainingDiagnosticHandler lDiagnosticHandler2("handler 2");

  XQuery_t lQuery1 = aZorba->compileQuery("let $i := (1 to 42) return $i * $i", &lDiagnosticHandler1);

  Iterator_t lIterator = lQuery1->iterator();

  XQuery_t lQuery2 = aZorba->compileQuery("declare variable $x external; for $i in $x return $i * $i", &lDiagnosticHandler2);

  DynamicContext* lCtx = lQuery2->getDynamicContext();

  lCtx->setVariable("x", lIterator);

  std::cout << lQuery2 << std::endl;

  return true;
}

int 
chaining(int argc, char* argv[])
{
  void* lStore = zorba::StoreManager::getStore();

  Zorba* lZorba = Zorba::getInstance(lStore);
  bool res = false;

  std::cout << "executing example 1" << std::endl;
  res = chaining_example_1(lZorba);
  if (!res) return 1; 
  std::cout << std::endl;

  std::cout << "executing example 2" << std::endl;
  res = chaining_example_2(lZorba);
  if (!res) return 1; 
  std::cout << std::endl;

  lZorba->shutdown();
  zorba::StoreManager::shutdownStore(lStore);
  return 0;
}
