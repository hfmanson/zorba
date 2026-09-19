using System.Runtime.InteropServices;
using System.Xml.Linq;

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void SetStringValueDelegate(IntPtr nodeHandle, IntPtr utf8Value);

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr CreateDocumentNodeDelegate();

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr CreateElementNodeDelegate(
    [MarshalAs(UnmanagedType.LPStr)] string localName,
    [MarshalAs(UnmanagedType.LPStr)] string namespaceUri);

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr CreateAttributeNodeDelegate(
    [MarshalAs(UnmanagedType.LPStr)] string localName,
    [MarshalAs(UnmanagedType.LPStr)] string namespaceUri,
    [MarshalAs(UnmanagedType.LPStr)] string value);

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr CreateTextNodeDelegate(
    [MarshalAs(UnmanagedType.LPStr)] string value);

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr CreateCommentNodeDelegate(
    [MarshalAs(UnmanagedType.LPStr)] string value);

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr CreateProcessingInstructionNodeDelegate(
    [MarshalAs(UnmanagedType.LPStr)] string target,
    [MarshalAs(UnmanagedType.LPStr)] string data);

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void AddDelegate(IntPtr container, IntPtr obj);

[StructLayout(LayoutKind.Sequential)]
public struct DomFacadeCallbacks
{
    public SetStringValueDelegate SetStringValue;
    public CreateDocumentNodeDelegate CreateDocumentNode;
    public CreateElementNodeDelegate CreateElementNode;
    public CreateAttributeNodeDelegate CreateAttributeNode;
    public CreateTextNodeDelegate CreateTextNode;
    public CreateCommentNodeDelegate CreateCommentNode;
    public CreateProcessingInstructionNodeDelegate CreateProcessingInstructionNode;
    public AddDelegate Add;
}

public static class NativeEngine
{
    public static XDocument? getXDocument()
    {
        XDocument? document = null;
        IntPtr ptr = GetDocument();
        if (ptr != IntPtr.Zero)
        {
            GCHandle gch = GCHandle.FromIntPtr(ptr);
            document = (XDocument?)gch.Target;
        }
        return document;
    }

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern void InitEngine(DomFacadeCallbacks callbacks, string? xmlFile);

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern void ShutdownEngine();

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr GetDocument();

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern void RunXQuery(string xquery);
}
