using Zorba;
using System.Xml.Linq;

//Test.Test1();
Test.boxupTest();

public class Test
{
    public static void _boxup(string xmlfile)
    {
        IntPtr docItemHandle = NativeEngine.LoadXML(xmlfile);
        XDocument? doc = NativeEngine.GetXDocument(docItemHandle);
        if (doc != null)
        {
            doc.Changed += Doc_Changed;
            Console.WriteLine(doc);
            NativeEngine.RunXQuery("import module namespace boxup=\"http://mansoft.nl/boxup\" at 'boxup.xqm'; boxup:check-move(., 1, 0)", docItemHandle);
            //Console.WriteLine(doc);
            NativeEngine.RunXQuery("import module namespace boxup=\"http://mansoft.nl/boxup\" at 'boxup.xqm'; boxup:check-move(., 0, 1)", docItemHandle);
            //Console.WriteLine(doc);
            doc.Changed -= Doc_Changed;
        }
        NativeEngine.FreeXML(docItemHandle);
    }
    public static void boxupTest()
    {
        NativeEngine.InitEngine();
        _boxup("boxup1.xml");
        _boxup("boxup2.xml");
        _boxup("boxup3.xml");
        _boxup("boxup4.xml");
        _boxup("boxup5.xml");
        NativeEngine.ShutdownEngine();
    }

    private static void Doc_Changed(object? sender, XObjectChangeEventArgs e)
    {
        if (sender is XAttribute attr)
        {
            Console.WriteLine(attr.Parent);
        }
    }

    public static void Test1()
    {
        NativeEngine.InitEngine();
        IntPtr docItemHandle = NativeEngine.LoadXML("henri.xml");
        NativeEngine.RunXQuery(".", docItemHandle);
        NativeEngine.FreeXML(docItemHandle);
        NativeEngine.ShutdownEngine();
    }
    public static void Test2()
    {
        NativeEngine.InitEngine();
        IntPtr docItemHandle = NativeEngine.LoadXML("henri.xml");
        NativeEngine.RunXQuery("replace value of node root/@henri with \"anders\"", docItemHandle);
        NativeEngine.FreeXML(docItemHandle);
        NativeEngine.ShutdownEngine();
    }
    public static void Test4()
    {
        NativeEngine.InitEngine();
        IntPtr docItemHandle = NativeEngine.LoadXML("henri.xml");
        NativeEngine.RunXQuery("insert node attribute larie { \"koek\" } into root", docItemHandle);
        NativeEngine.FreeXML(docItemHandle);
        NativeEngine.ShutdownEngine();
    }
    public static void Test5()
    {
        NativeEngine.InitEngine();
        NativeEngine.RunXQuery("import module namespace my = 'http://example.com/my' at 'my.xqm'; my:hello('henri')", IntPtr.Zero);
        NativeEngine.ShutdownEngine();
    }
};
