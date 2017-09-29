using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AplikaciaDB.Entities;

namespace AplikaciaModels.Models
{
    public class SubjectDetailModel
    {
        public Guid Id { get; set; }
        public int Credits { get; set; }
        public string Name { get; set; }
        public virtual ICollection<ActivityEntity> Activities { get; set; }
        public virtual ICollection<EnrollmentsEntity> Enrollments { get; set; }

    }
}
