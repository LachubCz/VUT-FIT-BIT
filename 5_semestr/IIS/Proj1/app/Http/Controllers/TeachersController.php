<?php

namespace App\Http\Controllers;

use App\subject;
use Illuminate\Http\Request;

class TeachersController extends Controller
{
    public function __construct()
    {
        $this->middleware('auth');//->except(['something', 'something_else']);
    }

    //vyucujici
    public function homevyucujici()
    {
        $studentdata = array(
            'jmeno' => 'Tomáš',
            'prijmeni' => 'Holík',
            'sessions' => 'xholik13'
        );
        return view('teacher.homevyucujici')->with($studentdata);
    }

    public function predmetyvyucujici()
    {
        $subjects = subject::All();

        return view('teacher.predmetyvyucujici', ['subjects' => $subjects]);
    }

    public function hodnocenivyucujici()
    {
        //$subjects = subject::All();

        return view('teacher.hodnocenivyucujici');//;, ['subjects' => $subjects]);
    }

    public function zkouskyvyucujici()
    {
        $studentdata = array(
            'jmeno' => 'Tomáš',
            'prijmeni' => 'Holík',
            'sessions' => 'xholik13'
        );
        return view('teacher.zkouskyvyucujici')->with($studentdata);
    }
}
