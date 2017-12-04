<?php

namespace App\Http\Controllers;

use App\term;
use App\score;
use Carbon\Carbon;
use Illuminate\Database\Eloquent\Collection;
use Illuminate\Http\Request;

class ScoresController extends Controller
{
    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {
        //
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
        $this->validate($request, [
            'pocet_bodu' => 'required|numeric',
            'teacher_id' => 'required',
            'student_id' => 'required',
            'question_id' => 'required'
        ]);
        $score = new score;
        $score->pocet_bodu = $request->input('pocet_bodu');
        $score->komentar = $request->input('komentar');
        $score->procentualni_uspesnost = ($request->input('pocet_bodu')/$request->input('max_bodu')) * 100;
        $score->teacher_id = $request->input('teacher_id');
        $score->student_id = $request->input('student_id');
        $score->question_id = $request->input('question_id');
        $score->term_id = $request->input('term_id');
        $score->save();

        return back()->with('success', 'Uloženo');
    }

    /**
     * Display the specified resource.
     *
     * @param  \App\score  $score
     * @return \Illuminate\Http\Response
     */
    public function show(score $score)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     *
     * @param  \App\score  $score
     * @return \Illuminate\Http\Response
     */
    public function edit(score $score)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @param  \App\score  $score
     * @return \Illuminate\Http\Response
     */
    public function update(Request $request, $score)
    {
        $this->validate($request, [
            'pocet_bodu' => 'required|numeric',
            'teacher_id' => 'required',
            'student_id' => 'required',
            'question_id' => 'required'
        ]);
        $scorenew = score::find($score);
        $scorenew->pocet_bodu = $request->input('pocet_bodu');
        $scorenew->komentar = $request->input('komentar');
        $scorenew->procentualni_uspesnost = ($request->input('pocet_bodu')/$request->input('max_bodu')) * 100;
        $scorenew->teacher_id = $request->input('teacher_id');
        $scorenew->student_id = $request->input('student_id');
        $scorenew->question_id = $request->input('question_id');
        $scorenew->save();

        return back()->with('success', 'Uloženo');  
    }

    /**
     * Remove the specified resource from storage.
     *
     * @param  \App\score  $score
     * @return \Illuminate\Http\Response
     */
    public function destroy($score)
    {
        $scorenew = score::find($score);
        $scorenew->delete();
        return back()->with('success', 'Smazáno');  
    }

    public function hodnocenivyucujiciho($termid)
    {
        $term = term::Find($termid);
        return view('score')->with('term',$term);
    }

    public function hodnocenistudent()
    {

        $datetime =  Carbon::now()->toDateTimeString();

        $terms = new Collection();

        foreach(auth()->user()->position->subjects as $subject) {
            foreach($subject->exams as $exam) {
                foreach ($exam->terms as $term) {
                    $terms->push($term);
                }
            }
        }

        $terms = $terms->where('datum_cas', '<', $datetime);

        return view('student.hodnocenistudent', compact('terms'));
    }
}
