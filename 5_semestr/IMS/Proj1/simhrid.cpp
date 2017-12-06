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
// 10080 * 26 = 262080 = pocet minut v pul roce, 3 smenny provoz
unsigned int delka_simulace_minuty = 524160;
unsigned int tydenni_prirustek_zbozi = 320;

unsigned int numOfActivated = 0; // celkovy pocet dovezenych kusu materialu
unsigned int count = 0; // celkovy pocet vyrobenych hrideli

Facility NLX("NLX 1500/500");
Facility KOEPFER("Koepfer 200 CNC");
Facility DC3("DC 3");
Facility BHOCD("BH OCD 2040");

Histogram Table ("Table", 0, 500, 100);


/*
* Proces popisujici pruchod hridele vyrobnim procesem
*/
class Hridel: public Process
{
public:
	
	// funkce simulujici cekani na stroj NLX 1500/500, nez obslouzi jine vyrobky tovarny
	void wait_for_NLX()
	{
		float rand_val = (float) rand()/RAND_MAX;

		while (rand_val<=0.5)
		{
			Wait(3.3);
			rand_val = (float) rand()/RAND_MAX;
		}
	}
	
	// funkce simulujici cekani na stroj Koepfer 200 CNC, nez obslouzi jine vyrobky tovarny
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
	
	// funkce simulujici cekani na stroj DC 3, nez obslouzi jine vyrobky tovarny
	void wait_for_DC3()
	{
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
		Priority = 1; // nastaveni zakladni priority fronty

		srand(time(NULL));

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

	  
		Wait(3.2); // transport na cementaci
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
		
		Priority = 2; // nastaveni vyssi priority fronty pro tento proces (zajisteni prednosti na stroji DC 3 pred procesy, ktere jej chteji pouzit poprve)
		Seize(DC3); // zaberu a pracuju na DC3

		wait_for_DC3(); // proces cekani na stroj

		Wait(0.2);
		Release(DC3);
	  
		Wait(0.9); // transport do skladu hotovych vyrobku

		count++; // inkrementujeme pocet vyrobenych hrideli
		Table(Time);
	}

};



class Gener : public Event {
  void Behavior() {
  	// dovezl se urcity pocet materialu, vsechny posleme do vyroby
	for (int i = 0; i < tydenni_prirustek_zbozi ;i++) {
		(new Hridel)->Activate();
		numOfActivated++; // inkrementujeme pocet dovezenych kusu materialu
	}
	
	// dovoz 1x za tyden
	Activate(Time+10080);
  }
};


int main()
{
	Init(0, delka_simulace_minuty);

	srand(time(NULL));

	(new Gener)->Activate();

	Run();
	
	Print("%i\t- pocet tydne dovezenych kusu materialu\n", tydenni_prirustek_zbozi);
	Print("%i\t- celkovy pocet dovezenych kusu materialu\n", numOfActivated);
	Print("%i\t- celkovy pocet vyrobenych hrideli\n", count);
	Print("%i\t- pocet nezpracovanych dovezenych kusu materialu\n", numOfActivated - count);
	Print("%d\t- prumerny pocet vyrobenych hrideli za tyden\n", (count/52));

	//NLX.Output();
	//KOEPFER.Output();
	//DC3.Output();
	//BHOCD.Output();
	//Table.Output();
}
