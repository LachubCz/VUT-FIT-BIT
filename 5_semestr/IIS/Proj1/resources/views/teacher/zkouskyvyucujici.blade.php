@extends ('layouts.teacher')

@section('content')
	<div class="container">
        <h1>Zkoušky</h1>
		<div class="student">Vyučující: 
			{{Auth::user()->degree->id_titul_pred}}
            {{Auth::user()->jmeno}}
            {{Auth::user()->prijmeni}}
            {{Auth::user()->degree->id_titul_za}}
        </div>
            <div>
                <button onclick="window.location='{{ URL::route('editQuestions')}}'" type="button" class="btn btn-default" style=" margin-right: 10px; margin-top: 12px;">Upravit otázky</button>
            </div>

        @php
        $subjectnum = 0;
        @endphp

	    @foreach (Auth::user()->position->subjects as $subject)
            @php
                $subjectnum++;
            @endphp

            @if($subjectnum % 2 == 0)
                <div class="row">
            @endif
            <div class="col-sm-6">
	        <H2>{{ $subject->zkratka }}</H2>
	        @if($subject->exams->count() < 2)
                @if($subject->garant_id == Auth::user()->position_id)
                    <button onclick="window.location='{{ URL::route('addTermExam', ['subjectid' => $subject->id] )}}'" type="button" class="btn  btn-success" style=" margin-right: 10px;">Přidat zkoušku</button>
                @endif
            @endif
	        @foreach ($subject->exams as $exam)
	        	<h3>{{ $exam->typ }}</h3>
                @if($subject->garant_id == Auth::user()->position_id)
    	        	<div style="display: -webkit-box;">
    	        	@if($exam->pocet_terminu > $exam->terms->count())

        	        	<button onclick="window.location='{{ URL::route('addTerm', ['examid' => $exam->id] )}}'"  type="button" class="btn  btn-success" style=" margin-right: 10px;">Přidat termín</button>

    	        	@endif

    	        		<button onclick="window.location='{{ URL::route('editExam', ['id' => $exam->id] )}}'"  type="button" class="btn  btn-primary" style=" margin-right: 10px;">Upravit zkoušku</button>

    	        		{!!Form::open(['action' => ['ExamsController@destroy', $exam->id], 'method' => 'POST', 'class' => 'pull-right', 'onsubmit' => 'return ConfirmDelete()'])!!}
    					    {{Form::hidden('_method', 'DELETE')}}
    					    {{Form::submit('Smazat zkoušku', ['class' => 'btn btn-danger'])}}
    					{!!Form::close()!!}</div>
                @endif
	        	@foreach ($exam->terms as $term)
	        		<h4>{{$term->cislo_terminu}}. Termín - {{$term->datum_cas}}, {{$term->misto}}</h4>

	        		<div style="display: -webkit-box;">

                        <button onclick="window.location='{{ URL::route('editTermQuestions', ['termid' => $term->id] )}}'" type="button" class="btn btn-default" style=" margin-right: 10px;">Upravit otázky</button>
                        @if($subject->garant_id == Auth::user()->position_id)
                        <button onclick="window.location='{{ URL::route('editTerm', ['termid' => $term->id] )}}'" type="button" class="btn btn-primary" style=" margin-right: 10px;">Upravit termín</button>

                        {!!Form::open(['action' => ['TermsController@destroy', $term->id], 'method' => 'POST', 'class' => 'pull-right', 'onsubmit' => 'return ConfirmDelete()'])!!}
                            {{Form::hidden('_method', 'DELETE')}}
                            {{Form::submit('Smazat termín', ['class' => 'btn btn-danger'])}}
                        {!!Form::close()!!}
                        @endif

                    </div>

                @endforeach
	    	@endforeach

	    	<hr/>
         </div>
        @if($subjectnum % 2 == 0)
            </div>
        @endif
	    @endforeach
    </div>
@stop