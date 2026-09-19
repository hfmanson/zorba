using System;
using System.Runtime.InteropServices;

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr GetChildrenDelegate(IntPtr nodeHandle);      // returns handle to an array/list
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr GetAttributesDelegate(IntPtr nodeHandle);
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr GetParentDelegate(IntPtr nodeHandle);
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr GetNodeNameDelegate(IntPtr nodeHandle);      // returns pointer to UTF‑8 string
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr GetNamespaceUriDelegate(IntPtr nodeHandle);
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate IntPtr GetStringValueDelegate(IntPtr nodeHandle);

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void InsertChildDelegate(IntPtr parentHandle, IntPtr childHandle, IntPtr beforeHandle);
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void RemoveChildDelegate(IntPtr parentHandle, IntPtr childHandle);
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void ReplaceChildDelegate(IntPtr parentHandle, IntPtr oldChildHandle, IntPtr newChildHandle);
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
    public GetChildrenDelegate GetChildren;
    public GetAttributesDelegate GetAttributes;
    public GetParentDelegate GetParent;
    public GetNodeNameDelegate GetNodeName;
    public GetNamespaceUriDelegate GetNamespaceUri;
    public GetStringValueDelegate GetStringValue;

    public InsertChildDelegate InsertChild;
    public RemoveChildDelegate RemoveChild;
    public ReplaceChildDelegate ReplaceChild;
    public SetStringValueDelegate SetStringValue;

    public CreateDocumentNodeDelegate CreateDocumentNode;
    public CreateElementNodeDelegate CreateElementNode;
    public CreateAttributeNodeDelegate CreateAttributeNode;
    public CreateTextNodeDelegate CreateTextNode;
    public CreateCommentNodeDelegate CreateCommentNode;
    public CreateProcessingInstructionNodeDelegate CreateProcessingInstructionNode;
    public AddDelegate Add;
}

internal static class NativeEngine
{
    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetDomFacade(DomFacadeCallbacks callbacks);

    [DllImport("zorba_simplestore", CallingConvention = CallingConvention.Cdecl)]
    public static extern void RunXQuery(string xquery);

    //[DllImport("xquery_engine", CallingConvention = CallingConvention.Cdecl)]
    //public static extern void SetRootNode(IntPtr rootHandle);
}
