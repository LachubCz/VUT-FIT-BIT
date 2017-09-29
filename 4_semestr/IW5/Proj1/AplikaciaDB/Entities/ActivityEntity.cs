using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using AplikaciaDB.Entities.Base.Implemetation;

namespace AplikaciaDB.Entities
{
    public class ActivityEntity : Entity
    {
        [Required, ForeignKey("Subject")]
        public Guid SubjectId { get; set; }
        public virtual SubjectEntity Subject { get; set; }
        [Required, MaxLength(50), StringLength(50)]
        public string Name { get; set; }
        public int MaximumStudents { get; set; }
        public float MinPoints { get; set; }
        public float MaxPoints { get; set; }
        [Required]
        public DateTime RegistrationStart { get; set; }
        public DateTime RegistrationEnd { get; set; }
        public virtual ICollection<ScoredActivityEntity> ScoredActivities { get; set; } = new HashSet<ScoredActivityEntity>();
        public virtual ICollection<StudentEntity> RegisteredStudents { get; set; } = new HashSet<StudentEntity>();
    }
}
