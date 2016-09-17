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
    /// Interaction logic for TokenEditDialog.xaml
    /// </summary>
    public partial class TokenEditDialog : Window
    {
        public TokenEditDialog()
        {
            InitializeComponent();
            this.DataContext = new TokenEditdialogViewModel();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            ((TokenEditdialogViewModel) this.DataContext).SaveData();
            this.Close();
        }
    }
}
