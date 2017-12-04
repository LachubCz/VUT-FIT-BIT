@extends ('layouts.app')

@section('content')

    <div class="container">
        <h1>{{$subject->zkratka}}</h1>
        <div class="table-responsive">
            <table class="table table-hover">
                <thead>
                <tr>
                    <th></th>
                </tr>
                </thead>
                <tbody>
                <tr>
                <tr>
                    <th scope="row">Počet kreditů</th>
                    <td>{{ $subject->pocet_kreditu }}</td>
                </tr>
                <tr>
                    <th scope="row">Garant</th>
                    <td>{{ $subject->garant->person->jmeno }} {{ $subject->garant->person->prijmeni }}</td>
                </tr>
                <tr>
                    <th scope="row">Učitelé</th>
                    <td>
                        @foreach($subject->teachers as $teacher)
                            @if($subject->garant->person->id == $teacher->id)

                            @else
                                {{$teacher->person->jmeno}} {{$teacher->person->prijmeni}}, &nbsp;&nbsp;&nbsp;&nbsp;
                            @endif
                        @endforeach
                    </td>
                </tr>
                <tr>
                    <th scope="row">Zkoušené znalostí</th>
                    <td></td>
                    @foreach($subject->exams as $exam)
                        <tr><th></th><td>{{$exam->typ}}</td></tr>
                     @endforeach
                <tr>
                    <th scope="row">Garant</th>
                    <td>{{ $subject->garant->person->jmeno }} {{ $subject->garant->person->prijmeni }}</td>
                </tr>
                </tbody>
            </table>
        </div>

@endsection