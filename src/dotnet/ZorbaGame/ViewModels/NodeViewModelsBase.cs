using Microsoft.UI.Xaml.Controls;
using System;
using System.Collections.Generic;
using System.IO;
using System.Xml.Linq;
using Zorba;

namespace ZorbaGame.ViewModels
{
    public abstract class NodeViewModelsBase
    {
        private readonly Dictionary<XAttribute, NodeViewModelBase> _attributeModels = new Dictionary<XAttribute, NodeViewModelBase>();

        public abstract XDocument LoadGame(Canvas game, int level);

        private void Document_Changed(object? sender, XObjectChangeEventArgs e)
        {
            if (sender is XAttribute attr && _attributeModels.TryGetValue(attr, out NodeViewModelBase? model))
            {
                model.UpdateField(attr.Name.LocalName, attr.Value);
            }
        }

        public static void InitEngine(string? xmlFile)
        {
            //Console.WriteLine("Current directory: " + Environment.CurrentDirectory);
            //Console.WriteLine("PATH: " + Environment.GetEnvironmentVariable("PATH"));
            DomFacadeCallbacks facade = new DomFacadeCallbacks
            {
                CreateDocumentNode = DomImpl.CreateDocumentNode,
                CreateElementNode = DomImpl.CreateElementNode,
                CreateAttributeNode = DomImpl.CreateAttributeNode,
                SetStringValue = DomImpl.SetStringValue,
                Add = DomImpl.Add
            };

            NativeEngine.InitEngine(facade, xmlFile);
        }
        public static void ShutdownEngine()
        {
            NativeEngine.ShutdownEngine();
        }

        protected XDocument LoadXML(string XMLFile)
        {
            var path = Path.Combine(AppContext.BaseDirectory, "Assets", XMLFile);
            //XDocument document = XDocument.Load(path);
            InitEngine(path);
            XDocument document = NativeEngine.getXDocument();
            document.Changed += Document_Changed; ;
            return document;
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
