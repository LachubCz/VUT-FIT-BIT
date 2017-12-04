<?php

namespace App;


class question extends Model
{
	public $timestamps = false;
    public function scores()
    {
        return $this->hasMany('App\score');
    }

    public function terms()
    {
        return $this->belongsToMany('App\term');
    }
}
