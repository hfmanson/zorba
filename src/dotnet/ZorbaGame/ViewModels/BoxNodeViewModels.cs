using Microsoft.UI.Xaml.Controls;
using System.Xml.Linq;
using ZorbaGame.Views;

namespace ZorbaGame.ViewModels
{
    public class BoxNodeViewModels : NodeViewModelsBase
    {
        override public XDocument LoadGame(Canvas game, int level)
        {
            XDocument document = LoadXML($"boxup{level}.xml");
            foreach (XElement element in document.Root.Elements())
            {
                BoxNodeViewModel model = new BoxNodeViewModel(element);
                AddAttributeModel(model, element);
                game.Children.Add(BoxupControl.CreateBoxupControl(model, element));
            }
            return document;
        }
    }
}
