using Microsoft.UI;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media;
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
                Control control;
                BoxNodeViewModel model = new BoxNodeViewModel(element);
                AddAttributeModel(model, element);
                XAttribute? depth = element.Attribute(XName.Get("depth"));
                if (depth == null)
                {
                    // elements without depth: mover and block
                    string LocalName = element.Name.LocalName;
                    if (LocalName == "mover")
                    {
                        MoverControl moverControl = new MoverControl
                        {
                            ViewModel = model
                        };
                        control = moverControl;
                    }
                    else
                    {
                        BlockControl blockControl = new BlockControl
                        {
                            ViewModel = model
                        };
                        control = blockControl;
                    }
                }
                else if (depth.Value == "2")
                {
                    SmallBoxControl smallBoxControl = new SmallBoxControl
                    {
                        ViewModel = model
                    };
                    control = smallBoxControl;
                }
                else
                {
                    BigBoxControl bigBoxControl = new BigBoxControl
                    {
                        ViewModel = model
                    };
                    control = bigBoxControl;
                }
                XAttribute? boxtype = element.Attribute(XName.Get("box-type"));
                if (boxtype != null)
                {
                    if (boxtype.Value == "source")
                    {
                        model.Stroke = new SolidColorBrush(Colors.Red);
                    }
                    else if (boxtype.Value == "destination")
                    {
                        model.Stroke = new SolidColorBrush(Colors.Blue);
                    }
                    else
                    {
                        model.Stroke = new SolidColorBrush(Colors.Black);
                    }
                }
                game.Children.Add(control);
            }
            return document;
        }
    }
}
