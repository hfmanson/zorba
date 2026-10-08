using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Media;
using ZorbaGameUWP.ViewModels;

namespace ZorbaGameUWP.Selectors
{
    public class BoxTemplateSelector : DataTemplateSelector
    {
        protected override DataTemplate SelectTemplateCore(object item, DependencyObject container)
        {
            if (item is BoxNodeViewModel vm && container is FrameworkElement element)
            {
                string key = vm.BoxupField;

                FrameworkElement current = element;
                while (current != null)
                {
                    if (current.Resources.TryGetValue(key, out object globalRes) && globalRes is DataTemplate globalTemplate)
                    {
                        element.SetBinding(Canvas.LeftProperty, new Binding { Path = new PropertyPath("Left"), Source = vm, Mode = BindingMode.OneWay });
                        element.SetBinding(Canvas.TopProperty, new Binding { Path = new PropertyPath("Top"), Source = vm, Mode = BindingMode.OneWay });
                        return globalTemplate;
                    }
                    current = (FrameworkElement) VisualTreeHelper.GetParent(current);
                }
            }

            return base.SelectTemplateCore(item, container);
        }
    }
}
