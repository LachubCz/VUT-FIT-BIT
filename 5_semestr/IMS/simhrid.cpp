/**
 * Projekt IMS 2017/18 - Vyrobni proces z oblasti: strojirenske nebo zemedelske
 * Simulace vyroby hridele
 * 
 * xbucha02, Petr Buchal
 * xcervi21, Radim Cervinka
**/
#include <iostream>
#include <queue>        // priority_queue
#include <cstdlib>      // exit()
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// simulátor
/////////////////////////////////////////////////////////////////////////////

void error(const char *msg) {
    cerr << "ERROR: " << msg << endl;
    exit(1);
}

typedef void (*event_ptr_t) (); // ukazatel na funkci popisující událost

// Aktivační záznam události (pro jednoduchost bez priorit)
struct act_record {
    event_ptr_t event_ptr;      // odkaz na událost
    double atime;               // aktivační čas
    // konstruktor naplní položky:
    act_record(event_ptr_t e, double at): event_ptr(e), atime(at) {}
};

// Porovnání aktivačních záznamů z hlediska jejich pořadí v kalendáři.
// Je nutné pro uspořádání kalendáře typu priority_queue<act_record>
bool operator <(const act_record & a, const act_record & b)
{
    return a.atime > b.atime;  // POZOR HACK 
    // porovnání je obráceně -- lze opravit, ale 
    // musí se použít jiné než implicitní řazení u priority_queue
    // (menší čas ==> vyšší priorita při řazení)
}

// Jednoduchý kalendář událostí 
// - na začátku je vždy aktivační záznam s nejmenším časem
priority_queue<act_record> calendar;

// Modelový čas
double Time;

// Plánování události na zadaný čas
void schedule(event_ptr_t event_ptr, double at)
{
    if (at < Time)
        error("Plánování do minulosti");
    act_record a(event_ptr, at);
    calendar.push(a);           // zařazení do kalendáře
}

/////////////////////////////////////////////////////////////////////////////
// popis modelu
/////////////////////////////////////////////////////////////////////////////

queue<event_ptr_t>  fronta;
bool                zarizeni_obsazeno = false;

void event4()
{
    // popis činnosti
    cout << "event 4 - konec obsluhy";
    zarizeni_obsazeno = false;
    if (!fronta.empty()) {
	cout << " + aktivace prvního ve frontě";
	schedule(fronta.front(), Time);	// aktivace první události z fronty
	fronta.pop();		// vyjme z fronty
    }
    cout << endl;
}

void event3()
{
    // popis činnosti
    cout << "event 3 - start obsluhy" << endl;
    zarizeni_obsazeno = true;
    schedule(event4, Time + 250);	// doba obsluhy
}

void event2()
{
    // popis činnosti
    if (zarizeni_obsazeno) {
	cout << "event 2 - zařazení do fronty" << endl;
	fronta.push(event3);	// zařadíme do fronty
    } else {
	cout << "event 2" << endl;
	schedule(event3, Time);	// aktivace obsluhy
    }
}

void event1()
{
    // popis činnosti
    cout << "event 1" << endl;
    schedule(event2, Time);	// aktivace event2
    schedule(event1, Time + 200);	// další výskyt
}

/////////////////////////////////////////////////////////////////////////////
// popis simulace
/////////////////////////////////////////////////////////////////////////////

const double TSTART = 0;
const double TEND = 1000;

int main()
{
    cout << "*** inicializace" << endl;
    // while(!calendar.empty()) calendar.pop(); // inicializace kalendáře
    Time = 0;
    cout << "Time = " << Time << endl;
    schedule(event1, Time);	// první aktivace
    cout << "*** začátek simulace:  čas = " << Time << endl;
    while (!calendar.empty()) {
	act_record a = calendar.top();	// přečteme první záznam
	calendar.pop();		// odstraníme první záznam
        if (a.atime > TEND) {
            Time = TEND;
            break;              // končíme simulaci
        }
	Time = a.atime;		// posuneme čas
	cout << "[Time = " << Time << "]: ";
	a.event_ptr();		// provedeme událost
    }
    cout << "*** konec simulace:  čas = " << Time << endl;
    cout << "aktuální délka fronty = " << fronta.size() << endl;
}
