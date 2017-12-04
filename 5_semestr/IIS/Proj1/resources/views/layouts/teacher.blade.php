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

    <script src="https://code.jquery.com/jquery-3.2.1.min.js" integrity="sha256-hwg4gsxgFZhOsEEamdOYGBf13FyQuiTwlAQgxVSNgt4=" crossorigin="anonymous"></script>
    <!-- Bootstrap Javascript -->
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js" integrity="sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa" crossorigin="anonymous"></script>
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
                    alert("Budete odhlášeni za 60 vteřin.");
                }
            });
        }
        setInterval(checkSession, 1000);
    </script>

    <script>
        function ConfirmDelete()
        {
            var x = confirm("Jste si jisti, že chcete danou položku smazat?");
            if (x)
                return true;
            else
                return false;
        }
    </script>

    <nav class="navbar navbar-inverse">
        <div class="container">
                <!-- Left Side Of Navbar -->
                <ul class="nav navbar-nav">
                    <li id="actual"><a id="actual" href="{{ url('homevyucujici') }}">Home</a></li>
                    <li><a href="{{ url('predmetyvyucujici') }}">Předměty</a></li>
                    <li><a href="{{ url('zkouskyvyucujici') }}">Zkoušky</a></li>
                    <li><a href="{{ url('hodnocenivyucujici') }}">Hodnocení</a></li>
                </ul>
                <!-- Right Side Of Navbar -->
                <ul class="nav navbar-nav navbar-right">
                    <!-- Authentication Links -->
                    @if(Auth::guest())
                        <li><a href="/login">Login</a></li>
                        <li><a href="/register}">Register</a></li>
                    @else
                        <li><a href="/logout">Logout</a></li>
                        <li   style="margin-top: 14.1px;"  >{{ Auth::user()->jmeno }} {{ Auth::user()->prijmeni }}</li>
                    @endif
                </ul>
        </div>
    </nav>
</div>
@include('inc.messages')
@yield('content')
</body>
</html>