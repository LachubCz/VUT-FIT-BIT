using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AplikaciaModels.Messages
{
    public class DeletedStudentMessage
    {
        public DeletedStudentMessage(Guid studentId)
        {
            StudentId = studentId;
        }

        public Guid StudentId { get; set; }
    }
}