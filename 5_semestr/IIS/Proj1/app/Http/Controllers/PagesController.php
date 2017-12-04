<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Auth;
use Redirect;

class PagesController extends Controller
{

	public function people()
    {
        return view('student.test');
    }

}
