using System.Xml.Linq;

NativeEngine.InitEngine("import schema namespace boxup=\"http://mansoft.nl/boxup\" at \"boxup.xsd\";");
//Test.Test1();
//Test.Test2();
//Test.Test3();
//Test.Test4();
Test.boxupTest();
NativeEngine.ShutdownEngine();

public class Test
{
    private static void Doc_Changed(object? sender, XObjectChangeEventArgs e)
    {
        if (sender is XAttribute attr)
        {
            Console.WriteLine(attr.Parent);
        }
    }

    public static void _boxup(string xmlfile)
    {
        IntPtr docItemHandle = NativeEngine.LoadXML(xmlfile);
        XDocument? doc = NativeEngine.GetXDocument(docItemHandle);
        if (doc != null)
        {
            doc.Changed += Doc_Changed;
            Console.WriteLine(doc);
            NativeEngine.XQuery("import module namespace boxmod=\"http://mansoft.nl/boxmod\" at 'boxup.xqm'; boxmod:check-move(., 1, 0)", docItemHandle);
            //Console.WriteLine(doc);
            NativeEngine.XQuery("import module namespace boxmod=\"http://mansoft.nl/boxmod\" at 'boxup.xqm'; boxmod:check-move(., 0, 1)", docItemHandle);
            //Console.WriteLine(doc);
            doc.Changed -= Doc_Changed;
        }
        NativeEngine.FreeXML(docItemHandle);
    }
    public static void boxupTest()
    {
        _boxup("boxup1.xml");
        //_boxup("boxup2.xml");
        //_boxup("boxup3.xml");
        //_boxup("boxup4.xml");
        //_boxup("boxup5.xml");
    }

    public static void Test1()
    {
        IntPtr docItemHandle = NativeEngine.LoadXML("boxup1.xml");
        Console.WriteLine(NativeEngine.XQuery("data(boxup:boxup/@rows) eq 3", docItemHandle));
        NativeEngine.FreeXML(docItemHandle);
    }
    public static void Test2()
    {
        IntPtr docItemHandle = NativeEngine.LoadXML("henri.xml");
        XDocument? doc = NativeEngine.GetXDocument(docItemHandle);
        NativeEngine.XQuery("replace value of node root/@henri with \"anders\"", docItemHandle);
        Console.WriteLine(doc);
        NativeEngine.FreeXML(docItemHandle);
    }
    public static void Test3()
    {
        IntPtr docItemHandle = NativeEngine.LoadXML("henri.xml");
        XDocument? doc = NativeEngine.GetXDocument(docItemHandle);
        NativeEngine.XQuery("insert node attribute larie { \"koek\" } into root", docItemHandle);
        Console.WriteLine(doc);
        NativeEngine.FreeXML(docItemHandle);
    }
    public static void Test4()
    {
        Console.WriteLine(NativeEngine.XQuery("import module namespace my = 'http://example.com/my' at 'my.xqm'; my:hello('henri')", IntPtr.Zero));
    }
};
