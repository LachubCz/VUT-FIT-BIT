<?php

namespace App\Http\Controllers;

use App\score;
use App\question;
use App\term;
use Illuminate\Http\Request;

class QuestionsController extends Controller
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
            'typ_otazky' => 'required|numeric',
            'zadani_otazky' => 'required',
            'max_pocet_bodu' => 'required|numeric|min:0',
            'min_pocet_bodu_pro_slozeni' => 'required|numeric|min:0'
        ]);

        if($request->input('max_pocet_bodu') < $request->input('min_pocet_bodu_pro_slozeni'))
        {
            return back()->with('error', 'Maximální počet bodů je menší než minimální počet bodů pro úspěšné složení zkoušky.');
        }

        $question = new question;
        $question->typ_otazky = $request->input('typ_otazky');
        $question->zadani_otazky = $request->input('zadani_otazky');
        $question->max_pocet_bodu = $request->input('max_pocet_bodu');
        $question->min_pocet_bodu_pro_slozeni = $request->input('min_pocet_bodu_pro_slozeni');

        $question->save();

        return back()->with('success', 'Uloženo');
    }

    public function storeTerm(Request $request, $termid)
    {
        $this->validate($request, [
            'typ_otazky' => 'required|numeric',
            'zadani_otazky' => 'required',
            'max_pocet_bodu' => 'required',
            'min_pocet_bodu_pro_slozeni' => 'required'
        ]);

        $question = new question;
        $question->typ_otazky = $request->input('typ_otazky');
        $question->zadani_otazky = $request->input('zadani_otazky');
        $question->max_pocet_bodu = $request->input('max_pocet_bodu');
        $question->min_pocet_bodu_pro_slozeni = $request->input('min_pocet_bodu_pro_slozeniS');

        $question->save();


        $question->terms()->attach($termid);

        return back()->with('success', 'Uloženo');
    }

    /**
     * Display the specified resource.
     *
     * @param  \App\question  $question
     * @return \Illuminate\Http\Response
     */
    public function show(question $question)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     *
     * @param  \App\question  $question
     * @return \Illuminate\Http\Response
     */
    public function edit(question $question)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @param  \App\question  $question
     * @return \Illuminate\Http\Response
     */
    public function update(Request $request, $id)
    {
        $this->validate($request, [
            'typ_otazky' => 'required|numeric',
            'zadani_otazky' => 'required',
            'max_pocet_bodu' => 'required|numeric|min:0',
            'min_pocet_bodu_pro_slozeni' => 'required|numeric|min:0'
        ]);

        if($request->input('max_pocet_bodu') < $request->input('min_pocet_bodu_pro_slozeni'))
        {
            return back()->with('error', 'Maximální počet bodů je menší než minimální počet bodů pro úspěšné složení zkoušky.');
        }

        $question = question::find($id);
        $question->typ_otazky = $request->input('typ_otazky');
        $question->zadani_otazky = $request->input('zadani_otazky');
        $question->max_pocet_bodu = $request->input('max_pocet_bodu');
        $question->min_pocet_bodu_pro_slozeni = $request->input('min_pocet_bodu_pro_slozeni');

        $question->save();

        return back()->with('success', 'Uloženo');
    }

    /**
     * Remove the specified resource from storage.
     *
     * @param  \App\question  $question
     * @return \Illuminate\Http\Response
     */
    public function destroy($question)
    {
        $queue = question::find($question);
        foreach ($queue->scores as $score) 
        {
            $score->delete();
        }
        $queue->delete();
        return back()->with('success', 'Smazáno');
    }

    public function editQuestions()
    {
        $questions = question::All();
        return view('editQuestins')->with('questions',$questions);
    }

    public function editTermQuestions($termid) {
        $term = term::All()->where('id', '=', $termid)->first();

        $questions = $term->questions;

        $all_questions = question::All();

        return view('editTermQuestions')->with('questions',$questions)->with('termid',$term->id)->with('all_questions', $all_questions);
    }

    public function addTerm($termid, $questionid)
    {
        $question = question::All()->where('id', '=', $questionid)->first();

        $question->terms()->attach($termid);

        return back()->with('success', 'Úspěšně přidáno do termínu');
    }

    public function termQuestionDestroy($termid, $questionid)
    {
        $question = question::All()->where('id', '=', $questionid)->first();

        $question->terms()->detach($termid);

        return back()->with('success', 'Úspěšně odstraněno z termínu');
    }
}
