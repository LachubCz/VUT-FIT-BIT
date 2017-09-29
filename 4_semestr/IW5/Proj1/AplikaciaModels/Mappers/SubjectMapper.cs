using AplikaciaModels.Models;
using AplikaciaDB.Entities;

namespace AplikaciaModels.Mappers
{
    public class SubjectMapper
    {
        public SubjectListModel MapEntityToListModel(SubjectEntity entity)
        {
            return new SubjectListModel()
            {
                Id = entity.Id,
                Name = entity.Name,
                Credits = entity.Credits
            };
        }

        public SubjectDetailModel MapEntityToDetailModel(SubjectEntity entity)
        {
            return new SubjectDetailModel()
            {
                Id = entity.Id,
                Name = entity.Name,
                Credits = entity.Credits,
                Activities = entity.Activities,
                Enrollments = entity.Enrollments
            };
        }
    }
}
