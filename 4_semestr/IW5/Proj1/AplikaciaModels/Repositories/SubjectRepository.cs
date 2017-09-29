using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AplikaciaDB;
using AplikaciaModels.Mappers;
using AplikaciaDB.Entities;
using AplikaciaModels.Models;

namespace AplikaciaModels.Repositories
{
    public class SubjectRepository
    {
        private SubjectMapper mapper = new SubjectMapper();

        public List<SubjectListModel> GetAll()
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                return aplikaciaDbContext.Subjects
                    .Select(mapper.MapEntityToListModel)
                    .ToList();
            }
        }

        public void AddStudentToSubject(Guid studentId, Guid subjectId)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var entity = new EnrollmentsEntity() { Id = Guid.NewGuid(), StudentId = studentId, SubjectId = subjectId };
                aplikaciaDbContext.Enrollments.Add(entity);
                aplikaciaDbContext.SaveChanges();
            }
        }

        public SubjectDetailModel GetById(Guid id)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var subject = aplikaciaDbContext
                    .Subjects
                    .Include(r => r.Activities.Select(i => i.Subject))
                    .Include(r => r.Enrollments.Select(i => i.Student))
                    .FirstOrDefault(r => r.Id == id);
                return mapper.MapEntityToDetailModel(subject);
            }
        }
    }
}
