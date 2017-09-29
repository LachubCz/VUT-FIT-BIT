using AplikaciaModels.Mappers;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AplikaciaDB;
using AplikaciaDB.Entities;
using AplikaciaModels.Models;

namespace AplikaciaModels.Repositories
{
    public class ActivityRepository
    {
        private ActivityMapper mapper = new ActivityMapper();

        /// <summary>
        /// Method used for tests, searches Activities by their name
        /// </summary>
        /// <param name="name"></param>
        /// <returns>First activity with the given name</returns>
        public ActivityEntity FindByName(string name)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var activity = aplikaciaDbContext
                    .Activities
                    //SELECT *
                    //FROM Activity JOIN ScoredActivity ON Activity.Id = ScoredActivity.ActivityId;
                    .Include(r => r.ScoredActivities.Select(i => i.Activity))
                    .Include(r => r.RegisteredStudents.Select(i => i.RegisteredActivities))
                    .FirstOrDefault(r => r.Name == name);
                return activity;
            }
        }

        //Methods used in models

        public ActivityDetailModel GetByid(Guid id)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var activity = aplikaciaDbContext
                    .Activities
                    .Include(r => r.ScoredActivities.Select(i => i.Activity))
                    .Include(r => r.RegisteredStudents.Select(i => i.RegisteredActivities))
                    .FirstOrDefault(r => r.Id == id);
                return mapper.MapEntityToDetailModel(activity);
            }
        }

        public ActivityDetailModel Insert(ActivityDetailModel activity)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var entity = mapper.MapDetailModelToEntity(activity);
                entity.Id = Guid.NewGuid();

                aplikaciaDbContext.Activities.Add(entity);
                aplikaciaDbContext.SaveChanges();

                return mapper.MapEntityToDetailModel(entity);
            }
        }

        public void Update(ActivityDetailModel activity)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var entity = aplikaciaDbContext.Activities.First(r => r.Id == activity.Id);

                entity.Name = activity.Name;
                entity.MaximumStudents = activity.MaximumStudents;
                entity.MinPoints = activity.MinPoints;
                entity.MaxPoints = activity.MaxPoints;
                entity.RegistrationStart = activity.RegistrationStart;
                entity.RegistrationEnd = activity.RegistrationEnd;

                aplikaciaDbContext.SaveChanges();

            }
        }

        public void Delete(Guid id)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var entity = new ActivityEntity() { Id = id };
                aplikaciaDbContext.Activities.Attach(entity);
                aplikaciaDbContext.Activities.Remove(entity);
                aplikaciaDbContext.SaveChanges();
            }
        }

        public List<ActivityListModel> GetAll(string subjectname)
        {
            using (var aplikaciaDBContext = new AplikaciaDbContext())
            {
                return aplikaciaDBContext.Activities
                    .Where(s => s.Subject.Name == subjectname)
                    .Select(mapper.MapEntityToListModel)
                    .ToList();
            }
        }

        public List<ActivityListModel> OrderByRegistrationStart(string subjectname)
        {
            using (var aplikaciaDBContext = new AplikaciaDbContext())
            {
                return aplikaciaDBContext.Activities
                    .Where(s => s.Subject.Name == subjectname)
                    .OrderBy(s => s.RegistrationStart)
                    .Select(mapper.MapEntityToListModel)
                    .ToList();
            }
        }

        public List<ActivityListModel> OrderByRegistrationStartDesc(string subjectname)
        {
            using (var aplikaciaDBContext = new AplikaciaDbContext())
            {
                return aplikaciaDBContext.Activities
                    .Where(s => s.Subject.Name == subjectname)
                    .OrderByDescending(s => s.RegistrationStart)
                    .Select(mapper.MapEntityToListModel)
                    .ToList();
            }
        }

        public List<ActivityListModel> OrderByRegistrationEnd(string subjectname)
        {
            using (var aplikaciaDBContext = new AplikaciaDbContext())
            {
                return aplikaciaDBContext.Activities
                    .Where(s => s.Subject.Name == subjectname)
                    .OrderBy(s => s.RegistrationEnd)
                    .Select(mapper.MapEntityToListModel)
                    .ToList();
            }
        }

        public List<ActivityListModel> OrderByRegistrationEndDesc(string subjectname)
        {
            using (var aplikaciaDBContext = new AplikaciaDbContext())
            {
                return aplikaciaDBContext.Activities
                    .Where(s => s.Subject.Name == subjectname)
                    .OrderByDescending(s => s.RegistrationEnd)
                    .Select(mapper.MapEntityToListModel)
                    .ToList();
            }
        }

    }
}