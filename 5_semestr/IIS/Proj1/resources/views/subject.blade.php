@extends ('layouts.teacher')

@section('content')
	<div class="container">
	@foreach ($subjects as $subject)
		@if($subject->id == $id)
			<h1>{{$subject->zkratka}}</h1>
			@break
		@endif
	@endforeach	

			<form method="GET" action="subject/{{$subject->id}}">
				{{csrf_field()}}
				<td><button  type="submit"  class="btn" style="margin-right: 10px; ">Informace o předmětu</button></td>
			</form>
        <table class="table" style="margin-top:10px;">
        <thead>
	      <tr>
	        <th>Login</th>
	        <th>Jméno</th>
	        <th>Přijmení</th>
	        <th>Hodnocení</th>
	        <th></th>
	      </tr>
	    </thead>
		<tbody>
	    @foreach ($students as $student)
    		@foreach ($student->subjects as $subject)
    			@if($subject->id == $id)
	    			<tr href="\home">
	    			<td>{{ $student->person->username }}</td>
	    			
	      			<td>{{ $student->person->jmeno }}</td>

	      			<td>{{ $student->person->prijmeni }}</td>
					<td>
						
						@php($scoreSum = 0)
							@foreach($subjects[$id-1]->exams as $exam)
							@php($biggestScoreFromTerm = 0)
								@foreach($exam->terms as $term)
									@php($ScoreFromTerm = 0)
									@foreach($term->questions as $question)
										@foreach($student->scores as $score)
											@if($question->id == $score->question_id && $score->term_id == $term->id)
												@php($ScoreFromTerm = $ScoreFromTerm + $score->pocet_bodu)
											@endif
										@endforeach
									@endforeach	
									@if($ScoreFromTerm > $biggestScoreFromTerm)
										@php($biggestScoreFromTerm = $ScoreFromTerm)
									@endif
								@endforeach	
								@php($scoreSum = $scoreSum + $biggestScoreFromTerm)

							@endforeach	
						{{$scoreSum}}
						
						
					</td>
	      			<td>
	        		<button onclick="window.location='{{ URL::route('studentDetails', ['subid' => $id, 'studid' => $student->id]) }}'" type="button" class="btn btn-primary" style=" margin-right: 10px;">Podrobnosti</button></td>
	      			</tr>
	      			@break
	    		@endif
    		@endforeach
	    @endforeach
	    </tbody>
		</table>
	</div>
@stop