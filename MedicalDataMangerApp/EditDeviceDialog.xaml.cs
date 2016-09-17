using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using MedicalDataManagerModel;

namespace MedicalDataMangerApp
{
    /// <summary>
    /// Interaction logic for EditDeviceDialog.xaml
    /// </summary>
    public partial class EditDeviceDialog : Window
    {
        private DeviceEditViewModel _viewModel;
        public EditDeviceDialog()
        {
            InitializeComponent();
            _viewModel = new DeviceEditViewModel();
            this.DataContext = _viewModel;

        }

        private void ButtonSave(object sender, RoutedEventArgs e)
        {
            _viewModel.SaveData();
            this.Close();
        }

        private void ButtonCancel(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
