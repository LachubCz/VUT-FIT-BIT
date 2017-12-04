<?php

use App\Task;

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/
/*
Route::get('/', function () {
    //return view('welcome');
    return 'Hello world';
});
*/

Route::get('/', 'PeopleController@destroyed');
Route::get('/home', 'PeopleController@destroyed');
Route::get('/homeall', 'PeopleController@home');

Route::resource('exams', 'ExamsController');
Route::resource('scores', 'ScoresController');
Route::resource('terms', 'TermsController');
Route::resource('questions', 'QuestionsController');

Route::get('/people', 'PeopleController@people');
Route::get('/people/{position_id}', 'PeopleController@show');


Route::post('/home', [
    'uses' => 'SessionsController@create',
    'as' => 'login22',
]);

Route::get('/login', [
    'uses' => 'SessionsController@create',
    'as' => 'login',
]);

Route::group(['middleware' => ['auth', 'guest']], function() {
    Route::get('/login', 'SessionsController@create');
});


Route::get('/logout', 'SessionsController@destroy');

Route::group(['middleware' => ['auth', 'guest']], function() {
    Route::get('/login', 'SessionsController@index');
});

Route::post('/login', [
    'uses' => 'SessionsController@store',
    'as' => 'login',
]);


Route::get('/register', 'RegistrationController@create');
Route::post('/register', 'RegistrationController@store');


Route::get('/homeperson', 'PeopleController@home')->name('home');

Route::group(['middleware' => ['resetLastActive', 'auth', 'positions']], function () {
//sem vkladej vsechny routy, kde je clovek prihlaseny

    Route::get('/terminystudent', [
        'uses' => 'TermsController@terminystudent',
        'as' => 'student.terminy',
        'middleware' => 'positions',
        'positions' => ['Student']
    ]);

    Route::post('/terminystudent/{term_id}/{student_id}/prihlasit', [
        'uses' => 'TermsController@terminystudentprihlasit',
        'as' => 'student.terminyodhlasit',
        'middleware' => 'positions',
        'positions' => ['Student']
    ]);

    Route::post('/terminystudent/{term_id}/{student_id}/odhlasit', [
        'uses' => 'TermsController@terminystudentodhlasit',
        'as' => 'student.terminyodhlasit',
        'middleware' => 'positions',
        'positions' => ['Student']
    ]);

    Route::get('/homestudent', [
        'uses' => 'StudentsController@homestudent',
        'as' => 'student.home',
        'middleware' => 'positions',
        'positions' => ['Student']
    ])->name('homestudent');

    Route::get('/predmetystudent', [
        'uses' => 'StudentsController@predmetystudent',
        'as' => 'student.terminy',
        'middleware' => 'positions',
        'positions' => ['Student']
    ]);

    Route::get('/hodnocenistudent', [
        'uses' => 'ScoresController@hodnocenistudent',
        'as' => 'student.registrace',
        'middleware' => 'positions',
        'positions' => ['Student']
    ]);

    Route::get('/homevyucujici', [
        'uses' => 'TeachersController@homevyucujici',
        'as' => 'vyucujici.home',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ])->name('homevyucujici');

    Route::get('/predmetyvyucujici', [
        'uses' => 'TeachersController@predmetyvyucujici',
        'as' => 'vyucujici.predmety',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::get('/hodnocenivyucujici', [
        'uses' => 'TeachersController@hodnocenivyucujici',
        'as' => 'vyucujici.hodnoceni',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);


    Route::get('/zkouskyvyucujici', [
        'uses' => 'TeachersController@zkouskyvyucujici',
        'as' => 'vyucujici.zkouskyi',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::get('/subjectvyucujici/{id}', [
        "uses" => 'SubjectsController@subjectTeacher',
        "as" => 'subjectteacher',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::get('/subjects', [
        "uses" => 'SubjectsController@index',
        "as" => 'subjectindex',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant', 'Student']
    ]);

    Route::get('subjectvyucujici/subject/{id}', [
        "uses" => 'SubjectsController@show',
        "as" => 'subject',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant', 'Student']
    ]);

    Route::get('/subject/{id}', [
        "uses" => 'SubjectsController@show',
        "as" => 'subjectshow',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant', 'Student']
    ]);

    Route::get('/subject/score/{termid}', [
        "uses" => 'ScoresController@hodnocenivyucujiciho',
        "as" => 'examscore',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::get('/addTerm/forexam/{examid}', [
        "uses" => 'TermsController@addTerm',
        "as" => 'addTerm',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::get('/editTerm/{termid}', [
        "uses" => 'TermsController@editTerm',
        "as" => 'editTerm',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::get('/editExam/{id}', [
        "uses" => 'ExamsController@editExam',
        "as" => 'editExam',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::get('/student/{studid}/{subid}', [
        "uses" => 'StudentsController@studentDetails',
        "as" => 'studentDetails',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::get('/editQuestions', [
        "uses" => 'QuestionsController@editQuestions',
        "as" => 'editQuestions',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::get('/editTermQuestions/{termid}', [
        "uses" => 'QuestionsController@editTermQuestions',
        "as" => 'editTermQuestions',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);


    Route::get('/addTermExam/{subjectid}', [
        "uses" => 'ExamsController@addTermExam',
        "as" => 'addTermExam',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::post('/exams/{subjectid}', [
        "uses" => 'ExamsController@addTermExam',
        "as" => 'addTermExamPost',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::post('/storeTermQuestion/{termid}', [
            "uses" => 'QuestionsController@storeTerm',
            "as" => 'storeTermQuestion',
            'middleware' => 'positions',
            'positions' => ['Teacher', 'Garant']
     ]);

    Route::get('/addTermQuestion/{termid}/{questionid}', [
        "uses" => 'QuestionsController@addTerm',
        "as" => 'addTermQuestion',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);

    Route::delete('/termQuestionDestroy/{termid}/{questionid}', [
        "uses" => 'QuestionsController@termQuestionDestroy',
        "as" => 'termQuestionDestroy',
        'middleware' => 'positions',
        'positions' => ['Teacher', 'Garant']
    ]);



//sem vkladej vsechny routy, kde je clovek prihlaseny
});

// This will be the route that checks expiration!
Route::post('session/ajaxCheck', ['uses' => 'SessionsController@ajaxCheck', 'as' => 'session.ajax.check']);

Route::get('/clear-cache', function() {
    $exitCode = Artisan::call('cache:clear');
    return view('home');
});

