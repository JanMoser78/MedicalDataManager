using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Threading;
using MedicalDataManagerModel;
using MedicalDataMangerApp.Annotations;

namespace MedicalDataMangerApp
{
    public class MedicationPlanEditViewModel : INotifyPropertyChanged
    {
        
        public event PropertyChangedEventHandler PropertyChanged;
        public ObservableCollection<MedicationPlan> MedicationPlans { get; private set; }
        public void Refresh()
        {
            this.MedicationPlans.Clear();
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                MedicationPlans = new ObservableCollection<MedicationPlan>(context.MedicationPlans.ToList());
            }
            
        }

        public MedicationPlanEditViewModel()
        {
            this.MedicationPlans = new ObservableCollection<MedicationPlan>();
            Refresh();
        }

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}