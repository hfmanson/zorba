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

        private NodeViewModelsBase _NodeViewModels;

        private int _level;

        private static string module = @"
module namespace boxup=""http://mansoft.nl/boxup"";

declare function boxup:max-depth2($boxes, $column, $row) {
  for $box in $boxes[xs:integer(@column) eq $column][xs:integer(@row) eq $row]
  order by xs:integer($box/@depth) descending
  return $box
};

declare function boxup:max-depth($boxes, $column, $row) {
  boxup:max-depth2($boxes, $column, $row)[1]
};

declare updating function boxup:move($model-mover, $column, $row) {
  replace value of node $model-mover/@column with $column,
  replace value of node $model-mover/@row with $row  
};

declare updating function boxup:update($context, $model-mover, $x, $y, $new-x, $new-y, $dx, $dy) {
  let $boxes := $context/boxup:boxup/boxup:box
      , $mover-boxes := $boxes[@column eq $model-mover/@column][@row eq $model-mover/@row]
  return
  (
    boxup:move($model-mover, $new-x, $new-y),
    for $box in $mover-boxes
    let $box-depth1 := $mover-boxes[xs:integer(@depth) = 1]
    return
      if (xs:integer($box/@dx) ne -$dx or xs:integer($box/@dy) ne -$dy or xs:integer($box/@depth) eq 2 and not(empty($box-depth1)) and (xs:integer($box-depth1/@dx) ne -$dx or xs:integer($box-depth1/@dy) ne -$dy)) then
        (
          boxup:move($box, $new-x, $new-y)
        )
      else ()
    )
};

declare updating function boxup:check-move($context, $dx, $dy)  {
  let $model-element := $context/boxup:boxup
    , $model-mover := $model-element/boxup:mover
    , $boxes := $model-element/boxup:box
    , $blocks := $model-element/boxup:block
    , $column := xs:integer($model-mover/@column)
    , $row := xs:integer($model-mover/@row)
    , $new-column := $column + $dx
    , $new-row := $row + $dy
return
  (
    if ($new-column gt 0 and $new-column le xs:integer($model-element/@columns) and $new-row gt 0 and $new-row le xs:integer($model-element/@rows) and empty($blocks[@column eq $new-column][@row eq $new-row])) then
    (
      let $current-box := boxup:max-depth($boxes, $column, $row)
          , $new-box := boxup:max-depth($boxes, $new-column, $new-row)
      return
        if
          (
            empty($new-box) or
            xs:integer($new-box/@dx) eq $dx and xs:integer($new-box/@dy) eq $dy and (
              empty($current-box) or xs:integer($current-box/@depth) gt xs:integer($new-box/@depth) or xs:integer($current-box/@dx) eq -xs:integer($new-box/@dx) and xs:integer($current-box/@dy) eq -xs:integer($new-box/@dy)
            )
          )
        then
            boxup:update(., $model-mover, $column, $row, $new-column, $new-row, $dx, $dy)
        else
          ()      
    )
    else
      ()
  )
};
";

        public Boxup()
        {
            InitializeComponent();
            _level = 1;
            loadLevel();
        }

        public void loadLevel()
        {
            game.Children.Clear();
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

        private void boxupMove(int dx, int dy)
        {
            NativeEngine.RunXQuery($"import module namespace boxup=\"http://mansoft.nl/boxup\" at 'boxup.xqm'; boxup:check-move(., {dx}, {dy})");
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
