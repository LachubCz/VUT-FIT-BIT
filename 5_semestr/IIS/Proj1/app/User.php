<?php

namespace App;

use Illuminate\Notifications\Notifiable;
use Illuminate\Foundation\Auth\User as Authenticatable;

class User extends Authenticatable
{
    use Notifiable;
    protected $table = 'people';
    protected $primaryKey = 'position_id';
    /**
     * The attributes that are mass assignable.
     *
     * @var array
     */
    protected $fillable = [
        'jmeno', 'prijmeni', 'rodne_cislo', 'username', 'email', 'password'
    ];

    /**
     * The attributes that should be hidden for arrays.
     *
     * @var array
     */
    protected $hidden = [
        'password', 'remember_token'
    ];


    //sem je mozne vytvaret nove metody nad databazovymi kolekcemi nebo spojovat tabulky

    public function degree()
    {
        return $this->hasOne('App\degree');
    }

    public function position()
    {
        return $this->morphTo();
    }


    public function roles()
    {
        dd($this);
    }

    public function hasAnyRole($roles)
    {
        if(is_array($roles))
        {
            foreach($roles as $role)
            {
                if($this->hasRole($role)){
                    return true;
                }
            }
        } else {
            if($this->hasRole($roles)) {
                return true;
            }
        }
        return false;
    }

    public function hasRole($role)
    {
        if($role == "Teacher") {
            if ($this->position_type == 'App\teacher') {
                return true;
            }
        }
        else if($role == "Student") {
            if ($this->position_type == 'App\student') {
                return true;
            }
        }

        return false;
    }

}
