using System;
using System.Runtime.InteropServices;
using Windows.UI.Core.Preview;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using ZorbaUWP;
using Windows.ApplicationModel;
using System.Xml.Linq;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace PlaygroundUWP
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern IntPtr LoadPackagedLibrary(string lpwLibFileName, uint reserved = 0);

        private IntPtr DocPtr;
        private XDocument doc;

        private void loadPackages()
        {
            IntPtr hVC = LoadPackagedLibrary("vcruntime140d_app.dll");
            IntPtr hCharset = LoadPackagedLibrary("libcharset.dll");
            IntPtr hIconv = LoadPackagedLibrary("libiconv.dll");
            IntPtr hLzma = LoadPackagedLibrary("lzma.dll");
            IntPtr hZlib = LoadPackagedLibrary("zlib1.dll");
            IntPtr hXml2 = LoadPackagedLibrary("libxml2.dll");
            IntPtr hZorba = LoadPackagedLibrary("zorba_simplestore.dll");
        }

        public MainPage()
        {
            InitializeComponent();
            NativeEngine.InitEngine("import module namespace boxup='http://mansoft.nl/boxup' at 'Assets/boxup.xqm';");

            string packageDirectory = Package.Current.InstalledLocation.Path;
            string xmlFile = System.IO.Path.Combine(packageDirectory, "Assets", "boxup1.xml");

            DocPtr = NativeEngine.LoadXML(xmlFile, false);
            doc =  NativeEngine.GetXDocument(DocPtr);
            doc.Changed += Doc_Changed;
            SystemNavigationManagerPreview.GetForCurrentView().CloseRequested += App_CloseRequested;
        }

        private void Doc_Changed(object sender, XObjectChangeEventArgs e)
        {
            if (sender is XAttribute attr)
            {
                string attrName = attr.Name.LocalName;
                string atrrValue = attr.Value;
                int x = 0;
            }
        }

        private void App_CloseRequested(object sender, SystemNavigationCloseRequestedPreviewEventArgs e)
        {
            doc.Changed -= Doc_Changed;
            NativeEngine.ShutdownEngine();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            string xq = NativeEngine.XQuery(xquery.Text, DocPtr);
            if (xq != null) result.Text = xq;
        }
    }
}
