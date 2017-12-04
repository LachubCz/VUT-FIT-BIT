@extends ('layouts.teacher')

@section('content')
<div class="container">
    <h1>Nová zkouška</h1>

    {!! Form::open(['action' => 'ExamsController@store', 'method' => 'POST']) !!}
    <div class="form-group">
        {{Form::label('typ', 'Typ')}}
        {{Form::text('typ', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
    </div>
    <div class="form-group">
        {{Form::label('max_pocet_bodu', 'Maximální počet bodů')}}
        {{Form::text('max_pocet_bodu', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
    </div>
    <div class="form-group">
        {{Form::label('min_pocet_bodu_pro_slozeni', 'Minimální počet bodů pro složení')}}
        {{Form::number('min_pocet_bodu_pro_slozeni', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
    </div>
    <div class="form-group">
        {{Form::label('pocet_terminu', 'Počet termínů')}}
        {{Form::number('pocet_terminu', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
    </div>
    <div class="form-group">
        {{Form::label('max_pocet_terminu_pro_studenta', 'Maximální počet termínů pro studenta')}}
        {{Form::number('max_pocet_terminu_pro_studenta', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
    </div>
    <div class="form-group">
        {{Form::text('subject_id', $subject->id, ['class' => 'form-control', 'placeholder' => 'Povinné', 'style'=>'display:none'])}}
    </div>
    {{Form::submit('Uložit', ['class'=>'btn btn-primary'])}}
    {!! Form::close() !!}

</div>

@endsection