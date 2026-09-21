using System.Runtime.InteropServices;
using System.Xml.Linq;
using Zorba;

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

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void FreeHandleDelegate(IntPtr handle);

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
    public FreeHandleDelegate FreeHandle;
}

public static class NativeEngine
{
    public static XDocument? GetXDocument(IntPtr docItemHandle)
    {
        XDocument? document = null;
        IntPtr docHandle = GetXDocumentHandle(docItemHandle);
        if (docHandle != IntPtr.Zero)
        {
            GCHandle gch = GCHandle.FromIntPtr(docHandle);
            document = (XDocument?)gch.Target;
        }
        return document;
    }

    public static void InitEngine(string prolog)
    {
        DomFacadeCallbacks facade = new DomFacadeCallbacks
        {
            CreateDocumentNode = DomImpl.CreateDocumentNode,
            CreateElementNode = DomImpl.CreateElementNode,
            CreateAttributeNode = DomImpl.CreateAttributeNode,
            SetStringValue = DomImpl.SetStringValue,
            Add = DomImpl.Add,
            FreeHandle = DomImpl.FreeHandle
        };
        InitEngine(facade, prolog);
    }
    public static string? XQuery(string xquery, IntPtr docHandle)
    {
        string? result = null;

        IntPtr ptr = RunXQuery(xquery, docHandle);
        if (ptr != IntPtr.Zero)
        {
            result = DomImpl.Utf8FromIntPtr(ptr);
            FreeString(ptr);
        }
        return result;
    }

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern void InitEngine(DomFacadeCallbacks callbacks, string prolog);

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern void ShutdownEngine();

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr GetXDocumentHandle(IntPtr docHandle);

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr LoadXML(string xmlFile);

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern void FreeXML(IntPtr docItemHandle);

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr RunXQuery(string xquery, IntPtr docHandle);

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern void FreeString(IntPtr p);
}
