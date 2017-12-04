@extends ('layouts.teacher')

@section('content')
	<div class="container">
		<h1>Upravení termínu</h1>
		<div class="student">Vyučující:
			{{Auth::user()->degree->id_titul_pred}}
            {{Auth::user()->jmeno}}
            {{Auth::user()->prijmeni}}
            {{Auth::user()->degree->id_titul_za}}
        </div>
        <br>
    	{!! Form::open(['action' => ['TermsController@update', $term->id], 'method' => 'POST']) !!}
	        <div class="form-group">
	        	{{Form::label('datum_cas', 'Datum a čas')}}
	            {{Form::text('datum_cas', $term->datum_cas, ['class' => 'form-control', 'placeholder' => 'Povinné - (rrrr-mm-dd hh:mm:ss)'])}}
	        </div>
	        <div class="form-group">
	        	{{Form::label('misto', 'Místo')}}
	            {{Form::text('misto', $term->misto, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
		    </div>
		    <div class="form-group">
		    	{{Form::label('zacatek_prihlasovani', 'Začátek přihlašování')}}
		    	{{Form::date('zacatek_prihlasovani', $term->zacatek_prihlasovani, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
		    </div>
		    <div class="form-group">
		    	{{Form::label('konec_prihlasovani', 'Konec přihlašování')}}
		    	{{Form::date('konec_prihlasovani', $term->konec_prihlasovani, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
		    </div>
		    <div class="form-group">
		    	{{Form::label('max_pocet_studentu', 'Maximální počet studentú')}}
		    	{{Form::number('max_pocet_studentu', $term->max_pocet_studentu, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
		    </div>
		    <div class="form-group">
		    	{{Form::label('cislo_terminu', 'Číslo termínu')}}
		    	{{Form::number('cislo_terminu', $term->cislo_terminu, ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
		    </div>
		    <div class="form-group">
		    	{{Form::text('exam_id', $term->exam->id, ['class' => 'form-control', 'placeholder' => 'Povinné', 'style'=>'display:none'])}}
		    </div>
		    	{{Form::hidden('_method', 'PUT')}}
		        {{Form::submit('Uložit', ['class'=>'btn btn-primary'])}}</td>
	   	{!! Form::close() !!}

	</div>
@stop