using Zorba;
using System.Xml.Linq;

Test.Test5();
Test.boxupTest();

public class Test
{
    public static void InitEngine(string? xmlFile)
    {
        //Console.WriteLine("Current directory: " + Environment.CurrentDirectory);
        //Console.WriteLine("PATH: " + Environment.GetEnvironmentVariable("PATH"));
        DomFacadeCallbacks facade = new DomFacadeCallbacks
        {
            CreateDocumentNode = DomImpl.CreateDocumentNode,
            CreateElementNode = DomImpl.CreateElementNode,
            CreateAttributeNode = DomImpl.CreateAttributeNode,
            SetStringValue = DomImpl.SetStringValue,
            Add = DomImpl.Add
        };

        NativeEngine.InitEngine(facade, xmlFile);
    }

    public static void ShutdownEngine()
    {
        NativeEngine.ShutdownEngine();
    }

    public static void boxupTest()
    {
        InitEngine("boxup1.xml");
        XDocument? doc = NativeEngine.getXDocument();
        if (doc != null)
        {
            doc.Changed += Doc_Changed;
            Console.WriteLine(doc);
            NativeEngine.RunXQuery("import module namespace boxup=\"http://mansoft.nl/boxup\" at 'boxup.xqm'; boxup:check-move(., 1, 0)");
            //Console.WriteLine(doc);
            NativeEngine.RunXQuery("import module namespace boxup=\"http://mansoft.nl/boxup\" at 'boxup.xqm'; boxup:check-move(., 0, 1)");
            //Console.WriteLine(doc);
            doc.Changed -= Doc_Changed;
        }
        ShutdownEngine();
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
        InitEngine("henri.xml");
        NativeEngine.RunXQuery(".");
        ShutdownEngine();
    }
    public static void Test2()
    {
        InitEngine("henri.xml");
        NativeEngine.RunXQuery("replace value of node root/@henri with \"anders\"");
        ShutdownEngine();
    }
    public static void Test3()
    {
        InitEngine("henri.xml");
        ShutdownEngine();
    }
    public static void Test4()
    {
        InitEngine("henri.xml");
        NativeEngine.RunXQuery("insert node attribute larie { \"koek\" } into root");
        ShutdownEngine();
    }
    public static void Test5()
    {
        InitEngine(null);
        NativeEngine.RunXQuery("import module namespace my = 'http://example.com/my' at 'my.xqm'; my:hello('henri')");
        ShutdownEngine();
    }
};
