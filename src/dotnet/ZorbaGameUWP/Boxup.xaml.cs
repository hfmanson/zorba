using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Xml.Linq;
using Windows.System;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using ZorbaGameUWP.ViewModels;
using System;

namespace ZorbaGameUWP
{
    public sealed partial class Boxup : Page
    {
        private double _width;

        private double _height;

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
            game.Children.Clear();
            if (_NodeViewModels != null)
            {
                _NodeViewModels.FreeXML();
            }
            _NodeViewModels = new BoxNodeViewModels();
            _document = _NodeViewModels.LoadGame(game, _level);
            XElement root = _document.Root;
            if (root != null)
            {
                string columns = root.Attribute("columns")?.Value;
                string rows = root.Attribute("rows")?.Value;
                if (columns != null && rows != null)
                {
                    Width = double.Parse(columns) + 0.2;
                    Height = double.Parse(rows) + 0.2;

                    game.Width = Width;
                    game.Height = Height;
                }
            }
        }

        public void cleanUp()
        {
            _NodeViewModels.FreeXML();
        }

        private void boxupMove(int dx, int dy)
        {
            string error = _NodeViewModels.RunXQuery($"boxup:check-move(., {dx}, {dy})");
            if (error != null)
            {
                ContentDialog errorDialog = new ContentDialog
                {
                    Title = "XQuery Error",
                    Content = error,
                    CloseButtonText = "OK",

                    // 2. CRUCIAL: Point the dialog to the visual root of your current XAML view
                    // TODO
                    //XamlRoot = Content.XamlRoot
                };

                errorDialog?.ShowAsync();
            }
        }


        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public double Width
        {
            get => _width;
            set { _width = value; OnPropertyChanged(); }
        }

        public double Height
        {
            get => _height;
            set { _height = value; OnPropertyChanged(); }
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
