using System;
using AplikaciaModels.Models;

namespace AplikaciaModels.Messages
{
    public class UpdatedStudentMessage
    {
        public StudentDetailModel Model { get; set; }
        public UpdatedStudentMessage(StudentDetailModel model)
        {
            Model = model;
        }
    }
}
