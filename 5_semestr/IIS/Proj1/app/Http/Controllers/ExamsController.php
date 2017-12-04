<?php

namespace App\Http\Controllers;

use App\exam;
use App\score;
use App\subject;
use Illuminate\Http\Request;

class ExamsController extends Controller
{
    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     **
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
            'typ' => 'required|max:63',
            'max_pocet_bodu' => 'required|min:0',
            'min_pocet_bodu_pro_slozeni' => 'required|min:0',
            'pocet_terminu' => 'required|min:0',
            'max_pocet_terminu_pro_studenta' => 'required|min:1',
            'subject_id' => 'required'
        ]);

        $exam = new exam;
        $exam->typ = $request->input('typ');
        $exam->max_pocet_bodu = $request->input('max_pocet_bodu');
        $exam->min_pocet_bodu_pro_slozeni = $request->input('min_pocet_bodu_pro_slozeni');
        $exam->pocet_terminu = $request->input('pocet_terminu');
        $exam->max_pocet_terminu_pro_studenta = $request->input('max_pocet_terminu_pro_studenta');
        $exam->subject_id = $request->input('subject_id');
        $exam->save();

        return redirect('zkouskyvyucujici')->with('success', 'Uloženo');
    }

    /**
     * Display the specified resource.
     *
     * @param  \App\exam  $exam
     * @return \Illuminate\Http\Response
     */
    public function show(exam $exam)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     *
     * @param  \App\exam  $exam
     * @return \Illuminate\Http\Response
     */
    public function edit(exam $exam)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @param  \App\exam  $exam
     * @return \Illuminate\Http\Response
     */
    public function update(Request $request, $id)
    {
        $this->validate($request, [
            'typ' => 'required|max:63',
            'max_pocet_bodu' => 'required|min:0',
            'min_pocet_bodu_pro_slozeni' => 'required|min:0',
            'pocet_terminu' => 'required|min:0',
            'max_pocet_terminu_pro_studenta' => 'required|min:1',
            'subject_id' => 'required'
        ]);

        $exam = exam::find($id);
        $exam->typ = $request->input('typ');
        $exam->max_pocet_bodu = $request->input('max_pocet_bodu');
        $exam->min_pocet_bodu_pro_slozeni = $request->input('min_pocet_bodu_pro_slozeni');
        $exam->pocet_terminu = $request->input('pocet_terminu');
        $exam->max_pocet_terminu_pro_studenta = $request->input('max_pocet_terminu_pro_studenta');
        $exam->subject_id = $request->input('subject_id');
        $exam->save();

        return redirect('zkouskyvyucujici')->with('success', 'Uloženo');
    }

    /**
     * Remove the specified resource from storage.
     *
     * @param  \App\exam  $exam
     * @return \Illuminate\Http\Response
     */
    public function destroy($id)
    {
        $exam = exam::find($id);
        $scores = score::All();

        foreach ($exam->terms as $term) 
        {
            foreach ($term->questions as $question) 
            {
                foreach ($question->scores as $score) 
                {
                    if ($score->term_id == $term->id) 
                    {
                        $score->delete();
                    }
                }
            }
            $term->delete();
        }
        $exam->delete();

        return back()->with('success', 'Smazáno');  
    }

    public function addTermExam($subjectid) {

        $subject = subject::All()->where('id', '=', $subjectid)->first();

        return view('teacher.CreateExam', compact('subject'));

    }

    public function editExam($id) {
        $exam = exam::Find($id);
        return view('editExam')->with('exam',$exam);
    }

}
