using Microsoft.UI.Xaml.Controls;
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection.Metadata;
using System.Xml.Linq;
using Zorba;

namespace ZorbaGame.ViewModels
{
    public abstract class NodeViewModelsBase
    {
        private IntPtr docItemHandle;
        XDocument? document;
        private readonly Dictionary<XAttribute, NodeViewModelBase> _attributeModels = new Dictionary<XAttribute, NodeViewModelBase>();

        public abstract XDocument LoadGame(Canvas game, int level);

        private void Document_Changed(object? sender, XObjectChangeEventArgs e)
        {
            if (sender is XAttribute attr && _attributeModels.TryGetValue(attr, out NodeViewModelBase? model))
            {
                model.UpdateField(attr.Name.LocalName, attr.Value);
            }
        }

        protected XDocument LoadXML(string XMLFile)
        {
            var path = Path.Combine(AppContext.BaseDirectory, "Assets", XMLFile);
            docItemHandle = NativeEngine.LoadXML(XMLFile, true);
            document = NativeEngine.GetXDocument(docItemHandle);
            document?.Changed += Document_Changed;
            return document;
        }

        public void FreeXML()
        {
            document?.Changed -= Document_Changed;
            document = null;
            NativeEngine.FreeXML(docItemHandle);
            docItemHandle = IntPtr.Zero;
        }

        public void RunXQuery(string xquery)
        {
            NativeEngine.XQuery(xquery, docItemHandle);
        }

        protected void AddAttributeModel(NodeViewModelBase model, XElement element)
        {
            foreach (XAttribute attr in element.Attributes())
            {
                _attributeModels[attr] = model;
            }
        }
    }
}
