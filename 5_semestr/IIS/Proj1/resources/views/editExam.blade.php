@extends ('layouts.teacher')

@section('content')
	<div class="container">
		<h1>Úprava: {{$exam->subject->zkratka}}, {{$exam->typ}} </h1>
		<div class="student">Vyučující:
			{{Auth::user()->degree->id_titul_pred}}
            {{Auth::user()->jmeno}}
            {{Auth::user()->prijmeni}}
            {{Auth::user()->degree->id_titul_za}}
        </div>
        <br>
    	{!! Form::open(['action' => ['ExamsController@store', $exam->id], 'method' => 'POST']) !!}
	        <div class="form-group">
	            {{Form::text('typ', $exam->typ, ['class' => 'form-control', 'placeholder' => 'Povinné', 'style'=>'display:none'])}}
	        </div>
	        <div class="form-group">
	        	{{Form::label('max_pocet_bodu', 'Maximální počet bodů')}}
	            {{Form::text('max_pocet_bodu', $exam->max_pocet_bodu, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
		    </div>
		    <div class="form-group">
		    	{{Form::label('min_pocet_bodu_pro_slozeni', 'Minimální počet bodů pro složení')}}
		    	{{Form::date('min_pocet_bodu_pro_slozeni', $exam->min_pocet_bodu_pro_slozeni, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
		    </div>
		    <div class="form-group">
		    	{{Form::label('pocet_terminu', 'Počet termínů')}}
		    	{{Form::date('pocet_terminu', $exam->pocet_terminu, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
		    </div>
		    <div class="form-group">
		    	{{Form::label('max_pocet_terminu_pro_studenta', 'Maximální počet termínů pro studenta')}}
		    	{{Form::number('max_pocet_terminu_pro_studenta', $exam->max_pocet_terminu_pro_studenta, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
		    </div>
		    <div class="form-group">
		    	{{Form::number('subject_id', $exam->subject_id, ['class' => 'form-control', 'placeholder' => 'Povinné', 'style'=>'display:none'])}}
		    </div>
		    	{{Form::hidden('_method', 'PUT')}}
		        {{Form::submit('Uložit', ['class'=>'btn btn-primary'])}}</td>
	   	{!! Form::close() !!}

	</div>
@stop