/**
 * @file plan_visitor_test.cpp
 * @author Paul Pedersen
 * @copyright 2006-2007 FLWOR Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "plan_visitor.h"
#include "exprtree/normalize_visitor.h"
#include "parser/xquery_driver.h"
#include "errors/Error.h"
#include "zorba.h"
#include "zorba/zorba_value_factory.h"

#include <iostream>

using namespace std;
using namespace xqp;

#ifndef _WIN32_WCE
int main(int argc, char* argv[])
#else
int _tmain(int argc, _TCHAR* argv[])
#endif
{
	///application specific

	zorba::initializeZorbaEngine();
	
	zorba_value_factory		zorbaValueFactory;
	static_context::init(&zorbaValueFactory);
	dynamic_context::init(&zorbaValueFactory);

	///thread specific

	zorba* zorp = zorba::allocateZorbaForNewThread();//new zorba();
	static_context* sctx_p = new static_context(&zorbaValueFactory);//NULL);
	dynamic_context* dctx_p = new dynamic_context(&zorbaValueFactory);//NULL);
	//add the error manager
	errors_english	*err_messages = new errors_english;///the english error messages
	ZorbaErrorAlerts	*err_manag = new ZorbaErrorAlerts(err_messages);

	library lib;
	dctx_p->set_library(&lib);

	zorp->set_static_context(sctx_p);
	zorp->set_dynamic_context(dctx_p);
	zorp->set_error_manager(err_manag);//add the error manager

	xquery_driver driver(cout);
	try {

		for (++argv; argv[0]; ++argv) {
#ifndef UNICODE
			if (*argv == std::string ("-p")) {
				driver.trace_parsing = true;
			}
			else if (*argv == std::string ("-s")) {
				driver.trace_scanning = true;
			}
#else
			if (!_tcscmp(*argv, _T("-p"))) {
				driver.trace_parsing = true;
			}
			else if (!_tcscmp(*argv, _T("-s"))) {
				driver.trace_scanning = true;
			}
#endif
			else {
#ifndef UNICODE
				driver.parse(*argv);
#else
				char	testfile[1024];
				WideCharToMultiByte(CP_ACP, 0,//or CP_UTF8
														*argv, -1, 
														testfile, sizeof(testfile)/sizeof(char),
														NULL, NULL);
				driver.parse(testfile);
#endif
				parsenode* n_p = driver.get_expr();
				cout << endl;
				cout << "Syntax tree:\n";
				n_p->put(cout) << endl;
	
				normalize_visitor nvs;
				MainModule* mm_p;
				QueryBody* qb_p;
				Expr* ex_p;
	
				if ((mm_p = dynamic_cast<MainModule*>(n_p))==NULL) {
					cout << "Parse error: expecting MainModule\n";
					return -1;
				}
				if ((qb_p = dynamic_cast<QueryBody*>(&*mm_p->get_query_body()))==NULL) {
					cout << "Parse error: expecting MainModule->QueryBody\n";
					return -1;
				}
				if ((ex_p = dynamic_cast<Expr*>(&*qb_p->get_expr()))==NULL) {
					cout << "Parse error: expecting MainModule->QueryBody->Expr\n";
					return -1;
				}

				cout << "Expression tree:\n";
				ex_p->accept(nvs);
				rchandle<expr> e_h = nvs.pop_nodestack();

				cout << endl;

				if (e_h==NULL) {
					cout << "e_h==NULL\n";
					return -1;
				}
				e_h->put(cout) << endl;

				cout << "Codegen:\n";
				plan_visitor pvs;
				e_h->accept(pvs);
				iterator_t it_h = pvs.pop_itstack();
				cout << endl;

				//cout << "iterator type = " << typeid(*it_h).name() << endl;

				cout << "Iterator tree:" << std::endl;
 				it_h->show(cout);

				cout << "\nIterator run:\n";
				if (it_h==NULL) {
					cout << "it_h==NULL\n";
					return -1;
				}

// 				it_h->open();
// 				while (true) {
// 					item_t i_p = it_h->next();
// 					if (i_p==NULL){ 
// 						break;
// 					}
// 					i_p->put(zorp,cout) << endl;
// 				}
// 				it_h->close();
					
				while (true) {
					item_t i_p = it_h->next();
					if (i_p == NULL)
						break;
					i_p->put(cout) << endl;
				}
				it_h->releaseResources();
			}
		}
	} catch (xqp_exception & e) {
		cout << "application exception: " << e.get_msg() << " - " << e.what() << endl;
	} catch (...) {
		cout << "catch all exception\n";
	}

	zorba::destroyZorbaForCurrentThread();
	zorba::uninitializeZorbaEngine();
}

