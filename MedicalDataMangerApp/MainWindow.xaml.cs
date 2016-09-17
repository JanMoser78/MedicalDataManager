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
using System.Windows.Navigation;
using System.Windows.Shapes;
using MedicalDataManagerModel;

namespace MedicalDataMangerApp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
           
        }

        private void Devices_Click(object sender, RoutedEventArgs e)
        {
            this.NavFrame.NavigationService.Navigate(new DevicePage());
        }

        private void Tokens_Click(object sender, RoutedEventArgs e)
        {
            this.NavFrame.NavigationService.Navigate(new TokenPage());
        }

        private void Persons_Click(object sender, RoutedEventArgs e)
        {
            this.NavFrame.NavigationService.Navigate(new PersonPage());
        }

        private void Substances_Click(object sender, RoutedEventArgs e)
        {
            this.NavFrame.NavigationService.Navigate(new SubstancePage());
        }

        private void Dosages_Click(object sender, RoutedEventArgs e)
        {
            this.NavFrame.NavigationService.Navigate(new DosagePage());
        }

        private void MedPlan_Click(object sender, RoutedEventArgs e)
        {
            this.NavFrame.NavigationService.Navigate(new MedicationPlanPage());
        }
    }
}
