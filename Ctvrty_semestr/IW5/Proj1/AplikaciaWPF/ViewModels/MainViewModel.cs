using System.Windows.Input;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using AplikaciaWPF.Commands;
using AplikaciaModels.Models;
using AplikaciaModels;
using AplikaciaModels.Repositories;
using AplikaciaModels.Messages;

namespace AplikaciaWPF.ViewModels
{
    public class MainViewModel : ViewModelBase
    {
        private readonly IMessenger _messenger;

        private readonly SubjectRepository _subjectRepository;
        private ObservableCollection<SubjectListModel> _subjects;

        public ICommand SelectSubjectCommand { get; }
        public ICommand CreateStudentCommand { get; }

        public ObservableCollection<SubjectListModel> Subjects
        {
            get { return _subjects; }
            set
            {
                if (Equals(value, _subjects)) return;
                _subjects = value;
                OnPropertyChanged();
            }
        }

        public MainViewModel(SubjectRepository subject_repository, IMessenger messenger)
        {
            _subjectRepository = subject_repository;
            _messenger = messenger;

            SelectSubjectCommand = new RelayCommand(SubjectSelectionChanged);
            CreateStudentCommand = new RelayCommand(() => _messenger.Send(new NewStudentMessage()));
        }

        public void OnLoad()
        {
            Subjects =new ObservableCollection<SubjectListModel>(_subjectRepository.GetAll());
        }

        public void SubjectSelectionChanged(object parameter)
        {
            var subject = (SubjectListModel)parameter;

            if (subject == null)
            {
                return;
            }

            _messenger.Send(new SelectedSubjectMessage() { Id = subject.Id });
        }
    }
}
