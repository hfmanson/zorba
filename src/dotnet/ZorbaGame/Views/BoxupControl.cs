using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Data;
using System.Xml.Linq;
using ZorbaGame.ViewModels;

namespace ZorbaGame.Views
{
    public class BoxupControl : Control
    {
        private static readonly ResourceDictionary resourceDict = new ResourceDictionary
        {
            Source = new System.Uri("ms-appx:///Views/BoxupStyles.xaml")
        };

        public BoxupControl(BoxNodeViewModel model, string styleKey)
        {
            ViewModel = model;
            // Automatically establish Canvas.Left binding for this instance
            this.SetBinding(Canvas.LeftProperty, new Binding
            {
                Path = new PropertyPath("ViewModel.Left"),
                Source = this
            });

            // Automatically establish Canvas.Top binding for this instance
            this.SetBinding(Canvas.TopProperty, new Binding
            {
                Path = new PropertyPath("ViewModel.Top"),
                Source = this
            });
            if (resourceDict.TryGetValue(styleKey, out object resource) &&
                resource is Style style)
            {
                Style = style;
            }
        }

        public static readonly DependencyProperty ViewModelProperty =
            DependencyProperty.Register(
                nameof(ViewModel),
                typeof(BoxNodeViewModel),
                typeof(BoxupControl),
                new PropertyMetadata(null));

        public BoxNodeViewModel ViewModel
        {
            get => (BoxNodeViewModel)GetValue(ViewModelProperty);
            set => SetValue(ViewModelProperty, value);
        }

        public static BoxupControl CreateBoxupControl(BoxNodeViewModel model, XElement element)
        {
            XAttribute? depth = element.Attribute(XName.Get("depth"));
            BoxupControl control;
            if (depth == null)
            {
                // elements without depth: mover and block
                if (element.Name.LocalName == "mover")
                {
                    control = new BoxupControl(model, "Mover");
                }
                else
                {
                    control = new BoxupControl(model, "Block");
                }
            }
            else if (depth.Value == "2")
            {
                control = new BoxupControl(model, "SmallBox");
            }
            else
            {
                control = new BoxupControl(model, "BigBox");
            }
            return control;
        }
    }
}
