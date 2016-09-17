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
        private Token _selectedToken    ;
        private Device _selectedDevice;
        private Dosage _selectedDosage;

        public event PropertyChangedEventHandler PropertyChanged;

        public MedicationPlanItemDialogViewModel()
        {
            GetDevices();
            GetTokens();
            GetDosages();
            this.ValidFrom = DateTime.Now;
            this.ValidTo = DateTime.Now.AddHours(1);
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

        private void GetDosages()
        {
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                this.Dosages = new ObservableCollection<Dosage>(context.Dosages.ToList());
            }
        }

        public void SaveData()
        {
            MedicationPlan medPlan = new MedicationPlan();
            medPlan.TokenId = SelectedToken.Id;
            medPlan.CompartmentNbr = Compartment;
            medPlan.Critical = Critical;
            medPlan.DeviceId = SelectedDevice.Id;
            medPlan.DosageId = SelectedDosage.Id;
            medPlan.ValidFrom = ValidFrom;
            medPlan.Validto = ValidTo;
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                context.MedicationPlans.Add(medPlan);
                context.SaveChanges();
            }


        }
        public ObservableCollection<Device> Devices { get; private set; }
        public ObservableCollection<Token> Tokens { get; private set; }

        public ObservableCollection<Dosage> Dosages { get; private set; }

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

        public Token SelectedToken
        {
            get { return _selectedToken; }
            set
            {
                _selectedToken = value;
                OnPropertyChanged();
            }
        }

        public Dosage SelectedDosage
        {
            get { return _selectedDosage; }
            set
            {
                _selectedDosage = value;
                OnPropertyChanged();
            }
        }

        public Device SelectedDevice
        {
            get { return _selectedDevice; }
            set
            {
                _selectedDevice = value;
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