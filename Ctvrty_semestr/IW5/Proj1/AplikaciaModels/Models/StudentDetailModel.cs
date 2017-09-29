using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using AplikaciaDB.Entities;

namespace AplikaciaModels.Models
{
    public class StudentDetailModel
    {
        public Guid Id { get; set; }
        public string Name { get; set; }
        public string SurName { get; set; }
        public ImageSource Photo { get; set; }
        public string Login { get; set; }
        public virtual ICollection<EnrollmentsEntity> Enrollments { get; set; }
        public virtual ICollection<ScoredActivityEntity> ScoredActivities { get; set; }

    }
}
