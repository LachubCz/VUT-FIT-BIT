using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using AplikaciaDB.Entities.Base.Implemetation;

namespace AplikaciaDB.Entities
{
    public class StudentEntity : Entity
    {
        [Required]
        public string Name { get; set;}
        [Required]
        public string SurName { get; set;}
        public byte[] Photo { get; set;}
        [Required]
        public string Login { get; set; }
        public virtual ICollection<EnrollmentsEntity> Enrollments { get; set; } = new List<EnrollmentsEntity>();
        public virtual ICollection<ActivityEntity> RegisteredActivities { get; set; } = new List<ActivityEntity>();
        public virtual ICollection<ScoredActivityEntity> ScoredActivities { get; set; } = new List<ScoredActivityEntity>();
    }
}
