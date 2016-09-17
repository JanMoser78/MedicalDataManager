using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using MedicalDataManagerModel;
using MedicalDataMangerApp.Annotations;

namespace MedicalDataMangerApp
{
    public class DeviceEditViewModel : INotifyPropertyChanged
    {
        private string _deviceID;
        private int _nbrCompartment;
        public event PropertyChangedEventHandler PropertyChanged;

        public string DeviceId
        {
            get { return _deviceID; }
            set
            {
                _deviceID = value;
                OnPropertyChanged();
            }
        }

        public int CompartmentNbr
        {
            get { return _nbrCompartment; }
            set
            {
                _nbrCompartment = value;
                OnPropertyChanged();
            }
        }

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }


        public void SaveData()
        {
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                Device actDevice = new Device();
                actDevice.Id = this.DeviceId;
                actDevice.NbrOfCompartments = this.CompartmentNbr;
                context.Devices.Add(actDevice);
                context.SaveChanges();
            }
        }
    }
} 