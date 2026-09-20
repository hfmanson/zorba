using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using ZorbaGame.ViewModels;

namespace ZorbaGame.Views
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
