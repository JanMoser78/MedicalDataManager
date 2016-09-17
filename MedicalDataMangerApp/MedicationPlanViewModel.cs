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
   public class MedicationPlanViewModel : INotifyPropertyChanged
    {
       public event PropertyChangedEventHandler PropertyChanged;

        public ObservableCollection<MedicationPlan> MedicationPlans { get; private set; }

       public MedicationPlanViewModel()
       {
           this.MedicationPlans = new ObservableCollection<MedicationPlan>();
          Refresh();
       }

       [NotifyPropertyChangedInvocator]
       protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
       {
           PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
       }

       public void Refresh()
       {
           this.MedicationPlans.Clear();
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                this.MedicationPlans = new ObservableCollection<MedicationPlan>(context.MedicationPlans.ToList());
            }

        }
    }
}
