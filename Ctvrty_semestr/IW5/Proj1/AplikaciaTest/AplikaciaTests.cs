using AplikaciaDB;
using AplikaciaModels.Repositories;
using System.Linq;
using Xunit;

namespace Aplikacia.Tests
{
    public class AplikaciaDbContextTests
    {
        [Fact]
        public void DbConnectionTest()
        {
            using (var db = new AplikaciaDbContext())
            {
                db.Activities.Any();
            }
        }

        private ActivityRepository activityRepository = new ActivityRepository();
        
        [Fact]
        public void FindByName_Activity_Test_NotNull()
        {
            var activity = activityRepository.FindByName("Test");
            Assert.NotNull(activity);
        }

        [Fact]
        public void FindByName_Activity_Test2_Null()
        {
            var activity = activityRepository.FindByName("Test2");
            Assert.Null(activity);
        }

        private StudentRepository studentRepository = new StudentRepository();

        [Fact]
        public void FindByName_Student_Tomas_NotNull()
        {
            var student = studentRepository.FindByName("Tomas");
            Assert.NotNull(student);
        }

        [Fact]
        public void FindByName_Student_Petr_Null()
        {
            var student = studentRepository.FindByName("Petr");
            Assert.Null(student);
        }

        [Fact]
        public void FindByName_ScoredActivity_Comment()
        {
            var student = studentRepository.FindByName("Tomas");
            var hasCommentSoGood = student.ScoredActivities.Any(scoredActivity => scoredActivity.Comment == "So good");
            Assert.True(hasCommentSoGood);
        }
    }
    /*
    public class AplikaciaTests
    {
        [Fact]
        public void AddStudentsToSubject()
        {
            Subject iw5 = new Subject() { Id = 1, Credits = 5, Name = "IW5" };
            Student studentMe = new Student() { StudentId = 1, Name = "Tomas", SurName = "Rebro" };
            Enrollment enrollment = new Enrollment() { StudentId = studentMe.StudentId, Student = studentMe, SubjectId = iw5.Id, Subject = iw5, ObtainedPoints = 0, Grade = Grade.F };
            iw5.Enrollments.Add(enrollment);
            studentMe.Enrollments.Add(enrollment);
            Assert.Equal(1, iw5.Enrollments.Count);
            Assert.Same(studentMe, iw5.Enrollments.First().Student);
        }
        [Fact]
        public void AddScoredActivityToSubject()
        {
            Subject iw5 = new Subject() { Id = 1, Credits = 5, Name = "IW5" };
            Activity activity = new Activity() { ScoredActivityId = 1, Subject = iw5, SubjectId = iw5.Id, Name = "Projekt 1.cast", MaximumStudents = 20, MinPoints = 5, MaxPoints = 20, RegistrationStart = new DateTime(2017, 5, 20, 20, 0, 0), RegistrationEnd = new DateTime(2017, 5, 25, 20, 0, 0) };
            iw5.ScoredActivities.Add(activity);
            Assert.Equal(1, iw5.ScoredActivities.Count);
        }
        [Fact]
        public void AddScoreToScoredActivity()
        {
            Subject iw5 = new Subject() { Id = 1, Credits = 5, Name = "IW5" };
            Activity activity = new Activity() { ScoredActivityId = 1, Subject = iw5, SubjectId = iw5.Id, Name = "Projekt 1.cast", MaximumStudents = 20, MinPoints = 5, MaxPoints = 20, RegistrationStart = new DateTime(2017, 5, 20, 20, 0, 0), RegistrationEnd = new DateTime(2017, 5, 25, 20, 0, 0) };
            iw5.ScoredActivities.Add(activity);
            Student studentMe = new Student() { StudentId = 1, Name = "Tomas", SurName = "Rebro" };
            Enrollment enrollment = new Enrollment() { StudentId = studentMe.StudentId, Student = studentMe, SubjectId = iw5.Id, Subject = iw5, ObtainedPoints = 0 };
            iw5.Enrollments.Add(enrollment);
            studentMe.Enrollments.Add(enrollment);
            ScoredActivity scoredActivity = new ScoredActivity() { StudentId = studentMe.StudentId, Student = studentMe, ScoredActivityId = activity.ScoredActivityId, Activity = activity, ObtainedPoints = 10, Comment = "No comment" };
            studentMe.ScoredActivities.Add(scoredActivity);
            activity.ScoredActivities.Add(scoredActivity);
            Assert.Equal(10, studentMe.ScoredActivities.First().ObtainedPoints);
            Assert.Same(iw5, studentMe.ScoredActivities.First().Activity.Subject);
            Assert.Same(studentMe, iw5.ScoredActivities.First().ScoredActivities.First().Student);
        }
        [Fact]
        public void AddMultipleScoredActivities()
        {
            Subject iw5 = new Subject() { Id = 1, Credits = 5, Name = "IW5" };
            Activity activity = new Activity() { ScoredActivityId = 1, Subject = iw5, SubjectId = iw5.Id, Name = "Projekt 1.cast", MaximumStudents = 20, MinPoints = 5, MaxPoints = 20, RegistrationStart = new DateTime(2017, 5, 20, 20, 0, 0), RegistrationEnd = new DateTime(2017, 5, 25, 20, 0, 0) };
            iw5.ScoredActivities.Add(activity);
            Activity activity2 = new Activity() { ScoredActivityId = 2, Subject = iw5, SubjectId = iw5.Id, Name = "Projekt 2.cast", MaximumStudents = 20, MinPoints = 5, MaxPoints = 20, RegistrationStart = new DateTime(2017, 5, 20, 20, 0, 0), RegistrationEnd = new DateTime(2017, 5, 25, 20, 0, 0) };
            iw5.ScoredActivities.Add(activity2);
            Student studentMe = new Student() { StudentId = 1, Name = "Tomas", SurName = "Rebro" };
            Enrollment enrollment = new Enrollment() { StudentId = studentMe.StudentId, Student = studentMe, SubjectId = iw5.Id, Subject = iw5, ObtainedPoints = 0 };
            iw5.Enrollments.Add(enrollment);
            studentMe.Enrollments.Add(enrollment);
     
            ScoredActivity scoredActivity = new ScoredActivity() { StudentId = studentMe.StudentId, Student = studentMe, ScoredActivityId = activity.ScoredActivityId, Activity = activity, ObtainedPoints = 10, Comment = "No comment" };
            studentMe.ScoredActivities.Add(scoredActivity);
            activity.ScoredActivities.Add(scoredActivity);

            ScoredActivity scoredActivityInfo2 = new ScoredActivity() { StudentId = studentMe.StudentId, Student = studentMe, ScoredActivityId = activity.ScoredActivityId, Activity = activity, ObtainedPoints = 12, Comment = "No comment" };
            studentMe.ScoredActivities.Add(scoredActivityInfo2);
            activity.ScoredActivities.Add(scoredActivityInfo2);

            Assert.Equal(22, studentMe.ScoredActivities.Where(x => x.Activity.Subject.Equals(iw5)).Sum(x => x.ObtainedPoints));
            Assert.Equal(0, studentMe.ScoredActivities.Where(x => !x.Activity.Subject.Equals(iw5)).Sum(x => x.ObtainedPoints));
        }
        [Fact]
        public void ChangeSubjectName()
        {
            Subject iw4 = new Subject() { Id = 1, Credits = 5, Name = "IW5" };
            Student studentMe = new Student() { StudentId = 1, Name = "Tomas", SurName = "Rebro" };
            Enrollment enrollment = new Enrollment() { StudentId = studentMe.StudentId, Student = studentMe, SubjectId = iw4.Id, Subject = iw4, ObtainedPoints = 0, Grade = Grade.F };
            iw4.Enrollments.Add(enrollment);
            studentMe.Enrollments.Add(enrollment);
            iw4.Name = "IW4";
            Assert.Equal(iw4.Name, studentMe.Enrollments.First().Subject.Name);
        }
    }
    */
}
