using System.Runtime.InteropServices;
using System.Xml.Linq;

namespace Zorba
{
    public static class DomImpl
    {
        public static string Utf8FromIntPtr(IntPtr ptr)
        {
            if (ptr == IntPtr.Zero)
                return null;

            // Find the length of the null‑terminated UTF‑8 string
            int len = 0;
            while (Marshal.ReadByte(ptr, len) != 0)
                len++;

            // Copy bytes into a managed array
            byte[] buffer = new byte[len];
            Marshal.Copy(ptr, buffer, 0, len);

            // Decode UTF‑8
            return System.Text.Encoding.UTF8.GetString(buffer);
        }

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
            object? objAttr = GCHandle.FromIntPtr(handle).Target;
            if (objAttr is XAttribute attr)
            {
                string newValue = Utf8FromIntPtr(utf8Value);
                if (newValue != attr.Value)
                {
                    attr.Value = newValue;
                }
            }
        }

        public static void Add(IntPtr containerHandle, IntPtr objectHandle)
        {
            object? objContainer = GCHandle.FromIntPtr(containerHandle).Target;
            if (objContainer is XContainer container)
            {
                object? obj = GCHandle.FromIntPtr(objectHandle).Target;
                container.Add(obj);
            }
        }

        public static void FreeHandle(IntPtr handle)
        {
            GCHandle.FromIntPtr(handle).Free();
        }
    }
}
