using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using AplikaciaDB.Entities.Base.Implemetation;

namespace AplikaciaDB.Entities
{
    public class ScoredActivityEntity : Entity
    {
        [Required, ForeignKey("Student")]
        public Guid StudentId { get; set; }
        public virtual StudentEntity Student { get; set; }
        [Required]
        public Guid ActivityId { get; set; }
        public virtual ActivityEntity Activity { get; set; }
        public float ObtainedPoints { get; set; }
        public string Comment { get; set; }
    }
}
