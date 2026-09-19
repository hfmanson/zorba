using ConsoleApp1;
using System.Xml.Linq;

Console.WriteLine("Hello, World!");
Console.WriteLine(Environment.CurrentDirectory);
Console.WriteLine(Environment.GetEnvironmentVariable("PATH"));
DomFacadeCallbacks facade = new DomFacadeCallbacks
{
    CreateDocumentNode = DomImpl.CreateDocumentNode,
    CreateElementNode = DomImpl.CreateElementNode,
    CreateAttributeNode = DomImpl.CreateAttributeNode,
    SetStringValue = DomImpl.SetStringValue,
    Add = DomImpl.Add
};

XDocument hfm = new XDocument();
XElement el = new XElement("root");
hfm.Add(el);
NativeEngine.SetDomFacade(facade);
//NativeEngine.RunXQuery("1+2");
//NativeEngine.RunXQuery("replace value of node attribute henri { \"manson\" } with \"anders\"");
//NativeEngine.RunXQuery("doc(\"henri.xml\")/root/@henri/data()");
//NativeEngine.RunXQuery("replace value of node doc(\"henri.xml\")/root/@henri with \"anders\"");
//NativeEngine.RunXQuery(".");
//NativeEngine.RunXQuery("replace value of node root/@henri with \"anders\"");
NativeEngine.RunXQuery("insert node attribute larie { \"koek\" } into root");
Console.WriteLine(DomImpl.doc);
