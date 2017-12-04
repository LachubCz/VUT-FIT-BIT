<?php

namespace App;

use User;

class teacher extends Model
{
    public function subjects()
    {
        return $this->belongsToMany('App\subject');
    }

    public function garant()
    {
        return $this->hasMany('App\subject', 'garant_id');
    }

    public function scored()
    {
        return $this->hasMany('App\score');
    }

    public function person()
    {
        return $this->morphOne('App\User', 'position');
    }

    public function isGarant()
    {
        if ($this->garant->Count() != 0) {
            return true;
        }

        return false;
    }

    public function isGarantIn($subject)
    {
        foreach ($this->garant as $value) 
        {
            if ($value->zkratka == $subject)
            {
                return true;
            }
        }
        return false;
    }
}
