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
    /// Interaction logic for MedicationPlanDataItemDialog.xaml
    /// </summary>
    public partial class MedicationPlanDataItemDialog : Window
    {
        private MedicationPlanItemDialogViewModel _viewmodel;

        public MedicationPlanDataItemDialog()
        {
            InitializeComponent();
            _viewmodel = new MedicationPlanItemDialogViewModel();
            this.DataContext = _viewmodel;
        }

        private void CancelClick(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
