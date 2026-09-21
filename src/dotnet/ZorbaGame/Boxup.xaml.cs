using Microsoft.UI.Xaml;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Xml.Linq;
using Windows.System;
using ZorbaGame.ViewModels;

namespace ZorbaGame
{
    public sealed partial class Boxup : Window
    {
        private double _width;

        private double _height;

        private XDocument _document;

        private NodeViewModelsBase? _NodeViewModels;

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
            XElement? root = _document.Root;
            if (root != null)
            {
                string? columns = root.Attribute("columns")?.Value;
                string? rows = root.Attribute("rows")?.Value;
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
            _NodeViewModels?.FreeXML();
        }

        private void boxupMove(int dx, int dy)
        {
            _NodeViewModels?.RunXQuery($"boxmod:check-move(., {dx}, {dy})");
        }


        public event PropertyChangedEventHandler? PropertyChanged;

        private void OnPropertyChanged([CallerMemberName] string? propertyName = null)
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

        private void Grid_KeyDown(object sender, Microsoft.UI.Xaml.Input.KeyRoutedEventArgs e)
        {
            // This catches keys regardless of what element has focus
            if (e.Key == VirtualKey.Left)
            {
                boxupMove(-1, 0);
            }
            else if (e.Key == VirtualKey.Right)
            {
                boxupMove(1, 0);
            }
            else if (e.Key == VirtualKey.Up)
            {
                boxupMove(0, -1);
            }
            else if (e.Key == VirtualKey.Down)
            {
                boxupMove(0, 1);
            }
            else if (e.Key == VirtualKey.P)
            {
                if (_level > 1)
                {
                    _level--;
                    loadLevel();
                }
            }
            else if (e.Key == VirtualKey.N)
            {
                if (_level < 17)
                {
                    _level++;
                    loadLevel();
                }
            }
        }
    }
}
