using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using MedicalDataManagerModel;
using MedicalDataMangerApp.Annotations;

namespace MedicalDataMangerApp
{
    public class TokenPageViewModel :INotifyPropertyChanged

    {
        public TokenPageViewModel()
        {
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                Tokens = new ObservableCollection<Token>(context.Tokens.ToList());
            }
        }
        public ObservableCollection<Token> Tokens { get; private set; }
        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
