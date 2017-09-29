using System.Data.Entity;
using System.Data.Entity.Migrations;

namespace AplikaciaDB
{
    public class AplikaciaDbMigrationsConfiguration<T> : DbMigrationsConfiguration<T>
        where T : DbContext
    {
        public AplikaciaDbMigrationsConfiguration()
        {
            AutomaticMigrationsEnabled = true;
        }
    }
}
