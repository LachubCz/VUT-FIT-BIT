using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using AplikaciaModels;
using AplikaciaModels.Repositories;
using AplikaciaModels.Models;
using AplikaciaWPF.Commands;
using AplikaciaModels.Messages;

namespace AplikaciaWPF.ViewModels
{
    public class StudentListViewModel : ViewModelBase
    {
        private ObservableCollection<StudentListModel> _students;
        private readonly StudentRepository _studentRepository;
        private readonly IMessenger _messenger;
        private string _str;
        private string _type;

        public ObservableCollection<StudentListModel> Students
        {
            get { return _students; }
            set
            {
                if (Equals(value, _students)) return;
                _students = value;
                OnPropertyChanged();
            }
        }

        public string Type
        {
            get { return _type; }
            set
            {
                if (value == _type) return;
                _type = value;
                OnPropertyChanged();
            }
        }

        public string Str
        {
            get { return _str; }
            set
            {
                if (value == _str) return;
                _str = value;
                OnPropertyChanged();
            }
        }

        public ICommand SelectStudentCommand { get; }
        public ICommand FilterCommand { get; }
        public ICommand ShowAllCommand { get; }

        public StudentListViewModel(StudentRepository studentRepository, IMessenger messenger)
        {
            _studentRepository = studentRepository;
            _messenger = messenger;

            _messenger.Register<DeletedStudentMessage>(DeletedStudentMessageReceived);
            _messenger.Register<UpdatedStudentMessage>((p) => OnLoad());
            SelectStudentCommand = new RelayCommand(StudentSelectionChanged);
            FilterCommand = new RelayCommand(Filter);
            ShowAllCommand = new RelayCommand(OnLoad);
        }

        public void Filter()
        {
            if (Type.ToString() == "System.Windows.Controls.ComboBoxItem: Login")
                Students = new ObservableCollection<StudentListModel>(_studentRepository.FindListByLogin(Str));
            if (Type.ToString() == "System.Windows.Controls.ComboBoxItem: Jméno")
                Students = new ObservableCollection<StudentListModel>(_studentRepository.FindListByName(Str));
            if (Type.ToString() == "System.Windows.Controls.ComboBoxItem: Přijmení")
                Students = new ObservableCollection<StudentListModel>(_studentRepository.FindListBySurName(Str));
        }

        public void OnLoad()
        {
            Students = new ObservableCollection<StudentListModel>(_studentRepository.GetAll());
        }

        public void StudentSelectionChanged(object parameter)
        {

            var studentId = parameter as StudentListModel;

            if (studentId == null)
            {
                return;
            }

            _messenger.Send(new SelectedStudentMessage() { Id = studentId.Id });
        }

        private void DeletedStudentMessageReceived(DeletedStudentMessage message)
        {
            var deletedStudent = Students.FirstOrDefault(s => s.Id == message.StudentId);
            if (deletedStudent != null)
            {
                Students.Remove(deletedStudent);
            }
        }
    }
}
