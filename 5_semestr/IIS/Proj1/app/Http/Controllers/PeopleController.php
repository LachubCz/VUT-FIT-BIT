<?php

namespace App\Http\Controllers;

use App\person;

class PeopleController extends Controller
{
//    public function __construct()
//    {
//        $this->middleware('guest', ['except' => 'store', 'create']);
//    }

    public function home()
    {
        if(auth()->guest()) {
            return view('sessions.index');
        }
        else if(auth()->user()) {
            if(auth()->user()->position_type == 'App\teacher') {
                return view('teacher.homevyucujici');
            }
            else if(auth()->user()->position_type == 'App\student') {
                return view('student.homestudent');
            }
            else {
                return view('sessions.index');
            }
        }
    }

    public function homeperson()
    {
        return view('person.home');
    }

    public function people()
    {
        $people = person::paginate(5);

        return view('person.people', ['people' => $people]);
    }

    public function show($id)
    {
        $people = person::where('position_id', '=', $id);

        //return view('teacher.personShow', compact('people'));
        return view('person.show')->with('people', $people);
    }

    public function create()
    {
        return view('person.create');
    }

    public function store()
    {
        //Validate data
        $this->validate(request(), [

            //validation needs
            'jmeno' => 'required',
            'prijmeni' => 'required'

        ]);

        // Create a new person using the request data
        // Save it to the database
        person::create(request(['jmeno', 'prijmeni', 'datum_narozeni', 'rodne_cislo', 'sessions', 'email']));
        /* $person = new person;
         * $person->jmeno = $request->input('jmeno');
         * $person->prijmeni = $request->input('body');
         * $post->save();
         */


        // and then redirect to the people page and send a message with success (inc/messsages.blade.php session)
        return redirect('/people')->with('success', 'Person Created');
    }

    public function destroyed()
    {
        if(auth()->guest()) {
            return view('sessions.index');
        }
        else if(auth()->user()) {
            if(auth()->user()->position_type == 'App\teacher') {
                return view('teacher.homevyucujici');
            }
            else if(auth()->user()->position_type == 'App\student') {
                return view('student.homestudent');
            }
            else {
                return view('sessions.index');
            }
        }
    }
}
