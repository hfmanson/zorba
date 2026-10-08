using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Xml.Linq;

namespace ZorbaGameUWP.ViewModels
{
    public class BoxNodeViewModel : NodeViewModelBase, INotifyPropertyChanged
    {
        private double _left;
        private double _top;
        private double _boxRotation;
        private string _stroke;
        private string _boxupField;

        public BoxNodeViewModel(XElement element)
        {
            string row = element.Attribute("row")?.Value ?? "0";
            UpdateField("row", row);
            string column = element.Attribute("column")?.Value ?? "0";
            UpdateField("column", column);

            double dx = double.Parse(element.Attribute("dx")?.Value ?? "0");
            double dy = double.Parse(element.Attribute("dy")?.Value ?? "0");
            UpdateRotation(dx, dy);

            XAttribute boxtype = element.Attribute(XName.Get("box-type"));
            _stroke = "black";
            if (boxtype != null)
            {
                if (boxtype.Value == "source") _stroke = "red";
                else if (boxtype.Value == "destination") _stroke = "blue";
            }

            XAttribute depth = element.Attribute(XName.Get("depth"));
            if (depth == null)
            {
                _boxupField = element.Name.LocalName == "mover" ? "Mover" : "Block";
            }
            else
            {
                _boxupField = depth.Value == "2" ? "SmallBox" : "BigBox";
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged([CallerMemberName] string propertyName = null) =>
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));

        public double Left { get => _left; set { _left = value; OnPropertyChanged(); } }
        public double Top { get => _top; set { _top = value; OnPropertyChanged(); } }
        public double BoxRotation { get => _boxRotation; set { _boxRotation = value; OnPropertyChanged(); } }
        public string Stroke { get => _stroke; set { _stroke = value; OnPropertyChanged(); } }
        public string BoxupField { get => _boxupField; set { _boxupField = value; OnPropertyChanged(); } }

        public override void UpdateField(string AttrName, string AttrValue)
        {
            double doubleValue = double.Parse(AttrValue);
            if (AttrName == "row") Top = doubleValue - 1.0;
            else if (AttrName == "column") Left = doubleValue - 1.0;
        }

        public void UpdateRotation(double dx, double dy)
        {
            BoxRotation = dy == 1.0 ? 0 : dy == -1.0 ? 180.0 : dx == 1 ? -90.0 : 90.0;
        }
    }
}
