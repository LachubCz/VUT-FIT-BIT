@extends ('layouts.app')

@section('content')

    <div class="container">
        <h1>Předměty</h1>
        <div class="table-responsive">
            <table class="table table-hover">
                <tr>
                    <th>Předmět</th>
                    <th>Počet kreditů</th>
                    <th>Garant</th>
                    <th>Učitelé</th>
                </tr>
                @foreach ($subjects as $subject)
                    <tr>
                        <td>{{$subject->zkratka}}</td>
                        <td>{{ $subject->pocet_kreditu }}</td>
                        <td>{{ $subject->garant->person->jmeno }} {{ $subject->garant->person->prijmeni }}</td>
                        <td> @foreach($subject->teachers as $teacher)
                                @if($subject->garant->person->id == $teacher->id)

                                @else
                                    {{$teacher->person->jmeno}} {{$teacher->person->prijmeni}}, &nbsp;&nbsp;&nbsp;&nbsp;
                        @endif
                        @endforeach
                    </tr>
                @endforeach
            </table>
        </div>

@endsection