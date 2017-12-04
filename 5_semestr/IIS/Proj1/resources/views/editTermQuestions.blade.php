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
                    <td>
                         {{$question->zadani_otazky}}
                        </td>
                    <td>
                            {{$question->max_pocet_bodu}}
                    </td>
                    <td>
                            {{$question->min_pocet_bodu_pro_slozeni}}
                        </td>
                    <td>
                        {!!Form::open(['route' => ['termQuestionDestroy', $termid, $question->id], 'method' => 'POST', 'class' => 'pull-right'])!!}
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
                {!! Form::open(['route' => ['storeTermQuestion', $termid], 'method' => 'POST']) !!}
                <td><div class="form-group">
                        {{Form::text('zadani_otazky', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
                    </div></td>
                <td><div class="form-group">
                        {{Form::number('max_pocet_bodu', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
                    </div></td>
                <td><div class="form-group">
                        {{Form::number('min_pocet_bodu_pro_slozeni', '', ['class' => 'form-control', 'placeholder' => 'Povinné'])}}
                        {{Form::number('typ_otazky', '1', ['class' => 'form-control', 'style'=>'display:none'])}}
                    </div></td>
                <td>{{Form::submit('Uložit', ['class'=>'btn btn-primary'])}}</td>
                {!! Form::close() !!}
            </tr>
            </tbody>
        </table>
        <h2>Přidat ze seznamu všech otázek</h2>

        <p>
            <button class="btn btn-primary" type="button" data-toggle="collapse" data-target="#collapseExample" aria-expanded="false" aria-controls="collapseExample">
                Zobraz všechny otázky
            </button>
        </p>
        <div class="collapse" id="collapseExample">
            <div class="card card-body">
                <table class="table" style="margin-top:10px;">
                    <thead>
                    <tr>
                        <th>Zadání otázky</th>
                        <th>Maximální počet bodů</th>
                        <th>Minimální počet bodů pro složení</th>
                    </tr>
                    </thead>
                    <tbody>

                    @foreach ($all_questions as $question)
                        <tr>
                            {!! Form::open(['route' => ['addTermQuestion', $termid, $question->id], 'method' => 'GET']) !!}
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
                            <td>{{Form::submit('Přidat', ['class'=>'btn btn-success'])}}
                                {!! Form::close() !!}
                        </tr>

                    @endforeach
                    </tbody>
                </table>
            </div>
        </div>

    </div>
@stop