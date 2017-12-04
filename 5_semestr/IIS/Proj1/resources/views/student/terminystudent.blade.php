@extends ('layouts.student')

@section('content')
  <div class="container">
    <h1>Termíny</h1>
              <div class="container">
                  <div class="table-responsive">
                      <table class="table table-hover">
                          <tr>
                              <th>Zkratka předmětu</th>
                              <th>Typ zkoušky</th>
                              <th>Datum</th>
                              <th>Misto</th>
                              <th>Začátek Prihlasování</th>
                              <th>Konec příhlášování</th>
                              <th>Maximální počet studentů</th>
                              <th>Aktuální počet studentů</th>
                              <th>Číslo termínu</th>
                          </tr>
                          @foreach ($terms as $term)
                                  @if($term->exam->typ == "Pulsemestralni zkouska")
                                      <tr class="table-secondary">
                                  @elseif($term->konec_prihlasovani < $date)
                                      @php
                                          $writtenterm = false;

                                            foreach(auth()->user()->position->terms as $studentterm) {
                                                 if($term->id == $studentterm->id) {
                                                    $writtenterm = true;
                                                }
                                           }
                                      @endphp
                                  @else
                                      <tr class="table-primary">
                                  @endif
                                  <td>{{$term->exam->subject->zkratka}}</td>
                                  <td>{{$term->exam->typ}}</td>
                                  <td>{{ $term->datum_cas }}</td>
                                  <td>{{ $term->misto }}</td>
                                  <td>{{ $term->zacatek_prihlasovani }}</td>
                                  <td>{{ $term->konec_prihlasovani }}</td>
                                  <td>{{ $term->max_pocet_studentu }}</td>
                                  <td>{{ count($term->students) }}</td>
                                  <td>{{ $term->cislo_terminu }}</td>
                                  @if($term->zacatek_prihlasovani < $date && $term->konec_prihlasovani > $date)
                                      @foreach(auth()->user()->position->terms as $studentterm)
                                          @if($term->id == $studentterm->id)
                                              <form method="POST" action="/terminystudent/{{$term->id}}/{{ auth()->user()->position->id }}/odhlasit">
                                                  {{csrf_field()}}
                                                <td><button  type="submit"  class="btn btn-danger" style="width: 100px; margin-right: 10px;">Odhlásit</button></td>
                                              </form>
                                          @endif
                                      @endforeach
                                  @endif
                                  @php
                                            $post = true;

                                            foreach(auth()->user()->position->terms as $studentterm) {
                                                 if($term->id == $studentterm->id) {
                                                    $post = false;
                                                }
                                           }
                                  @endphp
                                  @if($term->zacatek_prihlasovani < $date && $term->konec_prihlasovani > $date)
                                  @if($post == true)
                                  <form method="POST" action="/terminystudent/{{$term->id}}/{{ auth()->user()->position_id }}/prihlasit">
                                      {{csrf_field()}}
                                      <td><button  type="submit"  class="btn btn-success" style="width: 100px; margin-right: 10px;">Přihlásit</button></td>
                                  </form>
                                  @endif
                                  @endif

                              </tr>
                          @endforeach
                      </table>
                  </div>
  </div>
@stop	