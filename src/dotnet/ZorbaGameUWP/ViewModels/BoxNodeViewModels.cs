using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Xml.Linq;

namespace ZorbaGameUWP.ViewModels
{
    public class BoxNodeViewModels : NodeViewModelsBase
    {
        public ObservableCollection<BoxNodeViewModel> ActiveBoxesCollection { get; }
            = new ObservableCollection<BoxNodeViewModel>();
        private double _mapWidth;
        private double _mapHeight;

        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged([CallerMemberName] string propertyName = null) =>
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));


        public double MapWidth
        {
            get => _mapWidth;
            set { _mapWidth = value; OnPropertyChanged(); }
        }

        public double MapHeight
        {
            get => _mapHeight;
            set { _mapHeight = value; OnPropertyChanged(); }
        }

        override public XDocument LoadGame(int level)
        {
            XDocument document = LoadXML($"boxup{level}.xml");
            XElement root = document.Root;
            if (root != null)
            {
                string columns = root.Attribute("columns")?.Value;
                string rows = root.Attribute("rows")?.Value;
                if (columns != null && rows != null)
                {
                    MapWidth = double.Parse(columns) + 0.2;
                    MapHeight = double.Parse(rows) + 0.2;
                }
            }
            ActiveBoxesCollection.Clear();
            foreach (XElement element in document.Root.Elements())
            {
                BoxNodeViewModel model = new BoxNodeViewModel(element);
                AddAttributeModel(model, element);
                ActiveBoxesCollection.Add(model);
            }
            return document;
        }
    }
}
