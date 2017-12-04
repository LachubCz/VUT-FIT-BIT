@extends('layouts.app')

@section ('content')

    <div class="container">
        @if(auth()->guest())
        <h1>Login</h1>

        <form method="POST" action="/login">

            {{csrf_field()}}

            <div class="form_group has-error">

                <label for="username">UserName:</label>

                <input type="text" class="form-control" id="username" name="username" value="{!! old('username') !!}" required>

            </div>

            <div class="form_group has-error">

                <label for="password">Heslo:</label>

                <input type="password" class="form-control" id="password" name="password" required>

            </div>

            <hr>

            <div class="form_group has-error">

                <button type="submit" class="btn btn-lg btn-primary btn-block">Login</button>

            </div>

        </form>
        @endif

    </div>

@endsection
