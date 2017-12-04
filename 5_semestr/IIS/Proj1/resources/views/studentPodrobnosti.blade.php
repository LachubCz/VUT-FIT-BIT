@extends ('layouts.teacher')

@section('content')
	<div class="container">
	@foreach ($subjects as $subject)
		@if($subject->id == $subid)
			<h1>{{$subject->zkratka}} - {{$student->person->username}}, {{$student->person->prijmeni}} {{$student->person->jmeno}}</h1>
			@break
		@endif
	@endforeach	

	        <button onclick="window.location='{{ URL::route('subjectteacher', ['id' => $subid]) }}'" type="button" class="btn" style=" margin-right: 10px;">Zpět</button>

			@foreach ($student->terms as $term)
				@if($term->exam->subject->id == $subid)
					<h2>{{$term->exam->typ}} - {{$term->cislo_terminu}}. Termín</h2>
					<table class="table" style="margin-top:10px;">
			        <thead>
				      <tr>
				        <th>Otázka</th>
				        <th>Počet bodů</th>
				        <th>Komentář</th>
				        <th></th>
				      </tr>
				    </thead>
					<tbody>
						@foreach ($term->questions as $question)
						<tr>
							<td>{{ $question->zadani_otazky }}</td>
			      			@php ($in = false)
			      			@php ($scoreObj = null)
			      			@foreach ($student->scores as $score)
				      			@if($score->question_id == $question->id)
				      				@if($score->term_id == $term->id)
					      				@php ($in = true)
					      				@php ($scoreObj = $score)
					      				@break
					      			@endif
						    	@endif
						    @endforeach
						    @if($in == false)
							    	{!! Form::open(['action' => 'ScoresController@store', 'method' => 'POST']) !!}
								        <td><div class="form-group">
								            {{Form::number('pocet_bodu', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
								        </div></td>
								        <td><div class="form-group">
								            {{Form::text('komentar', '', ['class' => 'form-control', 'placeholder' => ''])}}
								            {{Form::number('teacher_id', Auth::user()->position_id, ['class' => 'form-control', 'style'=>'display:none'])}}
								            {{Form::number('student_id', $student->person->position_id , ['class' => 'form-control', 'style'=>'display:none'])}}
								            {{Form::number('max_bodu', $question->max_pocet_bodu , ['class' => 'form-control', 'style'=>'display:none'])}}
								            {{Form::number('question_id', $question->id , ['class' => 'form-control', 'style'=>'display:none'])}}
								            {{Form::number('term_id', $term->id , ['class' => 'form-control', 'style'=>'display:none'])}}
								        </div></td>
								        <td>{{Form::submit('Uložit', ['class'=>'btn btn-primary'])}}</td>
							    	{!! Form::close() !!}
							@else
							    	{!! Form::open(['action' => ['ScoresController@update', $scoreObj->id], 'method' => 'POST']) !!}
								        <td><div class="form-group">
								            {{Form::number('pocet_bodu', $scoreObj->pocet_bodu, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
								        </div></td>
								        <td><div class="form-group">
								            {{Form::text('komentar', $scoreObj->komentar, ['class' => 'form-control', 'placeholder' => ''])}}
								            {{Form::number('teacher_id', Auth::user()->position_id, ['class' => 'form-control', 'style'=>'display:none'])}}
								            {{Form::number('student_id', $student->person->position_id , ['class' => 'form-control', 'style'=>'display:none'])}}
								            {{Form::number('max_bodu', $question->max_pocet_bodu , ['class' => 'form-control', 'style'=>'display:none'])}}
								            {{Form::number('question_id', $question->id , ['class' => 'form-control', 'style'=>'display:none'])}}
								        </div></td>{{Form::hidden('_method', 'PUT')}}
								        <td>{{Form::submit('Uložit', ['class'=>'btn btn-primary'])}}
							    	{!! Form::close() !!}
									{!!Form::open(['action' => ['ScoresController@destroy', $scoreObj->id], 'method' => 'POST', 'class' => 'pull-right', 'onsubmit' => 'return ConfirmDelete()'])!!}
						                {{Form::hidden('_method', 'DELETE')}}
						                {{Form::submit('Smazat', ['class' => 'btn btn-danger'])}}
						            {!!Form::close()!!}</td>
							@endif
			      			</tr>
						@endforeach
					</tbody>
					</table>
				@endif
			@endforeach
	</div>
@stop