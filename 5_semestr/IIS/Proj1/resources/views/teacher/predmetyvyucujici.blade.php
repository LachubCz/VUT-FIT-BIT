@extends ('layouts.teacher')

@section('content')
	<div class="container">
		<h1>Předměty</h1>
		<div class="student">Vyučující: 
			{{Auth::user()->degree->id_titul_pred}}
            {{Auth::user()->jmeno}}
            {{Auth::user()->prijmeni}}
            {{Auth::user()->degree->id_titul_za}}
        </div>
        <br>
        <div style="display: -webkit-inline-box;">
	    @foreach (Auth::user()->position->subjects as $subject)
			<form method="GET" action="subjectvyucujici/{{$subject->id}}">
				{{csrf_field()}}
				<td><button  type="submit"  class="btn btn-warning" style="width: 100px; margin-right: 10px;">{{ $subject->zkratka }}</button></td>
			</form>
	    @endforeach
	    </div>
	</div>
@stop