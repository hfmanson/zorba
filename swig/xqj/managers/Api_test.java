/*
 * Copyright 2006-2012 The FLWOR Foundation.
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
package api_test;


import java.io.*;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.Properties;
import javax.xml.namespace.QName;
import javax.xml.xquery.*;
import org.zorbaxquery.api.*;
import org.zorbaxquery.api.xqj.*;

public class Api_test {

  static 
  {
    System.loadLibrary ( "zorba_api" );
  }

  static boolean example_1() throws XQException
  {
      ZorbaXQDataSource xqds = new ZorbaXQDataSource();
      XQConnection xqc = xqds.getConnection();
      XQExpression xqe = xqc.createExpression();
      org.zorbaxquery.api.xqj.ZorbaXQResultSequence xqs = (org.zorbaxquery.api.xqj.ZorbaXQResultSequence) xqe.executeQuery("1,2,3");
      ZorbaXQStaticCollectionManager colManager =  xqs.getStaticCollectionManager();
      xqc.close();
      xqc.close();
      return true;
  }


  static boolean example_2 () throws XQException
  {
      StringBuilder strBuilder = new StringBuilder();
      try {
            BufferedReader in = new BufferedReader(new FileReader("managers/module1.xq"));
            String str;
            while ((str = in.readLine()) != null) {
                strBuilder.append(str);
            }
            in.close();
      } catch (Exception e) {
          throw new XQException("Error reading file for test: " + e.getLocalizedMessage());
      }
      ZorbaXQDataSource xqds = new ZorbaXQDataSource();
      XQConnection xqc = xqds.getConnection();
      XQExpression xqe = xqc.createExpression();
      org.zorbaxquery.api.xqj.ZorbaXQResultSequence xqs = (org.zorbaxquery.api.xqj.ZorbaXQResultSequence) xqe.executeQuery(strBuilder.toString());
      ZorbaXQStaticCollectionManager colManager =  xqs.getStaticCollectionManager();
      boolean resultAdding = false;
      boolean resultDeleting = true;
      URI uri;
      QName qname;
      XQItemType type = null;
        try {
          uri = new URI("http://www.mod2.com/");
          qname = new QName("http://www.mod2.com/", "coll");
          type = xqc.createAtomicType( XQItemType.XQBASETYPE_QNAME, qname, uri);
        } catch (URISyntaxException e) {
            throw new XQException("Error creating QName: " + e.getLocalizedMessage());
        }
      XQItem colName = xqc.createItemFromString("coll",  type);
      colManager.createCollection(colName);
      resultAdding = colManager.isAvailableCollection(colName);
      colManager.deleteCollection(colName);
      resultDeleting = !colManager.isAvailableCollection(colName);
      xqc.close();
      return resultAdding && resultDeleting;
  }
  
  static boolean example_3a() throws XQException
  {
      StringBuilder strBuilder = new StringBuilder();
      try {
            BufferedReader in = new BufferedReader(new FileReader("managers/module1.xq"));
            String str;
            while ((str = in.readLine()) != null) {
                strBuilder.append(str);
            }
            in.close();
      } catch (Exception e) {
          throw new XQException("Error reading file for test: " + e.getLocalizedMessage());
      }
      ZorbaXQDataSource xqds = new ZorbaXQDataSource();
      org.zorbaxquery.api.xqj.ZorbaXQConnection xqc = (org.zorbaxquery.api.xqj.ZorbaXQConnection) xqds.getConnection();
      XQExpression xqe = xqc.createExpression();
      org.zorbaxquery.api.xqj.ZorbaXQResultSequence xqs = (org.zorbaxquery.api.xqj.ZorbaXQResultSequence) xqe.executeQuery(strBuilder.toString());
      ZorbaXQStaticCollectionManager colManager =  xqs.getStaticCollectionManager();
      URI uri;
      QName qname;
      XQItemType type = null;
        try {
          uri = new URI("http://www.mod2.com/");
          qname = new QName("http://www.mod2.com/", "coll");
          type = xqc.createAtomicType( XQItemType.XQBASETYPE_QNAME, qname, uri);
        } catch (URISyntaxException e) {
            throw new XQException("Error creating QName: " + e.getLocalizedMessage());
        }
      XQItem colName = xqc.createItemFromString("coll",  type);
      colManager.createCollection(colName);
      ZorbaXQCollection collection = colManager.getCollection(colName);
      
      ZorbaXQXmlDataManager manager = xqc.getXmlDataManager();
      XQSequence data = manager.parseXML("<books><book>Book 1</book><book>Book 2</book></books>");
      collection.insertNodesFirst(data);
      
      colManager.deleteCollection(colName);
      boolean resultDeleting = !colManager.isAvailableCollection(colName);
      xqc.close();
      return resultDeleting;
  }

  static boolean example_3b() throws XQException
  {
      StringBuilder strBuilder = new StringBuilder();
      try {
            BufferedReader in = new BufferedReader(new FileReader("managers/module1.xq"));
            String str;
            while ((str = in.readLine()) != null) {
                strBuilder.append(str);
            }
            in.close();
      } catch (Exception e) {
          throw new XQException("Error reading file for test: " + e.getLocalizedMessage());
      }
    InMemoryStore store = InMemoryStore.getInstance();
    Zorba zorba = Zorba.getInstance ( store );
    XQuery query = zorba.compileQuery(strBuilder.toString());
    StaticCollectionManager manager = query.getStaticCollectionManager();
    
    
    ItemFactory factory = zorba.getItemFactory();
    Item name = factory.createQName("http://www.mod2.com/", "coll");
    
    manager.createCollection(name);
    Collection collection = manager.getCollection(name);

    XmlDataManager xmlManager = zorba.getXmlDataManager();
    Item data = xmlManager.parseXMLtoItem("<books><book>Book 1</book><book>Book 2</book></books>");
    ItemSequence sequence = new ItemSequence(data);
    collection.insertNodesLast(sequence);
    
    zorba.shutdown();
    InMemoryStore.shutdown ( store );

    return true;
  }

  static boolean example_4() throws XQException
  {
      ZorbaXQDataSource xqds = new ZorbaXQDataSource();
      org.zorbaxquery.api.xqj.ZorbaXQConnection xqc = (org.zorbaxquery.api.xqj.ZorbaXQConnection) xqds.getConnection();
      XQExpression xqe = xqc.createExpression();
      ZorbaXQXmlDataManager xmlManager = xqc.getXmlDataManager();
      ZorbaXQCollectionManager colManager =  xmlManager.getCollectionManager();
      xqc.close();
      xqc.close();
      return true;
  }

  static boolean example_5() throws XQException
  {
      ZorbaXQDataSource xqds = new ZorbaXQDataSource();
      org.zorbaxquery.api.xqj.ZorbaXQConnection xqc = (org.zorbaxquery.api.xqj.ZorbaXQConnection) xqds.getConnection();
      ZorbaXQXmlDataManager xmlManager = xqc.getXmlDataManager();
      ZorbaXQCollectionManager colManager =  xmlManager.getCollectionManager();
      boolean resultAdding = false;
      boolean resultDeleting = true;
      URI uri;
      QName qname;
      XQItemType type = null;
        try {
          uri = new URI("http://www.mod2.com/");
          qname = new QName("http://www.mod2.com/", "col2");
          type = xqc.createAtomicType( XQItemType.XQBASETYPE_QNAME, qname, uri);
        } catch (URISyntaxException e) {
            throw new XQException("Error creating QName: " + e.getLocalizedMessage());
        }
      XQItem colName = xqc.createItemFromString("col2",  type);
      colManager.createCollection(colName);
      resultAdding = colManager.isAvailableCollection(colName);
      colManager.deleteCollection(colName);
      resultDeleting = !colManager.isAvailableCollection(colName);
      xqc.close();
      return resultAdding && resultDeleting;
  }
  static boolean example_6a() throws XQException
  {
    InMemoryStore store = InMemoryStore.getInstance();
    Zorba zorba = Zorba.getInstance ( store );
    XmlDataManager xmlManager = new XmlDataManager(zorba.getXmlDataManager());
    CollectionManager manager = new CollectionManager(xmlManager.getCollectionManager());
    ItemFactory factory = zorba.getItemFactory();
    Item name = factory.createQName("http://www.zorba-xquery.com/", "aaa");
    manager.createCollection(name);
    boolean resultAdding = manager.isAvailableCollection(name);
    Collection collection = null;
    //Item data = new Item();
    if (resultAdding) {
      collection = manager.getCollection(name);
      Item data = xmlManager.parseXMLtoItem("<books><book>Book 1</book><book>Book 2</book></books>");
      collection.insertNodesLast(new ItemSequence(data));
    }
    collection.delete();
    
    zorba.shutdown();
    InMemoryStore.shutdown ( store );
    return true;
  }

  static boolean example_6b() throws XQException
  {
      ZorbaXQDataSource xqds = new ZorbaXQDataSource();
      org.zorbaxquery.api.xqj.ZorbaXQConnection xqc = (org.zorbaxquery.api.xqj.ZorbaXQConnection) xqds.getConnection();
      ZorbaXQXmlDataManager xmlManager = xqc.getXmlDataManager();
      ZorbaXQCollectionManager colManager =  xmlManager.getCollectionManager();
      URI uri;
      QName qname;
      XQItemType type = null;
        try {
          uri = new URI("http://www.mod2.com/");
          qname = new QName("http://www.mod2.com/", "col2");
          type = xqc.createAtomicType( XQItemType.XQBASETYPE_QNAME, qname, uri);
        } catch (URISyntaxException e) {
            throw new XQException("Error creating QName: " + e.getLocalizedMessage());
        }
      XQItem colName = xqc.createItemFromString("col2",  type);
      colManager.createCollection(colName);
      ZorbaXQCollection collection = colManager.getCollection(colName);
      colName.close();
      XQSequence data = xmlManager.parseXML("<books><book>Book 1</book><book>Book 2</book></books>");
      collection.insertNodesLast(data);
      xqc.close();
      return true;
  }

  static boolean example_7() throws XQException
  {
    InMemoryStore store = InMemoryStore.getInstance();
    Zorba zorba = Zorba.getInstance ( store );
    ReaderZorbaStream stream = new ReaderZorbaStream(new StringReader("'Hello world!'"));
    XQuery query = zorba.compileQuery(stream);
    System.out.println(query.execute());
    zorba.shutdown();
    InMemoryStore.shutdown ( store );
    return true;
  }

  static boolean example_8() throws XQException, UnsupportedEncodingException
  {
    InMemoryStore store = InMemoryStore.getInstance();
    Zorba zorba = Zorba.getInstance ( store );
    String test = 
    "'"+
    // 500 chars
    "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"+ // 100 chars
    "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"+ // 100 chars
    "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"+ // 100 chars
    "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"+ // 100 chars
    "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"+ // 100 chars
    // 500 chars
    "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"+ // 100 chars
    "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"+ // 100 chars
    "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"+ // 100 chars
    "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"+ // 100 chars
    "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"+ // 100 chars
            
    "1234567890123456789012"+ // 100 chars
    "'";
    InputZorbaStream stream = new InputZorbaStream(new ByteArrayInputStream(test.getBytes("UTF-8")));
    XQuery query = zorba.compileQuery(stream);
    System.out.println(query.execute());
    zorba.shutdown();
    InMemoryStore.shutdown ( store );
    return true;
  }

  static boolean example_9() throws XQException, UnsupportedEncodingException
  {
    InMemoryStore store = InMemoryStore.getInstance();
    Zorba zorba = Zorba.getInstance ( store );
    String test = "<Hello><ab/><ax/>World</Hello>";
    InputZorbaStream stream = new InputZorbaStream(new ByteArrayInputStream(test.getBytes("UTF-8")));
    XQuery query = zorba.compileQuery(stream);
    System.out.println(query.execute());
    query.destroy();
    zorba.shutdown();
    InMemoryStore.shutdown ( store );
    
    return true;
  }

  static boolean example_10() throws XQException, UnsupportedEncodingException
  {
    InMemoryStore store = InMemoryStore.getInstance();
    Zorba zorba = Zorba.getInstance ( store );
    String test = "<Hello><ab/><ax/>World</Hello>";
    InputZorbaStream stream = new InputZorbaStream(new ByteArrayInputStream(test.getBytes("UTF-8")));
    XQuery query = zorba.compileQuery(stream);
    
    OutputZorbaStream OStream = new OutputZorbaStream(System.out);
    query.execute(OStream);

    query.destroy();
    zorba.shutdown();
    InMemoryStore.shutdown ( store );
    
    return true;
  }

  public static void main ( String argv[] ) throws XQException, UnsupportedEncodingException
  {
    boolean res = false;

    System.out.println ("Executing test 1" );
    res = example_1();
    if ( !res ) 
      System.exit (1);

    System.out.println ( "Executing test 2" );
    res = example_2();
    if (!res) 
      System.exit (1);
  
    System.out.println ( "Executing test 3a" );
    res = example_3a();
    if (!res) 
      System.exit (1);
  
    System.out.println ( "Executing test 3b" );
    res = example_3b();
    if (!res) 
      System.exit (1);
    
    System.out.println ( "Executing test 4" );
    res = example_4();
    if (!res) 
      System.exit (1);

    System.out.println ( "Executing test 5" );
    res = example_5();
    if (!res) 
      System.exit (1);
    System.out.println ( "Executing test 6a" );
    res = example_6a();
    if (!res) 
      System.exit (1);

    System.out.println ( "Executing test 6b" );
    res = example_6b();
    if (!res) 
      System.exit (1);

    System.out.println ( "Executing test 7" );
    res = example_7();
    if (!res) 
      System.exit (1);

    System.out.println ( "Executing test 8" );
    res = example_8();
    if (!res) 
      System.exit (1);

    System.out.println ( "Executing test 9" );
    res = example_9();
    if (!res) 
      System.exit (1);

    System.out.println ( "Executing test 10" );
    res = example_10();
    if (!res) 
      System.exit (1);

    System.out.println ( "Done." );

  } // main

} // class Test_Zorba

