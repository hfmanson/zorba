using System.Runtime.InteropServices;
using System.Xml.Linq;

namespace ConsoleApp1
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
        public static IntPtr CreateAttributeNode(string localName, string namespaceName, string value)
        {
            XAttribute attr = new XAttribute(XName.Get(localName, namespaceName), value);
            return GCHandle.ToIntPtr(GCHandle.Alloc(attr));
        }

        public static void SetStringValue(IntPtr handle, IntPtr utf8Value)
        {
            object? obj = GCHandle.FromIntPtr(handle).Target;
            if (obj is XAttribute attr)
            {
                attr.Value = Utf8FromIntPtr(utf8Value);
            }
        }

        public static void AppendAttribute(IntPtr elementHandle, IntPtr attrHandle)
        {
            //XElement elem = (XElement)((GCHandle)elementHandle).Target;
            //XAttribute attr = (XAttribute)((GCHandle)attrHandle).Target;

            ///elem.Attributes.Append(attr);
        }
    }
}
