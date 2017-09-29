using System;
using System.Windows.Input;
using AplikaciaWPF.ViewModels;
using AplikaciaModels;
//using AplikaciaModels.Messages;
using AplikaciaModels.Models;
using AplikaciaModels.Repositories;

namespace aplikaciaWPF.Commands
{
    public class AddActivity : ICommand
    {
        private readonly ActivityRepository _activityRepository;
        private readonly AddActivityViewModel _viewModel;  //otazka jestli pouzit tento
        private readonly IMessenger _messenger;

        public AddActivity(ActivityRepository activityRepository, AddActivityViewModel viewModel, IMessenger messenger)
        {
            _activityRepository = activityRepository;
            _viewModel = viewModel;
            _messenger = messenger;
        }

        public bool CanExecute(object parameter)
        {
            throw new NotImplementedException();
        }

        public void Execute(object parameter)
        {
            throw new NotImplementedException();
        }

        public event EventHandler CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }
    }
}
