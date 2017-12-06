/**
 * Projekt IMS 2017/18 - Vyrobni proces z oblasti: strojirenske nebo zemedelske
 * Simulace vyroby hridele
 * 
 * xbucha02, Petr Buchal
 * xcervi21, Radim Cervinka
**/
#include <iostream>
#include "simlib.h"
#include <cstdbool>
#include <vector>
#include <random>
#include <time.h>


// 24 * 60 * 7 = 10080 = pocet minut v jednom tydni, 3 smenny provoz
// 10080 * 52 = 524160 = pocet minut v roce, 3 smenny nepretrzity provoz
// 262080
unsigned int delka_simulace_minuty = 262080;
unsigned int tydenni_prirustek_zbozi = 331;

unsigned int numOfActivated = 0;
unsigned int count = 0;

Facility NLXbefore("NLX 1500/500 BF");
Facility NLX("NLX 1500/500");
Facility KOEPFERbefore("Koepfer 200 CNC BF");
Facility KOEPFER("Koepfer 200 CNC");
Facility DC3before("DC 3 BF");
Facility DC3("DC 3");
Facility BHOCDbefore("BH OCD 2040 BF");
Facility BHOCD("BH OCD 2040");
Histogram Table ("Table", 0, 500, 100);


/*
* Proces popisujici pruchod hridele vyrobnim procesem
*/
class Hridel: public Process
{
public:
	
	void wait_for_NLX()
	{
		
		float rand_val = (float) rand()/RAND_MAX;

		while (rand_val<=0.5)
		{
			Wait(3.3);
			rand_val = (float) rand()/RAND_MAX;
		}
	}
	
	void wait_for_KOEPFER()
	{
		float rand_val = (float) rand()/RAND_MAX;
		while (rand_val <= 0.8)
		{
			if (rand_val <= 0.2)
			{
				Wait(1.1);
			}
			else if (rand_val <= 0.4)
			{
				Wait(1.8);
			}
			else if (rand_val <= 0.6)
			{
				Wait(2.3);
			}
			else
			{
				Wait(3.1);
			}
		  rand_val = (float) rand()/RAND_MAX;
		}
	}
	
	void wait_for_DC3()
	{
		//srand(time(NULL));
		float rand_val = (float) rand()/RAND_MAX;

		while (rand_val <= 0.97)
		{	
			// prevodovka still 29% z 97% = 28% ze 100%
			if (rand_val <= 0.28)
			{
				// 38 % z 28% = 11% ze 100%
				if (rand_val <= 0.11)
				{
					Wait(0.2);
				}

				// 62% z 28% = 17% ze 100%
				else {
					Wait(0.1);
				}
			}
			// prevodovka EJC 39% z 97% = 38% ze 100%
			else if (rand_val <= 0.66)
			{	
				// 37% z 38% = 14% ze 100%
				if (rand_val <= 0.42)
				{
					Wait(0.4);
				}

				// 27% z 38% = 10% ze 100%
				else if (rand_val <= 0.52)
				{
					Wait(0.8);
				}

				// 27% z 38% = 10% ze 100%
				else if (rand_val <= 0.62)
				{
					Wait(1);
				}

				// 9% z 38% = 4% ze 100%
				else
				{
					Wait(0.2);
				}
			}
			// spojka EZF (32 % z 97%) = 31% ze 100%
			else 
			{
				// 44% z 31% = 14% ze 100%
				if (rand_val <= 0.8)
				{
					Wait(0.3);
				}

				// 23% z 31% = 8% ze 100%
				else if (rand_val <= 0.88)
				{
					Wait(0.4);
				}

				// 11% z 31% = 3% ze 100%
				else if (rand_val <= 0.91)
				{
					Wait(0.5);
				}
				
				// 11% z 31% = 3% ze 100%
				else if (rand_val <= 0.94)
				{
					Wait(1.5);
				}
			  
				// 11% z 38% = 3% ze 100%
				else
				{
					Wait(1.6);
				}
			}
			rand_val = (float) rand()/RAND_MAX;
		}
	}

	void Behavior()
	{
		//total_time_spent = Time;

		Priority = 1; // TODO

		///////////////////////////////////////////
		//NLX
		///////////////////////////////////////////
		Wait(1.8); // transport na NLX

		Seize(NLX); // zaberu a pracuju na NLX

		wait_for_NLX();

		Wait(7);
		Release(NLX);

		///////////////////////////////////////////
		//Koepfer
		///////////////////////////////////////////
		Wait(0.7); // transport na Koepfer
		
		Seize(KOEPFER); // zaberu a pracuju na Koepfer

		wait_for_KOEPFER(); // proces cekani na stroj

		Wait(3.2);
		Release(KOEPFER);

		///////////////////////////////////////////
		//DC3 poprve
		///////////////////////////////////////////
		Wait(0.8); // transport na DC3

		Seize(DC3); // zaberu a pracuju na DC3

		wait_for_DC3(); // proces cekani na stroj

		Wait(0.2);
		Release(DC3);

	  
		Wait(3.2); // transport do skladu
		Wait(7200); // doba cementace TODO 120h * 24

		///////////////////////////////////////////
		//OCD
		///////////////////////////////////////////
		Wait(2.5); // transport na OCD

		Seize(BHOCD); // zaberu a pracuju na OCD
		Wait(3.8);
		Release(BHOCD);
		

		///////////////////////////////////////////
		//DC3 podruhe
		///////////////////////////////////////////
		Wait(2.8); // transport na DC3
		
		Priority = 2;
		Seize(DC3); // zaberu a pracuju na DC3

		wait_for_DC3(); // proces cekani na stroj

		Wait(0.2);
		Release(DC3);
	  
		Wait(0.9); // transport do skladu hotoveho

		count++;
		Table(Time);
	}

};



class Gener : public Event {
  void Behavior() {
	for (int i = 0; i < tydenni_prirustek_zbozi ;i++) {
		(new Hridel)->Activate();
		numOfActivated++;
	}

	Activate(Time+10080);
  }
};


int main()
{
	Init(0, delka_simulace_minuty);

	srand(time(NULL));

	(new Gener)->Activate();

	Run();
	//Print("asdf: %i\n", tydenni_prirustek_zbozi);
	//Print("numOfActivated = %i.\n", numOfActivated);
	//Print("Dokoncil jsem simulaci, hurray! Count = %i.\n", count);
	//Print("Za dobu simulace se nezpracovalo %i dovezenych vyrobku.\n", numOfActivated - count);
	//Print("V prumeru tydne produkuji %d vyrobku.\n", (count/52));

	//Print("%i %i\n", numOfActivated - count, count);

	//NLX.Output();
	//KOEPFER.Output();
	//DC3.Output();
	//BHOCD.Output();
	//Table.Output();
}

