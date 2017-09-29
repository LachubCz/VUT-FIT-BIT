using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AplikaciaModels.Models;

namespace AplikaciaModels.Messages
{
    public class AddedStudentToSubjectMessage
    {
        public StudentDetailModel Model { get; set; }
        public AddedStudentToSubjectMessage(StudentDetailModel model)
        {
            Model = model;
        }
    }
}
