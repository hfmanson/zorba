using ConsoleApp1;
using System.Runtime.InteropServices;
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

NativeEngine.SetDomFacade(facade);
//NativeEngine.RunXQuery("1+2");
//NativeEngine.RunXQuery("replace value of node attribute henri { \"manson\" } with \"anders\"");
//NativeEngine.RunXQuery("doc(\"henri.xml\")/root/@henri/data()");
//NativeEngine.RunXQuery("replace value of node doc(\"henri.xml\")/root/@henri with \"anders\"");
//IntPtr ptr = NativeEngine.RunXQuery(".");
IntPtr ptr = NativeEngine.RunXQuery("replace value of node root/@henri with \"anders\"");
//NativeEngine.RunXQuery("insert node attribute larie { \"koek\" } into root");
GCHandle gch = GCHandle.FromIntPtr(ptr);
XDocument? doc = (XDocument?)gch.Target;
Console.WriteLine(doc);
