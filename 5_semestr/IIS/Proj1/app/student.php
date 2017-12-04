<?php

namespace App;

class student extends Model
{

    public function terms()
    {
        return $this->belongsToMany('App\term');
    }

    public function subjects()
    {
        return $this->belongsToMany('App\subject');
    }

    public function person()
    {
        return $this->morphOne('App\User', 'position');
    }

    public function scores() {
        return $this->hasMany('App\score');
    }
}
