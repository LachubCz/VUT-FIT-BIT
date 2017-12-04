<?php

namespace App;


class score extends Model
{
    public $timestamps = false;
    public function question()
    {
        return $this->belongsTo('App\question');
    }

    public function scorer()
    {
        return $this->belongsTo('App\teacher');
    }

    public function student() {
        return $this->belongsTo('App\student');
    }

}
