using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using MedicalDataManagerModel.Annotations;

namespace MedicalDataManagerModel
{
    public class TokenEditdialogViewModel : INotifyPropertyChanged
    {
        private string _tokenId;
        public event PropertyChangedEventHandler PropertyChanged;
        public ObservableCollection<Person> Persons { get; private set; }
        private Person _selectedPerson;
        public TokenEditdialogViewModel()
        {
            using (var context = new MedicalDataManagerDataBaseContainer())
            {
                this.Persons = new ObservableCollection<Person>(context.People.ToList());
            }
        }
        public Person SelectedPerson
        {
            get { return _selectedPerson; }
            set
            {
                _selectedPerson = value;
                OnPropertyChanged();
            }
        }
        public string TokenId
        {
            get { return _tokenId; }
            set
            {
                _tokenId = value;
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
                Token newToken = new Token()
                {
                    Id = int.Parse(TokenId),
                    Person = SelectedPerson
                };
                context.Tokens.Add(newToken);
                context.SaveChanges()
                ;
            }
        }
    }
}
