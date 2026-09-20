using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using ZorbaGame.ViewModels;

namespace ZorbaGame.Views
{
    public sealed partial class BlockControl : UserControl
    {
        public BlockControl()
        {
            InitializeComponent();
        }

        public static readonly DependencyProperty ViewModelProperty =
            DependencyProperty.Register(
                nameof(ViewModel),
                typeof(BoxNodeViewModel),
                typeof(BlockControl),
                new PropertyMetadata(null));

        public BoxNodeViewModel ViewModel
        {
            get => (BoxNodeViewModel)GetValue(ViewModelProperty);
            set => SetValue(ViewModelProperty, value);
        }
    }
}
