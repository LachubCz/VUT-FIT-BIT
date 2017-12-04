@extends ('layouts.student')

@section('content')
  <div class="container">
    <div class="container">
      <h1>Napsané termíny</h1>
          <div class="table-responsive">
              <table class="table table-hover">
                  <thead>
                  <tr>
                      <th></th>
                  </tr>
                  </thead>
                  <tbody>
                  @foreach ($terms as $term)
                  <tr>
                  <tr>
                      <th scope="row">Předmět</th>
                      <td>{{$term->exam->subject->zkratka}}</td>
                  </tr>
                  <tr>
                      <th scope="row">Zkouška</th>
                      <td>{{$term->exam->typ}}</td>
                  </tr>
                  <tr>
                      <th scope="row">Termín</th>
                      <td>{{$term->cislo_terminu}}</td>
                  </tr>
                  <tr>
                      <th scope="row">Max hodnocení</th>
                      <td>{{$term->exam->max_pocet_bodu}}</td>
                  </tr>
                  <tr>
                      <th scope="row">Min pro složení</th>
                      <td>{{$term->exam->min_pocet_bodu_pro_slozeni}}</td>
                  </tr>
                  </tbody>
              </table>
          </div>

        <div class="table-responsive">
            <table class="table table-hover">
                <tr>
                    <th>Zadání</th>
                    <th>Maximální počet bodů</th>
                </tr>
                @foreach ($term->questions as $question)
                    <tr>
                        <td>{{$question->zadani_otazky}}</td>
                        <td>{{$question->max_pocet_bodu}}</td>
                    </tr>

                @endforeach

            </table>
        </div>
        <div class="table-responsive">
            <table class="table table-hover">
                <tr>
                <th>Zadání</th>
                <th>Získané</th>
                <th>Procentuální úspěšnost</th>
                <th>Komentář</th>
                </tr>


                    @foreach (auth()->user()->position->scores as $score)
                        @if($term->id == $score->term_id)
                    <tr>
                        <td>{{$score->question->zadani_otazky}}</td>
                        <td>{{$score->pocet_bodu}}</td>
                        <td>{{$score->procentualni_uspesnost}}</td>
                        <td>{{$score->komentar}}</td>
                    </tr>
                    @endif
                @endforeach

            </table>
        </div>



              @endforeach


  </div>

@stop