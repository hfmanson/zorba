using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using ZorbaGameUWP.ViewModels;

namespace ZorbaGameUWP.Views
{
    public sealed partial class BigBoxControl : UserControl
    {
        public BigBoxControl()
        {
            InitializeComponent();
        }

        public static readonly DependencyProperty ViewModelProperty =
            DependencyProperty.Register(
                nameof(ViewModel),
                typeof(BoxNodeViewModel),
                typeof(BigBoxControl),
                new PropertyMetadata(null));

        public BoxNodeViewModel ViewModel
        {
            get => (BoxNodeViewModel)GetValue(ViewModelProperty);
            set => SetValue(ViewModelProperty, value);
        }
    }
}
