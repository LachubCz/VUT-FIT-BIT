using System.IO;
using System.Runtime.InteropServices.ComTypes;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using AplikaciaModels.Models;
using AplikaciaDB.Entities;

namespace AplikaciaModels.Mappers
{
    public class StudentMapper
    {
        public byte[] ImageSourceToByteArray(ImageSource image)
        {
            if (image == null)
            {
                return null;
            }

            var bitmapSource = image as BitmapSource;
            byte[] photo = null;

            if (bitmapSource == null) return photo;

            JpegBitmapEncoder encoder = new JpegBitmapEncoder();
            encoder.Frames.Add(BitmapFrame.Create(bitmapSource));
            using (var ms = new MemoryStream())
            {
                encoder.Save(ms);
                photo = ms.ToArray();
            }

            return photo;
        }

        public ImageSource ByteArrayToImageSource(byte[] photo)
        {
            if (photo == null)
            {
                return null;
            }
            using (var stream = new MemoryStream(photo))
            {
                stream.Seek(0, SeekOrigin.Begin);
                var image = new BitmapImage();
                image.BeginInit();
                image.StreamSource = stream;
                image.EndInit();

                ImageSource imgsrc = image as ImageSource;

                return imgsrc;
            }
        }

        public StudentDetailModel MapEntityToDetailModel(StudentEntity entity)
        {
            return new StudentDetailModel()
            {
                Id = entity.Id,
                Name = entity.Name,
                SurName = entity.SurName,
                Login = entity.Login,
                Photo = ByteArrayToImageSource(entity.Photo),
                Enrollments = entity.Enrollments,
                ScoredActivities = entity.ScoredActivities
            };
        }

        public StudentEntity MapDetailModelToEntity(StudentDetailModel entity)
        {
            return new StudentEntity()
            {
                Id = entity.Id,
                Name = entity.Name,
                SurName = entity.SurName,
                Login = entity.Login,
                Photo = ImageSourceToByteArray(entity.Photo),
                Enrollments = entity.Enrollments,
                ScoredActivities = entity.ScoredActivities
            };
        }

        public StudentListModel MapEntityToListModel(StudentEntity entity)
        {
            return new StudentListModel()
            {
                Id = entity.Id,
                Name = entity.Name,
                SurName = entity.SurName,
                Login = entity.Login
            };
        }
    }
}
