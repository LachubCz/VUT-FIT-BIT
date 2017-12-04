<?php

namespace App;


class degree extends Model
{
    public function user()
    {
        return $this->belongsTo('App\User');
    }
}