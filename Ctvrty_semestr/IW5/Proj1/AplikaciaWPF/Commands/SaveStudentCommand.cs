using System;
using System.Windows.Input;
using AplikaciaModels;
using AplikaciaModels.Models;
using AplikaciaModels.Repositories;
using AplikaciaWPF.ViewModels;
using AplikaciaModels.Messages;

namespace AplikaciaWPF.Commands
{
    class SaveStudentCommand : ICommand
    {
        private readonly StudentRepository _studentRepository;
        private readonly StudentDetailViewModel _viewModel;
        private readonly IMessenger _messenger;

        public SaveStudentCommand(StudentRepository studentRepository, StudentDetailViewModel viewModel, IMessenger messenger)
        {
            _studentRepository = studentRepository;
            _viewModel = viewModel;
            _messenger = messenger;
        }
        public event EventHandler CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }

        public bool CanExecute(object parameter)
        {
            var detail = parameter as StudentDetailModel;

            if (detail != null)
            {
                return !string.IsNullOrWhiteSpace(detail.SurName)
                    && !string.IsNullOrWhiteSpace(detail.Name) && !string.IsNullOrWhiteSpace(detail.Login);
            }

            return false;
        }

        public void Execute(object parameter)
        {
            var detail = parameter as StudentDetailModel;

            if (detail == null)
            {
                return;
            }

            if (detail.Id != Guid.Empty)
            {
                _studentRepository.UpdateStudent(detail);
            }
            else
            {
                _viewModel.Detail = _studentRepository.InsertStudent(detail);
            }

            _messenger.Send(new UpdatedStudentMessage(detail));
        }
    }
}
