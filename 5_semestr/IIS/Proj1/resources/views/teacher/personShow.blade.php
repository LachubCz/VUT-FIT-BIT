<!doctype html>
<html lang="{{ app()->getLocale() }}">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="{{asset('css/app.css')}}">
    <title>Informační systém</title>
</head>
<body>

<ul class="list-group">

    @if($people->first() != NULL)

        <li class="list-group-item"> Jmeno: {{  $people->first()->jmeno }}</li>
        <li class="list-group-item"> Prijmeni: {{  $people->first()->prijmeni }}</li>
        <li class="list-group-item"> Datum narozeni: {{  $people->first()->datum_narozeni }}</li>
        <li class="list-group-item"> Rodne cislo: {{  $people->first()->rodne_cislo }}</li>
        <li class="list-group-item"> id: {{  $people->first()->id_osoba }}</li>
        <li class="list-group-item"> prezdivka: {{  $people->first()->login }}</li>
        <li class="list-group-item"> e-mail: {{  $people->first()->email }}</li>

    @endif
</ul>

</body>
</html>