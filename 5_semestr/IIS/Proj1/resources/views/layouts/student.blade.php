<!DOCTYPE html>
<html lang="{{ app()->getLocale() }}">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <!-- CSRF Token -->
    <meta name="csrf-token" content="{{ csrf_token() }}">

    <title>Skvělý informační systém</title>

    <!-- Styles -->
    <link href="{{ asset('css/app.css') }}" rel="stylesheet">

    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
</head>
<body id="body">
<div id="app">
    <script>
        function checkSession() {
            $.post('{{ route('session.ajax.check') }}', { '_token' : '{!! csrf_token() !!}' }, function(data) {
                if (data == 'loggedOut') {
                    // User was logged out. Redirect to login page
                    document.location.href = '{!! route('login22') !!}';
                }
                else if (data != '') {
                    // User will be logged out soon.
                    alert("You will be logged out in 60 seconds");
                }
            });
        }
        setInterval(checkSession, 1000);
    </script>
    <nav class="navbar navbar-inverse">
        <div class="container">
                <ul class="nav navbar-nav">
                    <li id="actual"><a id="actual" href="{{ url('homestudent') }}">Home</a></li>
                    <li><a href="{{ url('terminystudent') }}">Termíny</a></li>
                    <li><a href="{{ url('predmetystudent') }}">Předměty</a></li>
                    <li><a href="{{ url('hodnocenistudent') }}">Hodnocení</a></li>
                </ul>
                <!-- Right Side Of Navbar -->
                <ul class="nav navbar-nav navbar-right">
                    <!-- Authentication Links -->
                    @if(Auth::guest())
                        <li><a href="/login">Login</a></li>
                        <li><a href="/register">Register</a></li>
                    @else
                        <li><a href="/logout">Logout</a></li>
                        <li   style="margin-top: 14.1px;"  >{{ Auth::user()->jmeno }} {{ Auth::user()->prijmeni }}</li>
                     @endif
                </ul>
            </div>
        </div>
    </nav>
</div>
    @include('inc.messages')
    @yield('content')
</body>
</html>