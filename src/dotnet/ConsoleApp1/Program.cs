using ConsoleApp1;

Console.WriteLine("Hello, World!");
Console.WriteLine(Environment.CurrentDirectory);
Console.WriteLine(Environment.GetEnvironmentVariable("PATH"));
DomFacadeCallbacks facade = new DomFacadeCallbacks
{
    CreateAttributeNode = DomImpl.CreateAttributeNode,
    SetStringValue = DomImpl.SetStringValue
};

NativeEngine.SetDomFacade(facade);
//NativeEngine.RunXQuery("1+2");
NativeEngine.RunXQuery("replace value of node attribute henri { \"manson\" } with \"anders\"");
