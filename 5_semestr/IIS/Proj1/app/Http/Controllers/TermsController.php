<?php

namespace App\Http\Controllers;

use App\term;
use App\score;
use App\User;
use App\exam;
use Carbon\Carbon;
use Illuminate\Database\Eloquent\Collection;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Redirect;

class TermsController extends Controller
{
    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {
        // /terms
    }

    /**
     * Show the form for creating a new resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function create()
    {
        // /terms/create
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
            'datum_cas' => 'required',
            'misto' => 'required',
            'zacatek_prihlasovani' => 'required',
            'konec_prihlasovani' => 'required',
            'max_pocet_studentu' => 'required|min:0',
            'cislo_terminu' => 'required|min:0',
            'exam_id' => 'required'
        ]);
        $terms = term::All();
        $newterm = new term;
        $newterm->datum_cas = $request->input('datum_cas');
        $newterm->misto = $request->input('misto');
        $newterm->zacatek_prihlasovani = $request->input('zacatek_prihlasovani');
        $newterm->konec_prihlasovani = $request->input('konec_prihlasovani');
        $newterm->max_pocet_studentu = $request->input('max_pocet_studentu');
        
        foreach ($terms as $term)
        {
            if ($term->exam->id == $request->input('exam_id')) 
            {
                if ($term->exam->pocet_terminu < $request->input('cislo_terminu')) 
                {
                    return back()->with('error', 'Nevalidní číslo termínu');
                }
            }
        }

        foreach ($terms as $term)
        {
            if ($term->exam->id == $request->input('exam_id'))
            {
                foreach ($term->exam->terms as $term2)
                {
                    if ($term2->cislo_terminu == $request->input('cislo_terminu'))
                    {
                        return back()->with('error', 'Nevalidní číslo termínu');
                    }
                }
            }
        }

        $newterm->cislo_terminu = $request->input('cislo_terminu');
        $newterm->exam_id = $request->input('exam_id');
        $newterm->save();

        return redirect('zkouskyvyucujici')->with('success', 'Uloženo');
    }

    /**
     * Display the specified resource.
     *
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function show($id)
    {
        // GET /tasks/{id}
    }

    /**
     * Show the form for editing the specified resource.
     *
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function edit($id)
    {
        // GET /tasks/{id}/edit
    }

    /**
     * Update the specified resource in storage.
     *
     * @param  \Illuminate\Http\Request  $request
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function update(Request $request, $id)
    {
        $this->validate($request, [
            'datum_cas' => 'required',
            'misto' => 'required',
            'zacatek_prihlasovani' => 'required',
            'konec_prihlasovani' => 'required',
            'max_pocet_studentu' => 'required|min:0',
            'cislo_terminu' => 'required|min:0',
            'exam_id' => 'required'
        ]);

        $terms = term::All();
        $newterm = term::Find($id);
        
        $newterm->datum_cas = $request->input('datum_cas');
        $newterm->misto = $request->input('misto');
        $newterm->zacatek_prihlasovani = $request->input('zacatek_prihlasovani');
        $newterm->konec_prihlasovani = $request->input('konec_prihlasovani');
        $newterm->max_pocet_studentu = $request->input('max_pocet_studentu');

        foreach ($terms as $term)
        {
            if ($term->exam->id == $request->input('exam_id')) 
            {
                if ($term->exam->pocet_terminu < $request->input('cislo_terminu')) 
                {
                    return back()->with('error', 'Nevalidní číslo termínu');
                }
            }
        }

        foreach ($terms as $term)
        {
            if ($term->exam->id == $request->input('exam_id'))
            {
                foreach ($term->exam->terms as $term2)
                {
                    if ($term2->cislo_terminu == $request->input('cislo_terminu'))
                    {
                        return back()->with('error', 'Nevalidní číslo termínu');
                    }
                }
            }
        }

        $newterm->cislo_terminu = $request->input('cislo_terminu');
        $newterm->exam_id = $request->input('exam_id');
        $newterm->save();

        return redirect('zkouskyvyucujici')->with('success', 'Uloženo');
    }

    /**
     * Remove the specified resource from storage.
     *
     * @param  int  $id
     * @return \Illuminate\Http\Response
     */
    public function destroy($id)
    {
        $term = term::find($id);
        $scores = score::All();
        foreach ($scores as $score) 
        {
            if ($score->termid == $id) 
            {
                $score->delete();
            }
        }
        $term->delete();
        return back()->with('success', 'Smazáno');  
    }

    public function addTerm($examid)
    {
        $terms = term::All();
        return view('addTermin')->with('term',$terms)->with('examid', $examid);
    }

    public function editTerm($termid)
    {
        $term = term::Find($termid);
        return view('editTerm')->with('term',$term);
    }

    public function terminystudent()
    {
        $date =  Carbon::now()->toDateString();

        $datetime =  Carbon::now()->toDateTimeString();

        $terms = new Collection();

        foreach(auth()->user()->position->subjects as $subject) {
            foreach($subject->exams as $exam) {
                foreach ($exam->terms as $term) {
                    $terms->push($term);
                }
            }
        }

        $terms->sortBy('konec_prihlasovani')->where('datum_cas', '>=', $datetime);


        return view('student.terminystudent', compact('terms', 'date'));
    }

    public function terminystudentprihlasit($term_id, $student_id)
    {
        $term_exam = 0;

        $term = term::All()->where('id', '=', $term_id)->first();

        if(count($term->students) == $term->max_pocet_studentu) {
            return Redirect::back()->withErrors(['Již bylo dosaženo maximálního počtu přihlášených uživatelů']);
        }

        auth()->user()->position->terms()->attach($term_id);

        foreach(exam::All() as $exam) {
            foreach(auth()->user()->position->terms as $term) {
                if($term->exam_id == $exam->id) {
                    $term_exam++;
                    if($term_exam > $exam->max_pocet_terminu_pro_studenta) {
                        auth()->user()->position->terms()->detach($term_id);
                        return Redirect::back()->withErrors(['Nemůžete se přihlásit na více termínů']);
                    }
                }

            }
            $term_exam = 0;
        }

        return back()->with('success', 'Úspěšně přihlášen');
    }


    public function terminystudentodhlasit($term_id, $student_id)
    {
        auth()->user()->position->terms()->detach($term_id);

        return back()->with('success', 'Úspěšně odhlášen');
    }


}
