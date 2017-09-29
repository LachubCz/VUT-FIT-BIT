using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AplikaciaDB.Entities;
using AplikaciaModels.Models;

namespace AplikaciaModels.Mappers
{
    public class EnrollmentsMapper
    {

        public EnrollmentsEntity MapDetailModelToEntity(EnrollmentsDetailModel entity)
        {
            return new EnrollmentsEntity()
            {
                Id = entity.Id,
                StudentId = entity.StudentId,
                SubjectId = entity.SubjectId,
                Student = entity.Student,
                Subject = entity.Subject,
                ObtainedPoints = entity.ObtainedPoints,
                Grade = entity.Grade
            };
        }

        public EnrollmentsDetailModel MapEntityToDetailModel(EnrollmentsEntity entity)
        {
            return new EnrollmentsDetailModel()
            { 
                Id = entity.Id,
                StudentId = entity.StudentId,
                SubjectId = entity.SubjectId,
                Student = entity.Student,
                Subject = entity.Subject,
                ObtainedPoints = entity.ObtainedPoints,
                Grade = entity.Grade
            };

        }
    }
}
