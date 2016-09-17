using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using MedicalDataManagerModel;
using MedicalDataMangerApp.Annotations;

namespace MedicalDataMangerApp
{
    public class DeviceEditViewModel : INotifyPropertyChanged
    {
        private Guid _deviceID;
        public event PropertyChangedEventHandler PropertyChanged;

        public Guid DeviceId
        {
            get { return _deviceID; }
            set
            {
                _deviceID = value;
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
                context.Devices.Add(actDevice);
                context.SaveChanges();
            }
        }
    }
} 