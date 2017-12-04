<?php

namespace App;

use Illuminate\Database\Eloquent\Model;

class exam extends Model
{
    public $timestamps = false;
    public function terms()
    {
        return $this->hasMany('App\term');
    }

    public function subject()
    {
        return $this->belongsTo('App\subject');
    }
}
