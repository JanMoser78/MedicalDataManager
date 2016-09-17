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

namespace MedicalDataMangerApp
{
    /// <summary>
    /// Interaction logic for MedicationPlanEdit.xaml
    /// </summary>
    public partial class MedicationPlanEdit : Window
    {
        private MedicationPlanEditViewModel _viewModel = new MedicationPlanEditViewModel();
        public MedicationPlanEdit()
        {
            InitializeComponent();
            this.DataContext = _viewModel;
        }

        private void AddItem(object sender, RoutedEventArgs e)
        {
            var editView = new MedicationPlanDataItemDialog();
            editView.ShowDialog();
            _viewModel.Refresh();
        }
    }
}
