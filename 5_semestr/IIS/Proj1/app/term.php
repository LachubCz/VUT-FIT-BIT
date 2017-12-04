<?php

namespace App;

class term extends Model
{
    public $timestamps = false;
    public function students()
    {
        return $this->belongsToMany('App\student');
    }

    public function questions()
    {
        return $this->belongsToMany('App\question');
    }

    public function exam()
    {
        return $this->belongsTo('App\exam');
    }
}
