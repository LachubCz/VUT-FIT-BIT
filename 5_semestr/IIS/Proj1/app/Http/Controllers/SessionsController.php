<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Http\Requests;
use App\User;
use Illuminate\Session\SessionManager;
use Illuminate\Session\Store;
use Illuminate\Support\Facades\Session;

class SessionsController extends Controller
{

    public function __construst()
    {
        $this->middleware('quest', ['except' => 'destroy']);
    }

    public function create()
    {
        return view('sessions.index');
    }

    public function store()
    {
        //Attempt to authenticate the user
        //if so sign them in
        if(!auth()->attempt(\request(['username', 'password']))) {
            return back()->withErrors('Zadané heslo a username nesouhlasí.');
        }



        $position = auth()->user()->position_type;

        if($position == 'App\teacher')
        {
            return redirect('homevyucujici');
        }
        else if($position == 'App\student')
        {
            return redirect('homestudent');
        }

        //redirect to the home page
        return redirect()->home();
    }


    public function destroy()
    {

        auth()->logout();

        return view('sessions.index');
    }

    public function ajaxCheck()
    {


        // Configuration
        $maxIdleBeforeLogout = 600 * 1;
        $maxIdleBeforeWarning = 540 * 1;
        $warningTime = $maxIdleBeforeLogout - $maxIdleBeforeWarning;

        // Calculate the number of seconds since the use's last activity
        $idleTime = date('U') - Session::get('lastActive');

        // Warn user they will be logged out if idle for too long
        if ($idleTime > $maxIdleBeforeWarning && empty(Session::get('idleWarningDisplayed'))) {

            Session::put('idleWarningDisplayed', true);

            return 'You have ' . $warningTime . ' seconds left before you are logged out';
        }

        // Log out user if idle for too long
        if ($idleTime > $maxIdleBeforeLogout && empty(Session::get('logoutWarningDisplayed'))) {

            // *** Do stuff to log out user here
            auth()->logout();

            Session::put('logoutWarningDisplayed', true);

            return 'loggedOut';
        }

        return '';
    }
}