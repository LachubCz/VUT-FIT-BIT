using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AplikaciaDB.Entities;
using AplikaciaModels.Models;

namespace AplikaciaModels.Mappers
{
    public class ActivityMapper
    {
        public ActivityDetailModel MapEntityToDetailModel(ActivityEntity entity)
        {
            return new ActivityDetailModel()
            {
                Id = entity.Id,
                SubjectId = entity.SubjectId,
                Subject = entity.Subject,
                Name = entity.Name,
                MaximumStudents = entity.MaximumStudents,
                MinPoints = entity.MinPoints,
                MaxPoints = entity.MaxPoints,
                RegistrationStart = entity.RegistrationStart,
                RegistrationEnd = entity.RegistrationEnd,
                ScoredActivities = entity.ScoredActivities,
                RegisteredStudents = entity.RegisteredStudents
            };
        }

        public ActivityEntity MapDetailModelToEntity(ActivityDetailModel entity)
        {
            return new ActivityEntity()
            {
                Id = entity.Id,
                SubjectId = entity.SubjectId,
                Subject = entity.Subject,
                Name = entity.Name,
                MaximumStudents = entity.MaximumStudents,
                MinPoints = entity.MinPoints,
                MaxPoints = entity.MaxPoints,
                RegistrationStart = entity.RegistrationStart,
                RegistrationEnd = entity.RegistrationEnd,
                ScoredActivities = entity.ScoredActivities,
                RegisteredStudents = entity.RegisteredStudents
            };
        }

        public ActivityListModel MapEntityToListModel(ActivityEntity entity)
        {
            return new ActivityListModel()
            {
                Id = entity.Id,
                SubjectId = entity.SubjectId,
                Subject = entity.Subject,
                Name = entity.Name,
                MaximumStudents = entity.MaximumStudents,
                MinPoints = entity.MinPoints,
                MaxPoints = entity.MaxPoints,
                RegistrationStart = entity.RegistrationStart,
                RegistrationEnd = entity.RegistrationEnd
            };
        }
    }
}
