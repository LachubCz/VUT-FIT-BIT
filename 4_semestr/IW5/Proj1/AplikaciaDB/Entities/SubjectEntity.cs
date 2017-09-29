using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using AplikaciaDB.Entities.Base.Implemetation;

namespace AplikaciaDB.Entities
{
     
    public class SubjectEntity : Entity
    {
        [Required]
        public int Credits { get; set; }
        [Required]
        public string Name { get; set; }
        public virtual ICollection<ActivityEntity> Activities { get; set; } = new HashSet<ActivityEntity>();
        public virtual ICollection<EnrollmentsEntity> Enrollments { get; set; } = new HashSet<EnrollmentsEntity>();
    }
}
