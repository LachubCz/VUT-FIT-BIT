using AplikaciaWPF.ViewModels;
using AplikaciaModels;
using AplikaciaModels.Repositories;
using System;

namespace AplikaciaWPF
{
    public class ViewModelLocator
    {
        private readonly Messenger _messenger = new Messenger();
        private readonly StudentRepository _studentRepository = new StudentRepository();
        private readonly SubjectRepository _subjectRepository = new SubjectRepository();

        public MainViewModel MainViewModel => CreateMainViewModel();

        public StudentListViewModel StudentListViewModel => CreateStudentListViewModel();

        public StudentDetailViewModel StudentDetailViewModel => CreateStudentDetailViewModel();

        public SubjectDetailViewModel SubjectDetailViewModel => CreateSubjectDetailViewModel();

        private SubjectDetailViewModel CreateSubjectDetailViewModel()
        {
            return new SubjectDetailViewModel(_subjectRepository, _studentRepository, _messenger);
        }

        private MainViewModel CreateMainViewModel()
        {
            return new MainViewModel(_subjectRepository, _messenger);
        }

        private StudentDetailViewModel CreateStudentDetailViewModel()
        {
            return new StudentDetailViewModel(_studentRepository, _messenger);
        }

        private StudentListViewModel CreateStudentListViewModel()
        {

            return new StudentListViewModel(_studentRepository, _messenger);
        }
    }
}