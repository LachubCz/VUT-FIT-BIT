namespace AplikaciaDB.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class firstmigration : DbMigration
    {
        public override void Up()
        {
            RenameTable(name: "dbo.Activities", newName: "ActivityEntities");
            RenameTable(name: "dbo.Students", newName: "StudentEntities");
            RenameTable(name: "dbo.Enrollments", newName: "EnrollmentsEntities");
            RenameTable(name: "dbo.Subjects", newName: "SubjectEntities");
            RenameTable(name: "dbo.ScoredActivities", newName: "ScoredActivityEntities");
            CreateStoredProcedure(
                "dbo.ActivityEntity_Insert",
                p => new
                    {
                        Id = p.Guid(),
                        SubjectId = p.Guid(),
                        Name = p.String(maxLength: 50),
                        MaximumStudents = p.Int(),
                        MinPoints = p.Single(),
                        MaxPoints = p.Single(),
                        RegistrationStart = p.DateTime(),
                        RegistrationEnd = p.DateTime(),
                    },
                body:
                    @"INSERT [dbo].[ActivityEntities]([Id], [SubjectId], [Name], [MaximumStudents], [MinPoints], [MaxPoints], [RegistrationStart], [RegistrationEnd])
                      VALUES (@Id, @SubjectId, @Name, @MaximumStudents, @MinPoints, @MaxPoints, @RegistrationStart, @RegistrationEnd)"
            );
            
            CreateStoredProcedure(
                "dbo.ActivityEntity_Update",
                p => new
                    {
                        Id = p.Guid(),
                        SubjectId = p.Guid(),
                        Name = p.String(maxLength: 50),
                        MaximumStudents = p.Int(),
                        MinPoints = p.Single(),
                        MaxPoints = p.Single(),
                        RegistrationStart = p.DateTime(),
                        RegistrationEnd = p.DateTime(),
                    },
                body:
                    @"UPDATE [dbo].[ActivityEntities]
                      SET [SubjectId] = @SubjectId, [Name] = @Name, [MaximumStudents] = @MaximumStudents, [MinPoints] = @MinPoints, [MaxPoints] = @MaxPoints, [RegistrationStart] = @RegistrationStart, [RegistrationEnd] = @RegistrationEnd
                      WHERE ([Id] = @Id)"
            );
            
            CreateStoredProcedure(
                "dbo.ActivityEntity_Delete",
                p => new
                    {
                        Id = p.Guid(),
                    },
                body:
                    @"DELETE [dbo].[ActivityEntities]
                      WHERE ([Id] = @Id)"
            );
            
            CreateStoredProcedure(
                "dbo.StudentEntity_Insert",
                p => new
                    {
                        Id = p.Guid(),
                        Name = p.String(),
                        SurName = p.String(),
                        Photo = p.Binary(),
                        Login = p.String(),
                    },
                body:
                    @"INSERT [dbo].[StudentEntities]([Id], [Name], [SurName], [Photo], [Login])
                      VALUES (@Id, @Name, @SurName, @Photo, @Login)"
            );
            
            CreateStoredProcedure(
                "dbo.StudentEntity_Update",
                p => new
                    {
                        Id = p.Guid(),
                        Name = p.String(),
                        SurName = p.String(),
                        Photo = p.Binary(),
                        Login = p.String(),
                    },
                body:
                    @"UPDATE [dbo].[StudentEntities]
                      SET [Name] = @Name, [SurName] = @SurName, [Photo] = @Photo, [Login] = @Login
                      WHERE ([Id] = @Id)"
            );
            
            CreateStoredProcedure(
                "dbo.StudentEntity_Delete",
                p => new
                    {
                        Id = p.Guid(),
                    },
                body:
                    @"DELETE [dbo].[StudentEntities]
                      WHERE ([Id] = @Id)"
            );
            
            CreateStoredProcedure(
                "dbo.EnrollmentsEntity_Insert",
                p => new
                    {
                        Id = p.Guid(),
                        StudentId = p.Guid(),
                        SubjectId = p.Guid(),
                        ObtainedPoints = p.Single(),
                        Grade = p.Int(),
                    },
                body:
                    @"INSERT [dbo].[EnrollmentsEntities]([Id], [StudentId], [SubjectId], [ObtainedPoints], [Grade])
                      VALUES (@Id, @StudentId, @SubjectId, @ObtainedPoints, @Grade)"
            );
            
            CreateStoredProcedure(
                "dbo.EnrollmentsEntity_Update",
                p => new
                    {
                        Id = p.Guid(),
                        StudentId = p.Guid(),
                        SubjectId = p.Guid(),
                        ObtainedPoints = p.Single(),
                        Grade = p.Int(),
                    },
                body:
                    @"UPDATE [dbo].[EnrollmentsEntities]
                      SET [StudentId] = @StudentId, [SubjectId] = @SubjectId, [ObtainedPoints] = @ObtainedPoints, [Grade] = @Grade
                      WHERE ([Id] = @Id)"
            );
            
            CreateStoredProcedure(
                "dbo.EnrollmentsEntity_Delete",
                p => new
                    {
                        Id = p.Guid(),
                    },
                body:
                    @"DELETE [dbo].[EnrollmentsEntities]
                      WHERE ([Id] = @Id)"
            );
            
            CreateStoredProcedure(
                "dbo.SubjectEntity_Insert",
                p => new
                    {
                        Id = p.Guid(),
                        Credits = p.Int(),
                        Name = p.String(),
                    },
                body:
                    @"INSERT [dbo].[SubjectEntities]([Id], [Credits], [Name])
                      VALUES (@Id, @Credits, @Name)"
            );
            
            CreateStoredProcedure(
                "dbo.SubjectEntity_Update",
                p => new
                    {
                        Id = p.Guid(),
                        Credits = p.Int(),
                        Name = p.String(),
                    },
                body:
                    @"UPDATE [dbo].[SubjectEntities]
                      SET [Credits] = @Credits, [Name] = @Name
                      WHERE ([Id] = @Id)"
            );
            
            CreateStoredProcedure(
                "dbo.SubjectEntity_Delete",
                p => new
                    {
                        Id = p.Guid(),
                    },
                body:
                    @"DELETE [dbo].[SubjectEntities]
                      WHERE ([Id] = @Id)"
            );
            
            CreateStoredProcedure(
                "dbo.ScoredActivityEntity_Insert",
                p => new
                    {
                        Id = p.Guid(),
                        StudentId = p.Guid(),
                        ActivityId = p.Guid(),
                        ObtainedPoints = p.Single(),
                        Comment = p.String(),
                    },
                body:
                    @"INSERT [dbo].[ScoredActivityEntities]([Id], [StudentId], [ActivityId], [ObtainedPoints], [Comment])
                      VALUES (@Id, @StudentId, @ActivityId, @ObtainedPoints, @Comment)"
            );
            
            CreateStoredProcedure(
                "dbo.ScoredActivityEntity_Update",
                p => new
                    {
                        Id = p.Guid(),
                        StudentId = p.Guid(),
                        ActivityId = p.Guid(),
                        ObtainedPoints = p.Single(),
                        Comment = p.String(),
                    },
                body:
                    @"UPDATE [dbo].[ScoredActivityEntities]
                      SET [StudentId] = @StudentId, [ActivityId] = @ActivityId, [ObtainedPoints] = @ObtainedPoints, [Comment] = @Comment
                      WHERE ([Id] = @Id)"
            );
            
            CreateStoredProcedure(
                "dbo.ScoredActivityEntity_Delete",
                p => new
                    {
                        Id = p.Guid(),
                    },
                body:
                    @"DELETE [dbo].[ScoredActivityEntities]
                      WHERE ([Id] = @Id)"
            );
            
            DropStoredProcedure("dbo.Activity_Insert");
            DropStoredProcedure("dbo.Activity_Update");
            DropStoredProcedure("dbo.Activity_Delete");
            DropStoredProcedure("dbo.Student_Insert");
            DropStoredProcedure("dbo.Student_Update");
            DropStoredProcedure("dbo.Student_Delete");
            DropStoredProcedure("dbo.Enrollment_Insert");
            DropStoredProcedure("dbo.Enrollment_Update");
            DropStoredProcedure("dbo.Enrollment_Delete");
            DropStoredProcedure("dbo.Subject_Insert");
            DropStoredProcedure("dbo.Subject_Update");
            DropStoredProcedure("dbo.Subject_Delete");
            DropStoredProcedure("dbo.ScoredActivity_Insert");
            DropStoredProcedure("dbo.ScoredActivity_Update");
            DropStoredProcedure("dbo.ScoredActivity_Delete");
        }
        
        public override void Down()
        {
            DropStoredProcedure("dbo.ScoredActivityEntity_Delete");
            DropStoredProcedure("dbo.ScoredActivityEntity_Update");
            DropStoredProcedure("dbo.ScoredActivityEntity_Insert");
            DropStoredProcedure("dbo.SubjectEntity_Delete");
            DropStoredProcedure("dbo.SubjectEntity_Update");
            DropStoredProcedure("dbo.SubjectEntity_Insert");
            DropStoredProcedure("dbo.EnrollmentsEntity_Delete");
            DropStoredProcedure("dbo.EnrollmentsEntity_Update");
            DropStoredProcedure("dbo.EnrollmentsEntity_Insert");
            DropStoredProcedure("dbo.StudentEntity_Delete");
            DropStoredProcedure("dbo.StudentEntity_Update");
            DropStoredProcedure("dbo.StudentEntity_Insert");
            DropStoredProcedure("dbo.ActivityEntity_Delete");
            DropStoredProcedure("dbo.ActivityEntity_Update");
            DropStoredProcedure("dbo.ActivityEntity_Insert");
            RenameTable(name: "dbo.ScoredActivityEntities", newName: "ScoredActivities");
            RenameTable(name: "dbo.SubjectEntities", newName: "Subjects");
            RenameTable(name: "dbo.EnrollmentsEntities", newName: "Enrollments");
            RenameTable(name: "dbo.StudentEntities", newName: "Students");
            RenameTable(name: "dbo.ActivityEntities", newName: "Activities");
            throw new NotSupportedException("Scaffolding create or alter procedure operations is not supported in down methods.");
        }
    }
}
