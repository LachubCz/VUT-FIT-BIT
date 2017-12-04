<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;

use App\User;
use App\degree;

class RegistrationController extends Controller
{
    //
    public function create()
    {
        return view('register.create');
    }


    public function store()
    {
        //Validate the form
        $this->validate(request(), [

            'username' => 'required|max:255',

            'jmeno' => 'required|max:255',

            'prijmeni' => 'required|max:255',

            'email' => 'required|email|unique:people|max:255',

            'password' => 'required|confirmed|max:255|min:6'

        ]);


        //Create and save the user
        $person = User::Create([

            'username' => request('username'),

            'jmeno' => request('jmeno'),

            'prijmeni' => request('prijmeni'),

            'email' => request('email'),

            'password' => bcrypt(request('password')),

        ]);

        //Sign them in
        auth()->login($person);

        //Create and save the degree for user
        $degree = new degree;

        $degree->user_position_id = auth()->user()->position_id;

        $degree->save();

        //Redirect to the user home page
        return view('person.home');
    }
}
