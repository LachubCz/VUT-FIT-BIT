@extends ('layouts.teacher')

@section('content')
	<div class="container">
		<h1>Hodnocení</h1>
		<div class="student">Vyučující: 
			{{Auth::user()->degree->id_titul_pred}}
            {{Auth::user()->jmeno}}
            {{Auth::user()->prijmeni}}
            {{Auth::user()->degree->id_titul_za}}
        </div>

	    @foreach (Auth::user()->position->subjects as $subject)
	        <H2>{{ $subject->zkratka }}</H2>
	        @if($subject->exams->count() == 0)
	        	<p>Žádné nahlášené zkoušky</p>
	        @endif
	        @foreach ($subject->exams as $exam)
	        	<h3>{{ $exam->typ }}</h3>
	        	@if($exam->terms->count() == 0)
	        	<p>Žádné nahlášené termíny</p>
	        	@endif
	        	@foreach ($exam->terms as $term)
	        		<button onclick="window.location='{{ URL::route('examscore', ['termid' => $term->id]) }}'" type="button" class="btn btn-warning" style=" margin-right: 10px;">{{$term->cislo_terminu}}. Termín</button>
	        	@endforeach
	    	@endforeach
	    @endforeach
	</div>
@stop