using System.Xml.Linq;
using Windows.System;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using ZorbaGameUWP.ViewModels;

namespace ZorbaGameUWP
{
    public sealed partial class Boxup : Page
    {
        private XDocument _document;

        private NodeViewModelsBase _NodeViewModels;

        private int _level;

        public Boxup()
        {
            InitializeComponent();
            _level = 1;
            loadLevel();
        }

        public void loadLevel()
        {
            //game.Children.Clear();
            if (_NodeViewModels != null)
            {
                _NodeViewModels.FreeXML();
            }
            _NodeViewModels = new BoxNodeViewModels();
            _document = _NodeViewModels.LoadGame(_level);
            BoxupItemsControl.DataContext = _NodeViewModels;
        }

        public void cleanUp()
        {
            _NodeViewModels?.FreeXML();
        }

        private void boxupMove(int dx, int dy)
        {
            _NodeViewModels?.RunXQuery($"boxup:check-move(., {dx}, {dy})");
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            // Bind to the core window thread listener
            Window.Current.CoreWindow.KeyDown += CoreWindow_KeyDown;
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            base.OnNavigatedFrom(e);
            // Always unbind to prevent memory leaks
            Window.Current.CoreWindow.KeyDown -= CoreWindow_KeyDown;
        }

        private void CoreWindow_KeyDown(CoreWindow sender, KeyEventArgs args)
        {
            // This catches keys regardless of what element has focus
            if (args.VirtualKey == VirtualKey.Left)
            {
                boxupMove(-1, 0);
                args.Handled = true;
            }
            else if (args.VirtualKey == VirtualKey.Right)
            {
                boxupMove(1, 0);
                args.Handled = true;
            }
            else if (args.VirtualKey == VirtualKey.Up)
            {
                boxupMove(0, -1);
                args.Handled = true;
            }
            else if (args.VirtualKey == VirtualKey.Down)
            {
                boxupMove(0, 1);
                args.Handled = true;
            }
            else if (args.VirtualKey == VirtualKey.P)
            {
                if (_level > 1)
                {
                    _level--;
                    loadLevel();
                }
                args.Handled = true;
            }
            else if (args.VirtualKey == VirtualKey.N)
            {
                if (_level < 17)
                {
                    _level++;
                    loadLevel();
                }
                args.Handled = true;
            }
            else if (args.VirtualKey == VirtualKey.R)
            {
                loadLevel();
                args.Handled = true;
            }
        }
    }
}
