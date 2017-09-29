using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using AplikaciaDB;
using AplikaciaModels.Mappers;
using AplikaciaDB.Entities;
using AplikaciaModels.Models;

namespace AplikaciaModels.Repositories
{
    public class StudentRepository
    {

        private StudentMapper mapper = new StudentMapper();

        public StudentEntity FindByName(string name)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var student = aplikaciaDbContext
                    .Students
                    //SELECT *
                    //FROM Activity JOIN ScoredActivity ON Activity.Id = ScoredActivity.ActivityId;
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .Include(r => r.RegisteredActivities.Select(i => i.RegisteredStudents))
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .FirstOrDefault(r => r.Name == name);
                return student;
            }
        }

        public List<StudentListModel> FindListByName(string name)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                return aplikaciaDbContext
                    .Students
                    .Where((r => r.Name == name))
                    //SELECT *
                    //FROM Activity JOIN ScoredActivity ON Activity.Id = ScoredActivity.ActivityId;
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .Include(r => r.RegisteredActivities.Select(i => i.RegisteredStudents))
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .Select(mapper.MapEntityToListModel)
                    .ToList();
            }
        }

        public List<StudentListModel> FindListBySurName(string name)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                return aplikaciaDbContext
                    .Students
                    .Where((r => r.SurName == name))
                    //SELECT *
                    //FROM Activity JOIN ScoredActivity ON Activity.Id = ScoredActivity.ActivityId;
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .Include(r => r.RegisteredActivities.Select(i => i.RegisteredStudents))
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .Select(mapper.MapEntityToListModel)
                    .ToList();
            }
        }

        public List<StudentListModel> FindListByLogin(string name)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                return aplikaciaDbContext
                    .Students
                    .Where((r => r.Login == name))
                    //SELECT *
                    //FROM Activity JOIN ScoredActivity ON Activity.Id = ScoredActivity.ActivityId;
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .Include(r => r.RegisteredActivities.Select(i => i.RegisteredStudents))
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .Select(mapper.MapEntityToListModel)
                    .ToList();
            }
        }

        public StudentDetailModel FindByid(Guid id)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var student = aplikaciaDbContext
                    .Students
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .Include(r => r.RegisteredActivities.Select(i => i.RegisteredStudents))
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .FirstOrDefault(r => r.Id == id);
                return mapper.MapEntityToDetailModel(student);
            }
        }

        public StudentDetailModel InsertStudent(StudentDetailModel student)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var entity = mapper.MapDetailModelToEntity(student);
                entity.Id = Guid.NewGuid();

                aplikaciaDbContext.Students.Add(entity);
                aplikaciaDbContext.SaveChanges();

                return mapper.MapEntityToDetailModel(entity);
            }
        }

        public void UpdateStudent(StudentDetailModel student)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var entity = aplikaciaDbContext.Students.First(r => r.Id == student.Id);

                entity.Name = student.Name;
                entity.SurName = student.SurName;
                entity.Photo = mapper.ImageSourceToByteArray(student.Photo);
                entity.Login = student.Login;

                aplikaciaDbContext.SaveChanges();

            }
        }

        public void DeleteStudentByTheId(Guid id)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var entity = new StudentEntity() { Id = id };
                aplikaciaDbContext.Students.Attach(entity);
                aplikaciaDbContext.Students.Remove(entity);
                aplikaciaDbContext.SaveChanges();
            }
        }

        public List<StudentListModel> GetAll()
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                return aplikaciaDbContext.Students
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .Include(r => r.RegisteredActivities.Select(i => i.RegisteredStudents))
                    .Include(r => r.ScoredActivities.Select(i => i.Student))
                    .Select(mapper.MapEntityToListModel)
                    .ToList();
            }
        }
    }
}
