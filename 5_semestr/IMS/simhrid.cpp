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

int lastHridel = -1;
int countOfHridels = 600;

int NLXLast = -1;
int KoepferLast = -1;
int DC3_1Last = -1;
int DC3_2Last = -1;
int OCDLast = -1;

Facility NLXbefore("NLX 1500/500 BF");
Facility NLX("NLX 1500/500");
Facility KOEPFERbefore("Koepfer 200 CNC BF");
Facility KOEPFER("Koepfer 200 CNC");
Facility DC3before("DC 3 BF");
Facility DC3("DC 3");
Facility BHOCDbefore("BH OCD 2040 BF");
Facility BHOCD("BH OCD 2040");

/*
* Proces popisujici pruchod hridele vyrobnim procesem
*/
class Hridel: public Process
{
public:
	int numberOfHridel;	
	//std::vector<Hridel> *SmeckaHrideli;
  	
    void wait_for_NLX()
    {
    	srand(time(NULL));
    	float rand_val = (float) rand()/RAND_MAX;
      	printf("%f\n", rand_val);
      	while (rand_val<=0.5)
		{
        	Wait(3.3);
      	}
    }
  	
  	void wait_for_KOEPFER()
    {
		//float rand_val = Random();
		srand(time(NULL));
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
          rand_val = Random();
        }
    }
  	
  	void wait_for_DC3()
    {
  		//float rand_val = Random();
  		srand(time(NULL));
  		float rand_val = (float) rand()/RAND_MAX;
  		printf("%f\n", rand_val);
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
          	rand_val = Random();
        }
    }

	void Behavior()
	{

      	///////////////////////////////////////////
      	//NLX
      	///////////////////////////////////////////
		printf("NLX: %d\n", numberOfHridel);
		Wait(1.8); // transport na NLX

		//while(NLXLast != (numberOfHridel - 1));
		//Print("Jsem za whilem: %i\n", numberOfHridel);
      	wait_for_NLX(); // proces cekani na stroj
		//Print("Jsem za waitforseize: %i\n", numberOfHridel);
		Seize(NLX); // zaberu a pracuju na NLX
		//printf("NLX(1): %d\n", numberOfHridel);
		wait_for_NLX();
		//NLXLast = numberOfHridel;
		Wait(7);
		//printf("NLX(2): %d\n", numberOfHridel);
		Release(NLX);
		//NLXLast = numberOfHridel;
        /*if (numberOfHridel != (countOfHridels - 1))
        {
        	(*SmeckaHrideli).at(numberOfHridel + 1)->Activate();
        }*/

		printf("Koepfer: %d\n", numberOfHridel);
      	///////////////////////////////////////////
      	//Koepfer
      	///////////////////////////////////////////
		Wait(0.7); // transport na Koepfer
		//while(KoepferLast != (numberOfHridel - 1));
        
		Seize(KOEPFER); // zaberu a pracuju na Koepfer
		wait_for_KOEPFER(); // proces cekani na stroj
		Wait(3.2);
		Release(KOEPFER);
		KoepferLast = numberOfHridel;

		printf("DC3(1): %d\n", numberOfHridel);
        ///////////////////////////////////////////
      	//DC3 poprve
      	///////////////////////////////////////////
		Wait(0.8); // transport na DC3
      	//while(DC3_1Last != (numberOfHridel - 1));
      	
		Seize(DC3); // zaberu a pracuju na DC3
		wait_for_DC3(); // proces cekani na stroj
		Wait(0.2);
		Release(DC3);
		DC3_1Last = numberOfHridel;
      
		Wait(3.2); // transport do skladu
		Wait(7200); // doba cementace TODO 120h * 24

		printf("OCD: %d\n", numberOfHridel);
      	///////////////////////////////////////////
      	//OCD
      	///////////////////////////////////////////
		Wait(2.5); // transport na OCD
      	//while(OCDLast != (numberOfHridel - 1));
		Seize(BHOCD); // zaberu a pracuju na OCD
		Wait(3.8);
		Release(BHOCD);
		OCDLast = numberOfHridel;
      	
		printf("DC3(2): %d\n", numberOfHridel);

      	///////////////////////////////////////////
      	//DC3 podruhe
      	///////////////////////////////////////////
		Wait(2.8); // transport na DC3
      	//while(DC3_2Last != (numberOfHridel - 1));
      	
		Seize(DC3); // zaberu a pracuju na DC3
		wait_for_DC3(); // proces cekani na stroj
		Wait(0.2);
		Release(DC3);
		DC3_2Last = numberOfHridel;
      
		Wait(0.9); // transport do skladu hotoveho
	}

};

int main()
{
	Init(0,10080); // 24 * 60 * 7 = 10080 = pocet minut v jednom tydni, 3 smenny provoz
	// potreba nejake aktivace generatoru nebo tak neco(new Gener)->Activate();
  	//(new Generator)->Activate();

	std::vector<Hridel*> SmeckaHrideli(countOfHridels);

  	for(int i = 0; i < SmeckaHrideli.size(); i++)
    {
    	SmeckaHrideli.at(i) = new Hridel;//-;>Activate();
      	//SmeckaHrideli.at(i) = new Hridel;
      
    	SmeckaHrideli.at(i)->numberOfHridel = i;
    	SmeckaHrideli.at(i)->Activate();
    	//SmeckaHrideli.at(i).SmeckaHrideli = &SmeckaHrideli;
    }
  	//zkativuj me
  	//SmeckaHrideli.at(0).Activate();
  
  	Run(); 
	Print("Dokoncil jsem simulaci, hurray!\n");
}