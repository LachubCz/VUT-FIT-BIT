using System;
using AplikaciaDB.Entities.Base.Interface;
using NHibernate;

namespace AplikaciaDB.Entities.Base.Implemetation
{
    public abstract class Entity : IEntity
    {
        public Guid Id { get; set; } = Guid.NewGuid();

        public override bool Equals(object obj)
        {
            var compareTo = obj as Entity;

            if (ReferenceEquals(compareTo, null))
                return false;

            if (ReferenceEquals(this, compareTo))
                return true;

            if (GetRealType() != compareTo.GetRealType())
                return false;

            if (!IsTransient() && !compareTo.IsTransient() && Id == compareTo.Id)
                return true;

            return false;
        }

        public static bool operator ==(Entity a, Entity b)
        {
            if (ReferenceEquals(a, null) && ReferenceEquals(b, null))
                return true;

            if (ReferenceEquals(a, null) || ReferenceEquals(b, null))
                return false;

            return a.Equals(b);
        }

        public static bool operator !=(Entity a, Entity b)
        {
            return !(a == b);
        }
        
        public override int GetHashCode()
        {
            return (GetRealType().ToString() + Id).GetHashCode();
        }
        
        public virtual bool IsTransient()
        {
            return false;
        }

        public virtual Type GetRealType()
        {
            return NHibernateUtil.GetClass(this);
        } 
        
    }
}
