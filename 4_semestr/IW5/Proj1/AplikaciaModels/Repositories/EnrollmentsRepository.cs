using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AplikaciaDB;
using AplikaciaDB.Entities;
using AplikaciaModels.Mappers;
using AplikaciaModels.Models;

namespace AplikaciaModels.Repositories
{
    public class EnrollmentsRepository
    {
        private EnrollmentsMapper mapper = new EnrollmentsMapper();

        public void Update(EnrollmentsDetailModel activity)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var entity = aplikaciaDbContext.Enrollments.First(r => r.Id == activity.Id);

                entity.ObtainedPoints = activity.ObtainedPoints;
                entity.Grade = activity.Grade;

                aplikaciaDbContext.SaveChanges();
            }
        }

        public EnrollmentsDetailModel Insert(EnrollmentsDetailModel activity)
        {
            using (var aplikaciaDbContext = new AplikaciaDbContext())
            {
                var entity = mapper.MapDetailModelToEntity(activity);
                entity.Id = Guid.NewGuid();

                aplikaciaDbContext.Enrollments.Add(entity);
                aplikaciaDbContext.SaveChanges();

                return mapper.MapEntityToDetailModel(entity);
            }
        }
    }
}
