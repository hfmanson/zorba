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

    typedef void (*FreeHandleFn)(void* handle);

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

        FreeHandleFn FreeHandle;
    };

    ZORBA_DLL_PUBLIC void InitEngine(DomFacadeCallbacksNative callbacks, const char* prolog);
    ZORBA_DLL_PUBLIC void ShutdownEngine();
    ZORBA_DLL_PUBLIC void* LoadXML(const char* xmlFile, bool validate);
    ZORBA_DLL_PUBLIC void FreeXML(void* docItemHandle);
    ZORBA_DLL_PUBLIC char* RunXQuery(const char* xquery, void* docItemHandle);
    ZORBA_DLL_PUBLIC void FreeString(char* p);
    ZORBA_DLL_PUBLIC NodeHandle GetXDocumentHandle(void* docItemHandle);
    extern DomFacadeCallbacksNative g_facade;
}
