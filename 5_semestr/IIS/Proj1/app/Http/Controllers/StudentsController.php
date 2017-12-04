<?php

namespace App\Http\Controllers;

use App\student;
use App\term;
use App\subject;
use Carbon\Carbon;
use function GuzzleHttp\Promise\all;
use Illuminate\Database\Eloquent\Collection;
use Illuminate\Http\Request;
use Illuminate\Pagination\LengthAwarePaginator;
use Illuminate\Pagination\Paginator;
use Illuminate\Support\Facades\DB;
use PhpParser\Node\Expr\Array_;

class StudentsController extends Controller
{
    public function __construct()
    {
        $this->middleware('auth');//->except(['something', 'something_else']);
    }

    protected static function paginate($items, $perPage = 12)
    {
        //Get current page form url e.g. &page=1
        $currentPage = LengthAwarePaginator::resolveCurrentPage();

        //Slice the collection to get the items to display in current page
        $currentPageItems = $items->slice(($currentPage - 1) * $perPage, $perPage);

        //Create our paginator and pass it to the view
        return new LengthAwarePaginator($currentPageItems, count($items), $perPage);
    }

    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {

        $people = collect(DB::select('select * from people join students where students.id = people.position_id'));


        $peopleColl = StudentsController::paginate($people , 5)->setPath('students');



       return view('student.students', ['people' => $peopleColl]);
    }

    //student
    public function homestudent()
    {
        //if (Auth::guest()) return redirect()->guest('sessions');
        $studentdata = array(
            'jmeno' => 'Tomáš',
            'prijmeni' => 'Holík',
            'sessions' => 'xholik13'
        );
        return view('student.homestudent')->with($studentdata);
    }

    public function predmetystudent()
    {
        $subjects = auth()->user()->position->subjects;

        return view('student.predmetystudent', compact('subjects'));
    }

    /**
     * Show the form for creating a new resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function create()
    {
        //
    }

    /**
     * Store a newly created resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @return \Illuminate\Http\Response
     */
    public function store(Request $request)
    {
        //
    }

    /**
     * Display the specified resource.
     *
     * @param  \App\student  $student
     * @return \Illuminate\Http\Response
     */
    public function show(student $student)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     *
     * @param  \App\student  $student
     * @return \Illuminate\Http\Response
     */
    public function edit(student $student)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @param  \App\student  $student
     * @return \Illuminate\Http\Response
     */
    public function update(Request $request, student $student)
    {
        //
    }

    /**
     * Remove the specified resource from storage.
     *
     * @param  \App\student  $student
     * @return \Illuminate\Http\Response
     */
    public function destroy(student $student)
    {
        //
    }

    public function studentDetails($studid, $subid)
    {
        $student = student::Find($studid);
        $subjects = subject::All();

        return view('studentPodrobnosti')->with('student', $student)->with('subid', $subid)->with('subjects', $subjects);
    }
}
