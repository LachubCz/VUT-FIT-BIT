using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AplikaciaDB.Entities;

namespace AplikaciaModels.Models
{
    public class EnrollmentsDetailModel
    {
        public Guid Id { get; set; }
        public Guid StudentId { get; set; }
        public Guid SubjectId { get; set; }
        public virtual StudentEntity Student { get; set; }
        public virtual SubjectEntity Subject { get; set; }
        public float ObtainedPoints { get; set; }
        public GradeEntity Grade { get; set; }
    }
}
