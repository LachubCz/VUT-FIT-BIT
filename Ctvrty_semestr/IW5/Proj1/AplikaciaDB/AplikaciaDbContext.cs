using System.Data.Entity;
using System.Data.Entity.ModelConfiguration;
using AplikaciaDB.Entities;

namespace AplikaciaDB
{
    public class AplikaciaDbContext : DbContext
    {
        public IDbSet<StudentEntity> Students { get; set; }
        public IDbSet<SubjectEntity> Subjects { get; set; }
        public IDbSet<ActivityEntity> Activities { get; set; }
        public IDbSet<EnrollmentsEntity> Enrollments { get; set; }
        public IDbSet<ScoredActivityEntity> ScoredActivities { get; set; }

        public AplikaciaDbContext()
            : base(@"Data Source=(localdb)\MSSQLLocalDB;Initial Catalog=AplikaciaDB;Integrated Security=True;Connect Timeout=30;Encrypt=False;TrustServerCertificate=True;ApplicationIntent=ReadWrite;MultiSubnetFailover=False")
        {
        }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            Database.SetInitializer<AplikaciaDbContext>(null);

            modelBuilder.Entity<ScoredActivityEntity>().HasKey(q => q.Id);
            modelBuilder.Entity<SubjectEntity>().HasKey(q => q.Id);
            modelBuilder.Entity<StudentEntity>().HasKey(q => q.Id);
            modelBuilder.Entity<ActivityEntity>().HasKey(q => q.Id);
            modelBuilder.Entity<EnrollmentsEntity>().HasKey(q => q.Id);

            modelBuilder.Entity<StudentEntity>()
                        .HasMany(t => t.Enrollments)
                        .WithRequired(t => t.Student);

            modelBuilder.Entity<SubjectEntity>()
                        .HasMany(t => t.Enrollments)
                        .WithRequired(t => t.Subject);

            modelBuilder.Entity<StudentEntity>()
                        .HasMany(t => t.ScoredActivities)
                        .WithRequired(t => t.Student);

            modelBuilder.Entity<StudentEntity>()
                        .HasMany(t => t.RegisteredActivities)
                        .WithMany(t => t.RegisteredStudents)
                        .Map(cs =>
                            {
                                cs.MapLeftKey("StudentRefId");
                                cs.MapRightKey("ActivityRefId");
                                cs.ToTable("StudentRegisteredActivities");
                            });

            modelBuilder.Entity<SubjectEntity>()
                        .HasMany(t => t.Activities)
                        .WithRequired(t => t.Subject);

            modelBuilder.Entity<ActivityEntity>()
                        .HasMany(t => t.ScoredActivities)
                        .WithRequired(t => t.Activity);


            modelBuilder.Entity<StudentEntity>().MapToStoredProcedures();
            modelBuilder.Entity<ActivityEntity>().MapToStoredProcedures();
            modelBuilder.Entity<SubjectEntity>().MapToStoredProcedures();
            modelBuilder.Entity<EnrollmentsEntity>().MapToStoredProcedures();
            modelBuilder.Entity<ScoredActivityEntity>().MapToStoredProcedures();

            base.OnModelCreating(modelBuilder);
        }

        public void FixEfProviderServicesProblem()
        {
            var instance = System.Data.Entity.SqlServer.SqlProviderServices.Instance;
        }
    }
}
