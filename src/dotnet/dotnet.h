// xquery_engine.h

#pragma once

#include <cstdint>

extern "C" {

    typedef void* NodeHandle;

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
        SetStringValueFn   SetStringValue;

        CreateDocumentNodeFn CreateDocumentNode;
        CreateElementNodeFn CreateElementNode;
        CreateAttributeNodeFn CreateAttributeNode;
        CreateTextNodeFn CreateTextNode;
        CreateCommentNodeFn CreateCommentNode;
        CreateProcessingInstructionNodeFn CreateProcessingInstructionNode;

        AddFn Add;
    };

    ZORBA_DLL_PUBLIC void InitEngine(DomFacadeCallbacksNative callbacks, const char* xmlFile);
    ZORBA_DLL_PUBLIC void ShutdownEngine();
    ZORBA_DLL_PUBLIC void RunXQuery(const char* xquery);
    ZORBA_DLL_PUBLIC NodeHandle GetDocument();
    extern DomFacadeCallbacksNative g_facade;
}
