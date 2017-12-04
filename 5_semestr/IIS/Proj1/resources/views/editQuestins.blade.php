@extends ('layouts.teacher')

@section('content')
	<div class="container">
		<h1>Aktuální otázky</h1>
		<div class="student">Vyučující:
			{{Auth::user()->degree->id_titul_pred}}
            {{Auth::user()->jmeno}}
            {{Auth::user()->prijmeni}}
            {{Auth::user()->degree->id_titul_za}}
        </div>

        <table class="table" style="margin-top:10px;">
        <thead>
	      <tr>
	        <th>Zadání otázky</th>
	        <th>Maximální počet bodů</th>
	        <th>Minimální počet bodů pro složení</th>
	      </tr>
	    </thead>
		<tbody>
			@foreach ($questions as $question)
			<tr>
			  	{!! Form::open(['action' => ['QuestionsController@update', $question->id], 'method' => 'POST']) !!}
			       <td><div class="form-group">
			           {{Form::text('zadani_otazky', $question->zadani_otazky, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
			       </div></td>
			       <td><div class="form-group">
			           {{Form::number('max_pocet_bodu', $question->max_pocet_bodu, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
			        </div></td>
			       <td><div class="form-group">
			           {{Form::number('min_pocet_bodu_pro_slozeni', $question->min_pocet_bodu_pro_slozeni, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
			           {{Form::number('typ_otazky', $question->typ_otazky, ['class' => 'form-control', 'style'=>'display:none'])}}
			       </div></td>{{Form::hidden('_method', 'PUT')}}
			        <td>{{Form::submit('Uložit', ['class'=>'btn btn-primary'])}}
			   	{!! Form::close() !!}
			   	{!!Form::open(['action' => ['QuestionsController@destroy', $question->id], 'method' => 'POST', 'class' => 'pull-right'])!!}
					    {{Form::hidden('_method', 'DELETE')}}
					    {{Form::submit('Smazat', ['class' => 'btn btn-danger'])}}
				{!!Form::close()!!}</td>
			   	</tr>
			@endforeach
	    </tbody>
		</table>
		<h2>Přidat novou otázku</h2>
		<table class="table" style="margin-top:10px;">
        <thead>
	      <tr>
	        <th>Zadání otázky</th>
	        <th>Maximální počet bodů</th>
	        <th>Minimální počet bodů pro složení</th>
	      </tr>
	    </thead>
		<tbody>
			<tr>
			  	{!! Form::open(['action' => 'QuestionsController@store', 'method' => 'POST']) !!}
			       <td><div class="form-group">
			           {{Form::text('zadani_otazky', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
			       </div></td>
			       <td><div class="form-group">
			           {{Form::number('max_pocet_bodu', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
			        </div></td>
			       <td><div class="form-group">
			           {{Form::number('min_pocet_bodu_pro_slozeni', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
			           {{Form::number('typ_otazky', 1, ['class' => 'form-control', 'style'=>'display:none'])}}
			       </div></td>
			        <td>{{Form::submit('Uložit', ['class'=>'btn btn-primary'])}}</td>
			   	{!! Form::close() !!}
			</tr>
		</tbody>
		</table>
	</div>
@stop