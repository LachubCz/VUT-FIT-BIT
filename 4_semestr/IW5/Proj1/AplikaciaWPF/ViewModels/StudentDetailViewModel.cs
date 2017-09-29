using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using AplikaciaModels;
using AplikaciaModels.Repositories;
using AplikaciaModels.Models;
using AplikaciaWPF.Commands;
using AplikaciaModels.Messages;
using Microsoft.Win32;

namespace AplikaciaWPF.ViewModels
{
    public class StudentDetailViewModel : ViewModelBase
    {
        private readonly StudentRepository _studentRepository;
        private readonly IMessenger _messenger;
        private StudentDetailModel _detail;

        public StudentDetailModel Detail
        {
            get { return _detail; }
            set
            {
                if (Equals(value, Detail)) return;

                _detail = value;
                OnPropertyChanged();
            }
        }


        public ICommand CreateStudentCommand { get; }

        public ICommand DeleteCommand { get; }

        public ICommand SaveCommand { get; }

        public StudentDetailViewModel(StudentRepository studentRepository, IMessenger messenger)
        {
            _studentRepository = studentRepository;
            _messenger = messenger;
            DeleteCommand = new RelayCommand(Delete);
            SaveCommand = new SaveStudentCommand(studentRepository, this, messenger);
            CreateStudentCommand = new RelayCommand(() => _messenger.Send(new NewStudentMessage()));

            _messenger.Register<SelectedStudentMessage>(SelectedStudent);
            _messenger.Register<NewStudentMessage>(NewStudentMessageReceived);
        }

        private void NewStudentMessageReceived(NewStudentMessage message)
        {
            Detail = new StudentDetailModel();
        }

        private void SelectedStudent(SelectedStudentMessage message)
        {
            Detail = _studentRepository.FindByid(message.Id);
        }

        public void Delete()
        {
            if (IsSavedStudent())
            {
                _studentRepository.DeleteStudentByTheId(Detail.Id);
                _messenger.Send(new DeletedStudentMessage(Detail.Id));
            }

            Detail = null;
        }

        private bool IsSavedStudent()
        {
            return Detail.Id != Guid.Empty;
        }
    }
}
