using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using MedicalDataManagerModel;
using MedicalDataMangerApp.Annotations;

namespace MedicalDataMangerApp
{
    public class MedicationPlanItemDialogViewModel :INotifyPropertyChanged
    {
        private DateTime _validfrom;
        private DateTime _validTo;
        private bool _critical;
        private int _compartment;

        public event PropertyChangedEventHandler PropertyChanged;

        public MedicationPlanItemDialogViewModel()
        {
            GetDevices();
            GetTokens();

        }

        private void GetDevices()
        {
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                this.Devices = new ObservableCollection<Device>(context.Devices.ToList());
            }
        }

        private void GetTokens()
        {
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                this.Tokens = new ObservableCollection<Token>(context.Tokens.ToList());
            }
        }

        public ObservableCollection<Device> Devices { get; private set; }
        public ObservableCollection<Token> Tokens { get; private set; }

        public DateTime ValidFrom
        {
            get { return _validfrom; }
            set
            {
                _validfrom = value;
                OnPropertyChanged();
            }
        }

        public DateTime ValidTo
        {
            get { return _validTo; }
            set
            {
                _validTo = value;
                OnPropertyChanged();
            }
        }

        public bool Critical
        {
            get { return _critical; }
            set
            {
                _critical = value;
                OnPropertyChanged();
            }
        }
        public int Compartment
        {
            get { return _compartment; }
            set
            {
                _compartment = value;
                OnPropertyChanged();
            }
        }





        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}