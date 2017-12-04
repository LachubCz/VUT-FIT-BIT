<!doctype html>
<html lang="{{ app()->getLocale() }}">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="{{asset('css/app.css')}}">
    <title>Informační systém</title>
</head>
<body>

<div class="container">
    <ul class="pagination">
    @foreach ($people as $person)
            <li>
                <a href="/people/{{ $person->position_id }}">
                {{ $person->jmeno }}
                </a>
            </li>

    @endforeach
        </ul>
</div>

{{ $people->links() }}

</body>
</html>