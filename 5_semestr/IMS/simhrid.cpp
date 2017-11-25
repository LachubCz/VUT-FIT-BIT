/**
 * Projekt IMS 2017/18 - Vyrobni proces z oblasti: strojirenske nebo zemedelske
 * Simulace vyroby hridele
 * 
 * xbucha02, Petr Buchal
 * xcervi21, Radim Cervinka
**/
#include <iostream>
#include "simlib.h"

Facility NLX("NLX 1500/500");
Facility KOEPFER("Koepfer 200 CNC");
Facility DC3("DC 3");
Facility BHOCD("BH OCD 2040");

/*
* Proces popisujici pruchod hridele vyrobnim procesem
*/
class Hridel: public Process {
public:
	void Behavior() {
		Wait(1.8); // transport na NLX
		Seize(NLX); // zaberu a pracuju na NLX
		Wait(7);
		Release(NLX);

		Wait(0.7); // transport na Koepfer
		Seize(KOEPFER); // zaberu a pracuju na Koepfer
		Wait(3.2);
		Release(KOEPFER);

		Wait(0.8); // transport na DC3
		Seize(DC3); // zaberu a pracuju na DC3
		Wait(0.2);
		Release(DC3);

		Wait(3.2); // transport do skladu
		Wait(); // doba cementace

		Wait(2.5); // transport na OCD
		Seize(BHOCD); // zaberu a pracuju na OCD
		Wait(3.8);
		Release(BHOCD);

		Wait(2.8); // transport na DC3
		Seize(DC3); // zaberu a pracuju na DC3
		Wait(0.2);
		Release(DC3);

		Wait(0.9); // transport do skladu hotoveho
	}
}

/*
* Pokus o vytvoreni procesu, ktery modeluje chovani a zabirani stroje NLX
*/
class NLX : public Process {
public:
	void Behavior() {
		while (1) {
			if (Random()<=0.5) {
				Seize(NLX);
				Wait(3.3);
				Release(NLX);
			}
		}
		

	}
}

int main()
{
	Init(0,10080); // 24 * 60 * 7 = 10080 = pocet minut v jednom tydni, 3 smenny provoz
	// potreba nejake aktivace generatoru nebo tak neco(new Gener)->Activate();
	Run(); 
	Print("Dokoncil jsem simulaci, hurray!\n");
}
