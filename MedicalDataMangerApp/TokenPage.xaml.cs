﻿using MedicalDataManagerModel;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
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

namespace MedicalDataMangerApp
{
    /// <summary>
    /// Interaction logic for TokenPage.xaml
    /// </summary>
    public partial class TokenPage : Page

    { 
    public TokenPage()
{
    InitializeComponent();
   this.DataContext = new TokenPageViewModel();

}

}
}



