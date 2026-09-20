using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace ZorbaGame.ViewModels
{
    // A simple base class implementing INotifyPropertyChanged for data binding
    public abstract class NodeViewModelBase
    {
        public abstract void UpdateField(string AttrName, string AttrValue);
    }
}
