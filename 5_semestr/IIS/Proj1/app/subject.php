<?php

namespace App;

class subject extends Model
{
    public function students()
    {
        return $this->belongsToMany('App\student');
    }

    public function exams()
    {
        return $this->hasMany('App\exam');
    }

    public function teachers()
    {
        return $this->belongsToMany('App\teacher');
    }

    public function garant()
    {
        return $this->belongsTo('App\teacher');
    }

    public function isTeachersSubject($index)
    {
        if ($this->garant_id == $index) {
            return true;
        }
        return false;
    }

    public function terms()
    {
        return $this->hasManyThrough('App\term', 'App\exam','subject_id', 'exam_id', 'id', 'subject_id');
    }

}
