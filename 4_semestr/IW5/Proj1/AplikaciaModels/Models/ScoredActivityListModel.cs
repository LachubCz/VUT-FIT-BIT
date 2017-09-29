using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AplikaciaDB.Entities;

namespace AplikaciaModels.Models
{
    public class ScoredActivityListModel
    {
        public Guid Id { get; set; }
        public Guid StudentId { get; set; }
        public virtual StudentEntity Student { get; set; }
        public virtual ActivityEntity Activity { get; set; }
        public float ObtainedPoints { get; set; }
        public string Comment { get; set; }

    }
}
