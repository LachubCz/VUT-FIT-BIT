using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using AplikaciaDB.Entities.Base.Implemetation;

namespace AplikaciaDB.Entities
{
    public class EnrollmentsEntity : Entity
    {
        [Required, ForeignKey("Student")]
        public Guid StudentId { get; set; }
        [Required, ForeignKey("Subject")]
        public Guid SubjectId { get; set; }
        public virtual StudentEntity Student { get; set; }
        public virtual SubjectEntity Subject { get; set; }
        public float ObtainedPoints { get; set; }
        public GradeEntity Grade { get; set; }
    }
}
