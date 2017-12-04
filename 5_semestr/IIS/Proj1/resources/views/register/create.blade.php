@extends('layouts.app')

@section ('content')

    <div class="container">
        @if(auth()->guest())

        <h1>Register</h1>



        <form role="form" method="POST" action="/register">

            {{csrf_field()}}

            <div class="form_group has-error">

                <label for="username">UserName:</label>

                <input type="text" class="form-control " id="username" name="username" value="{!! old('username') !!}" required>

            </div>


            <div class="form_group has-error">

                <label for="jmeno">Jméno:</label>

                <input type="text" class="form-control" id="jmeno" name="jmeno" value="{!! old('jmeno') !!}" required>

            </div>

            <div class="form_group has-error">

                <label for="prijmeni">Příjmení:</label>

                <input type="text" class="form-control" id="prijmeni" name="prijmeni" value="{!! old('prijmeni') !!}" required>

            </div>

            <div class="form_group has-error">

                <label for="email">Email:</label>

                <input type="email" class="form-control" id="email" name="email" value="{!! old('email') !!}" required>

            </div>

            <div class="form_group has-error">

                <label for="password">Heslo:</label>

                <input type="password" class="form-control" id="password" name="password" required>

            </div>

            <div class="form_group has-error">

                <label for="password_confirmation">Potvrzení hesla:</label>

                <input type="password" class="form-control" id="password_confirmation" name="password_confirmation" required>

            </div>

            <hr>

            <div class="form_group">

                <button type="submit" class="btn btn-lg btn-primary btn-block">Register</button>

            </div>

        </form>
        @endif


    </div>

@endsection