@extends ('layouts.student')

@section('content')
  <div class="container">
      <h1>Informační systém</h1>
	  <div class="student">Dobrý den,
           {{Auth::user()->degree->id_titul_pred}}
          {{Auth::user()->jmeno}}
          {{Auth::user()->prijmeni}}
          {{Auth::user()->degree->id_titul_za}}
            .

      </div>

  </div>
@endsection