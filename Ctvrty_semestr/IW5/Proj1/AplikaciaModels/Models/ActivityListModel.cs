using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AplikaciaDB.Entities;

namespace AplikaciaModels.Models
{
    public class ActivityListModel
    {
        public Guid Id { get; set; }
        public Guid SubjectId { get; set; }
        public SubjectEntity Subject { get; set; }
        public string Name { get; set; }
        public int MaximumStudents { get; set; }
        public float MinPoints { get; set; }
        public float MaxPoints { get; set; }
        public DateTime RegistrationStart { get; set; }
        public DateTime RegistrationEnd { get; set; }


    }
}
