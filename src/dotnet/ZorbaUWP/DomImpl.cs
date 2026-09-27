using System;
using System.Runtime.InteropServices;
using System.Xml.Linq;

namespace ZorbaUWP
{
    class DomImpl
    {
        public static IntPtr CreateDocumentNode()
        {
            XDocument doc = new XDocument();
            return GCHandle.ToIntPtr(GCHandle.Alloc(doc));
        }

        public static IntPtr CreateElementNode(string localName, string namespaceName)
        {
            XElement element = new XElement(XName.Get(localName, namespaceName));
            return GCHandle.ToIntPtr(GCHandle.Alloc(element));
        }

        public static IntPtr CreateAttributeNode(string localName, string namespaceName, string value)
        {
            XAttribute attr = new XAttribute(XName.Get(localName, namespaceName), value);
            return GCHandle.ToIntPtr(GCHandle.Alloc(attr));
        }

        public static void SetStringValue(IntPtr handle, IntPtr utf8Value)
        {
            object objAttr = GCHandle.FromIntPtr(handle).Target;
            if (objAttr is XAttribute attr)
            {
                string newValue = Marshal.PtrToStringAnsi(utf8Value);
                if (newValue != attr.Value)
                {
                    attr.Value = newValue;
                }
            }
        }

        public static void Add(IntPtr containerHandle, IntPtr objectHandle)
        {
            object objContainer = GCHandle.FromIntPtr(containerHandle).Target;
            if (objContainer is XContainer container)
            {
                object obj = GCHandle.FromIntPtr(objectHandle).Target;
                container.Add(obj);
            }
        }

        public static void FreeHandle(IntPtr handle)
        {
            GCHandle.FromIntPtr(handle).Free();
        }
    }
}
