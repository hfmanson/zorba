// xquery_engine.h

#pragma once

#include <cstdint>

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {

    typedef void* NodeHandle;

    typedef NodeHandle(*GetChildrenFn)(NodeHandle);
    typedef NodeHandle(*GetAttributesFn)(NodeHandle);
    typedef NodeHandle(*GetParentFn)(NodeHandle);
    typedef const char* (*GetNodeNameFn)(NodeHandle);
    typedef const char* (*GetNamespaceUriFn)(NodeHandle);
    typedef const char* (*GetStringValueFn)(NodeHandle);

    typedef void (*InsertChildFn)(NodeHandle parent, NodeHandle child, NodeHandle before);
    typedef void (*RemoveChildFn)(NodeHandle parent, NodeHandle child);
    typedef void (*ReplaceChildFn)(NodeHandle parent, NodeHandle oldChild, NodeHandle newChild);
    typedef void (*SetStringValueFn)(NodeHandle node, const char* utf8Value);

    typedef NodeHandle (*CreateDocumentNodeFn)();
    typedef NodeHandle (*CreateElementNodeFn)(const char* localName, const char* namespaceUri);
    typedef NodeHandle (*CreateAttributeNodeFn)(const char* localName, const char* namespaceUri, const char* value);
    typedef NodeHandle (*CreateTextNodeFn)(const char* value);
    typedef NodeHandle (*CreateCommentNodeFn)(const char* value);
    typedef NodeHandle (*CreateProcessingInstructionNodeFn)(const char* target, const char* data);

    typedef void (*AddFn)(NodeHandle container, NodeHandle obj);

    struct DomFacadeCallbacksNative
    {
        GetChildrenFn      GetChildren;
        GetAttributesFn    GetAttributes;
        GetParentFn        GetParent;
        GetNodeNameFn      GetNodeName;
        GetNamespaceUriFn  GetNamespaceUri;
        GetStringValueFn   GetStringValue;

        InsertChildFn      InsertChild;
        RemoveChildFn      RemoveChild;
        ReplaceChildFn     ReplaceChild;
        SetStringValueFn   SetStringValue;

        CreateDocumentNodeFn CreateDocumentNode;
        CreateElementNodeFn CreateElementNode;
        CreateAttributeNodeFn CreateAttributeNode;
        CreateTextNodeFn CreateTextNode;
        CreateCommentNodeFn CreateCommentNode;
        CreateProcessingInstructionNodeFn CreateProcessingInstructionNode;

        AddFn Add;
    };

    EXPORT void SetDomFacade(DomFacadeCallbacksNative callbacks);
    EXPORT void RunXQuery(const char *xquery);

    //EXPORT void SetRootNode(NodeHandle root);
    extern DomFacadeCallbacksNative g_facade;
}
