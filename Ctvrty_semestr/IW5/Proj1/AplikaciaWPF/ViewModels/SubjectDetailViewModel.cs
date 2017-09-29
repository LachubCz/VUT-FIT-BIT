using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using AplikaciaModels;
using AplikaciaModels.Repositories;
using AplikaciaModels.Models;
using System.Collections.ObjectModel;
using AplikaciaWPF.Commands;
using AplikaciaModels.Messages;


namespace AplikaciaWPF.ViewModels
{
    public class SubjectDetailViewModel : ViewModelBase
    {
        private readonly StudentRepository _studentRepository;
        private readonly SubjectRepository _subjectRepository;
        private readonly IMessenger _messenger;
        private SubjectDetailModel _detail;
        private ObservableCollection<StudentListModel> _students;

        public ICommand AddStudentToSubjectCommand { get; }

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

        public SubjectDetailModel Detail
        {
            get { return _detail; }
            set
            {
                if (Equals(value, Detail)) return;

                _detail = value;
                OnPropertyChanged();
            }
        }

        public SubjectDetailViewModel(SubjectRepository subjectRepository, StudentRepository studentRepository, IMessenger messenger)
        {
            _subjectRepository = subjectRepository;
            _studentRepository = studentRepository;
            _messenger = messenger;

            AddStudentToSubjectCommand = new AddStudentToSubjectCommand(subjectRepository, studentRepository, this, messenger);

            _messenger.Register<SelectedSubjectMessage>(SelectedSubject);
            //_messenger.Register<AddedStudentToSubjectMessage>(AddedStudentToSubjectMessageReceived);
        }

        public void OnLoad()
        {
            Students = new ObservableCollection<StudentListModel>(_studentRepository.GetAll());
        }

        /*public void AddedStudentToSubjectMessageReceived(AddedStudentToSubjectMessage message)
        {
            message.Model
        }*/

        public void SelectedSubject(SelectedSubjectMessage message)
        {
            Detail = _subjectRepository.GetById(message.Id);
        }


    }
}
