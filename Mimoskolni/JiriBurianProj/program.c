#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int otazka (char *line)
{
  int i = 0, DelkaSlova=0;
  long int cislo = 0;

  for (i = 0; 1; ++i)
  {
  	if (line[i]==41)
  	{
  		break;
  	}
  	if (line[i]==13)
  	{
  		break;
  	}
  	if (line[i]<58 && line[i]>47)
  	{
  		DelkaSlova++;
  	}
  	else
  	{
  		DelkaSlova=0;
  		break;
  	}
  }

  i = 0;
  while(i < DelkaSlova)
  {
    cislo = cislo * 10;
    cislo = cislo + (((int) line[i]) - 48);

    i++;
  }

  return cislo;
}



int pocetradku (char *filename)
{
	FILE* myfile = fopen(filename, "r");
	int ch, number_of_lines = 0;

	do 
	{
	    ch = fgetc(myfile);
	    if(ch == '\n')
	    	number_of_lines++;
	} while (ch != EOF);

	if(ch != '\n' && number_of_lines != 0) 
	    number_of_lines++;

	fclose(myfile);
	return number_of_lines;
}



char vyberodpovedi (char pismeno)
{
	switch(pismeno)
	{
	case 'a':
	case 'A':
	return '0';

	case 'b':
	case 'B':
	return '1';

	case 'c':
	case 'C':
	return '2';

	default:
	return 'e';
	}
}



void opravacislovani (char *filename)
{
	int ocekavanecislo = 1;
	char line[100];
	FILE *soubor = fopen(filename,"r");
	FILE *temp = fopen("temp2.txt", "w");

	while(1)
	{
		fgets(line, 99, soubor);
		if (line[0]=='e')
		{
			fprintf(temp, "%s", line);
			break;
		}
		if (otazka(line) == 0 || otazka(line) == ocekavanecislo)
		{
			fprintf(temp, "%s", line);
			if (otazka(line) == ocekavanecislo)
			{
				ocekavanecislo++;
			}
		}
		else
		{
			line[0]--;
			fprintf(temp, "%s", line);
			ocekavanecislo++;
		}
	}

	fclose(temp);
	fclose(soubor);
}



void odebrat (char *filename)
{
	int pocetotazek, cislootazky, aktualniotazka, pocetodpovedi, cisloradku=0, pomazani=0;
	char line[100], prezdchoziradek[100];
	FILE *soubor = fopen(filename,"r");
	FILE *temp = fopen("temp.txt", "w");
	printf("###########################################################\n");
	fscanf(soubor, "%i", &pocetotazek);
	pocetotazek--;
	fprintf(temp, "%d", pocetotazek);

	printf("Vlozte cislo otazky, kterou chcete smazat: ");
	scanf ("%d", &cislootazky);

	while(1)
	{
		if (cisloradku!=0)
		{
			strcpy(prezdchoziradek,line);
		}
		fgets(line, 99, soubor);
		aktualniotazka=otazka(line);
		if (cislootazky == aktualniotazka)
		{
			pocetodpovedi=atoi(prezdchoziradek);
			for (int i = 0; i < pocetodpovedi; ++i)
			{
				fgets(line, 99, soubor);
				cisloradku++;
				pomazani=1;
			}
		}
		else
		{
			if (pomazani==1)
			{
				pomazani=0;
			}
			else
			{
				if (cisloradku!=0)
				{
					fprintf(temp, "%s", prezdchoziradek);
				}
			}
		}
		if (line[0] == 'e')
		{
			fprintf(temp, "%s", line);
			break;
		}
		cisloradku++;
	}
	while ( getchar() != '\n' );
	fclose(temp);
	fclose(soubor);
}

void pridat (char *filename)
{
	int pocetotazek, pocetodpovedi, i;
	char line[100];
	FILE *soubor = fopen(filename,"r");
	FILE *temp = fopen("temp.txt", "w");
	printf("###########################################################\n");
	fscanf(soubor, "%i", &pocetotazek);
	pocetotazek++;

	int radky=pocetradku(filename);

	fprintf(temp, "%d\n", pocetotazek);
	fgets(line, 99, soubor);
	for (int i = 0; i < (radky-2); ++i)
	{
		fgets(line, 99, soubor);
		fprintf(temp, "%s", line);
	}

	printf("Vlozte pocet odpovedi: ");
	scanf ("%d", &pocetodpovedi);
	if (pocetodpovedi < 3)
	{
		pocetodpovedi = 3;
	}
	fprintf(temp, "%d\n", pocetodpovedi);

	for (int i = 0; i < 99; ++i)
	{
		line[i]=0;
	}
	
	printf("Vlozte zneni otazky: ");
	while ( getchar() != '\n' );
	i=0;
	while(1)
	{
		line[i] = getchar();
    	if (line[i] == '\n')
    		break;
    	i++;
	}
	fprintf(temp, "%d) %s", pocetotazek, line);


	for (int i = 0; i < 99; ++i)
	{
		line[i]=0;
	}
	printf("Vlozte spravnou odpoved: ");

	i=0;
	while(1)
	{
		line[i] = getchar();
    	if (line[i] == '\n')
    		break;
    	i++;
	}

	fprintf(temp, ") %s", line);


	for (int x = 0; x < (pocetodpovedi-1); ++x)
	{

		for (int i = 0; i < 99; ++i)
		{
			line[i]=0;
		}

		printf("Vlozte spatnou odpoved: ");
		i=0;
		while(1)
		{
			line[i] = getchar();
	    	if (line[i] == '\n')
	    	break;
	    	i++;
		}

		fprintf(temp, ") %s", line);
	}
	fprintf(temp, "end" );
	fclose(temp);
	fclose(soubor);
}


void ukazat (char *filename)
{
	int pocetotazek, odpoved=0;
	char line[100], znak='a';

	FILE *soubor = fopen(filename,"r");
	fscanf(soubor, "%i", &pocetotazek);
	printf("###########################################################\n");
	printf("Pocet otazek v testu: %d\n", pocetotazek);
	printf("Spravna odpoved je vzdy A.\n");
	fgets(line, 99, soubor);

	while (1)
	{
		fgets(line, 99, soubor);
		if (line[0] == 'e')
		{
			break;
		}
		if (line[0] == ')')
		{
			printf("%c%s",znak, line);
			znak++;
		}
		else
		{
			if (odpoved == 0)
			{
				printf("\nPocet odpovedi: %s", line);
				odpoved=1;
			}
			else
			{
				printf("%s", line);
				znak='a';
				odpoved=0;
			}
		}
	}
	fclose(soubor);
}




void test (char *filename)
{
	FILE *soubor = fopen(filename,"r");
	int pocetotazek, pocetodpovedi, odpoved = 0, nahodnost=0, vytisknutychodpovedi=0, zkoumanaodpoved=0, spravne=0;
	char line[100], spravodp='0', znak='a', spravnaodpoved[100], volba;

	fscanf(soubor, "%i", &pocetotazek);
	printf("###########################################################");
	fgets(line, 99, soubor);
	fscanf(soubor, "%i", &pocetodpovedi);

	while (1)
	{
		fgets(line, 99, soubor);
		if (line[0] == ')')
			if (odpoved==0)
			{
				odpoved=1;
				strcpy(spravnaodpoved,line);
			}
			else
				odpoved=1;
		else
		{
			if (odpoved==1)
			{
				odpoved=0;
				znak='a';
				pocetodpovedi=line[0]-'0';

				volba=getchar();
				while('\n'!=getchar());
				volba=vyberodpovedi(volba);

				if (volba == spravodp)
				{
					printf("Spravne!\n");
					spravne++;
				}
				else
					printf("Spatne!\n");

				spravodp='0';
				vytisknutychodpovedi=0;
				zkoumanaodpoved=0;
				nahodnost=0;

				continue;
			}
			else
				odpoved=0;
		}
		if (line[0] == 'e')
		{
			break;
		}
		if (odpoved==1)
		{
			if (vytisknutychodpovedi>=3)
			{
				continue;
			}
			else
			{
				if(nahodnost==0 && (pocetodpovedi-3)>0)
				{
					switch (pocetodpovedi)
					{
						case 4:{
						nahodnost = (rand() % 2)*(rand() % 2);break;}
						case 5:{
						nahodnost = (rand() % 2)*(rand() % 2);break;}
						case 6:{
						nahodnost = (rand() % 2)*(rand() % 2);break;}
						case 7:{
						nahodnost = (rand() % 2)*(rand() % 2)*(rand() % 2);break;}
						case 8:{
						nahodnost = (rand() % 2)*(rand() % 2)*(rand() % 2)*(rand() % 2);break;}
					}
					if (nahodnost==1)
					{
						printf("%c%s",znak, spravnaodpoved);
						znak++;
						pocetodpovedi--;
						vytisknutychodpovedi++;
						zkoumanaodpoved++;
					}
					else
					{
						if (vytisknutychodpovedi==2)
						{
							printf("%c%s",znak, spravnaodpoved);
							znak++;
							pocetodpovedi--;
							vytisknutychodpovedi++;
							zkoumanaodpoved++;
						}
						if (zkoumanaodpoved>0 && vytisknutychodpovedi!=3)
						{
							printf("%c%s",znak, line);
							znak++;
							pocetodpovedi--;
							vytisknutychodpovedi++;
							zkoumanaodpoved++;
							spravodp++;
						}
						else
						{
							pocetodpovedi--;
							zkoumanaodpoved++;
						}
					}
				}
				else
				{
					if (nahodnost==1 && (pocetodpovedi-3)>0)
					{
						if ((rand() % 2))
						{
							printf("%c%s",znak, line);
							znak++;
							pocetodpovedi--;
							vytisknutychodpovedi++;
							zkoumanaodpoved++;
						}
						else
						{
							pocetodpovedi--;
							zkoumanaodpoved++;
						}
					}
					else
					{
						if (nahodnost==1 && (pocetodpovedi-3)<=0)
						{
							printf("%c%s",znak, line);
							znak++;
							pocetodpovedi--;
							vytisknutychodpovedi++;
							zkoumanaodpoved++;
						}
					}
					if (nahodnost==0 && vytisknutychodpovedi==2)
					{
						printf("%c%s",znak, spravnaodpoved);
						znak++;
						pocetodpovedi--;
						vytisknutychodpovedi++;
						zkoumanaodpoved++;
						nahodnost=1;
					}
					if (nahodnost==0 && vytisknutychodpovedi==0 && pocetodpovedi==3 && zkoumanaodpoved!=1)
					{
						printf("%c%s",znak, spravnaodpoved);
						znak++;
						pocetodpovedi--;
						vytisknutychodpovedi++;
						zkoumanaodpoved++;
						nahodnost=1;
					}
					if (nahodnost==0 && vytisknutychodpovedi==0 && pocetodpovedi==3 && zkoumanaodpoved== 1)
					{
						printf("%c%s",znak, line);
						znak++;
						pocetodpovedi--;
						spravodp++;
						vytisknutychodpovedi++;
						zkoumanaodpoved++;
					}
					if (nahodnost==0 && vytisknutychodpovedi==1 && pocetodpovedi==2 && zkoumanaodpoved== 2)
					{
						printf("%c%s",znak, spravnaodpoved);
						znak++;
						pocetodpovedi--;
						vytisknutychodpovedi++;
						zkoumanaodpoved++;
						nahodnost=1;
					}
					if (nahodnost==0 && vytisknutychodpovedi!=2)
					{
						printf("%c%s",znak, line);
						znak++;
						pocetodpovedi--;
						vytisknutychodpovedi++;
						zkoumanaodpoved++;
						spravodp++;
					}
				}
			}
		}
		else
			printf("%s", line);
	}
	printf("Meli jste spravne %d/%d otazek, vase uspesnost je %.2f!\n", spravne,pocetotazek,((float)spravne/pocetotazek));
	fclose(soubor);
}




int main(void)
{
	char volba, edit, x, vyber;

	printf("\n##Vzdelavaci hra##\n\n\n");
	
do{
	printf("MENU\n\n(N)-Nova hra\n(E)-Editace\n(K)-Konec\n\n");

	volba=getchar();
    while('\n'!=getchar());

	switch (volba)
	{	case 'n':
		case 'N':
			do {
					printf("###########################################################\n");
					printf("(M) - Matematika; (C) - Cestina; (Z) - Zemepis; (K) - Konec\n\n");
					
					vyber=getchar();
					while('\n'!=getchar());
			
					switch (vyber)
					{ case 'M':
					  case 'm': 
					  			test("matika.txt");
								break;

					  case 'C':
				      case 'c': {	
				      			test("cestina.txt");
				      			}
								break;
					  case 'Z':
					  case 'z': {
				      			test("zemepis.txt");
				      			}
								break;
					  case 'K':
					  case 'k': 
					  printf("###########################################################\n");
					  break;
					
					  default: printf("\n\aChybne zadana volba!\n");
					}

				} while (vyber!='K'&&vyber!='k');
					break;
		case 'e':
		case 'E': 
				do {
					printf("###########################################################\n");
					printf("(M) - Matematika; (C) - Cestina; (Z) - Zemepis; (K) - Konec\n\n");
					
					edit=getchar();
					while('\n'!=getchar());
			
					switch (edit)
					{ case 'M':
					  case 'm': do {
					  			
					  			printf("###########################################################\n");
								printf("(U) - Ukazat; (P) - Pridat; (O) - Odebrat; (Z) - Zpet\n\n");
								
								x=getchar();
								while('\n'!=getchar());
								
								switch (x)	{
		
									case 'U':
									case 'u':
												ukazat("matika.txt");
												break;

									case 'P':
									case 'p':
												pridat("matika.txt");
												remove("matika.txt");
												rename("temp.txt", "matika.txt");
												break;

									case 'O':
									case 'o':
												odebrat("matika.txt");
												opravacislovani("temp.txt");
												remove("matika.txt");
												remove("temp.txt");
												rename("temp2.txt", "matika.txt");
												break;
			
									case 'Z':
									case 'z': 	break;

									default: printf("\n\aChybne zadana volba!\n");
				 }
			    } while (x!='Z' && x!='z'); 
								break;

					  case 'C':
				      case 'c': do {	
				      			
				      			printf("###########################################################\n");
								printf("(U) - Ukazat; (P) - Pridat; (O) - Odebrat; (Z) - Zpet\n\n");

								x=getchar();
								while('\n'!=getchar());

									switch (x)
									{	
										case 'U':
										case 'u':
													ukazat("cestina.txt");
													break;

										case 'P':
										case 'p':
													pridat("cestina.txt");
													remove("cestina.txt");
													rename("temp.txt", "cestina.txt");
													break;

										case 'O':
										case 'o':
													odebrat("cestina.txt");
													opravacislovani("temp.txt");
													remove("cestina.txt");
													remove("temp.txt");
													rename("temp2.txt", "cestina.txt");
													break;
			
										case 'Z':
										case 'z': 	break;

										default: printf("\n\aChybne zadana volba!\n");
									}
								} while (x!='Z'&&x!='z'); 
								break;

					  case 'Z':
					  case 'z': do {
					  			
					  			printf("###########################################################\n");
								printf("(U) - Ukazat; (P) - Pridat; (O) - Odebrat; (Z) - Zpet\n\n");
								
								x=getchar();
								while('\n'!=getchar());

								switch (x)	{
		
									case 'U':
									case 'u':
												ukazat("zemepis.txt");
												break;

									case 'P':
									case 'p':
												pridat("zemepis.txt");
												remove("zemepis.txt");
												rename("temp.txt", "zemepis.txt");
												break;

									case 'O':
									case 'o':
												odebrat("zemepis.txt");
												opravacislovani("temp.txt");
												remove("zemepis.txt");
												remove("temp.txt");
												rename("temp2.txt", "zemepis.txt");
												break;
			
									case 'Z':
									case 'z': 	break;

									default: printf("\n\aChybne zadana volba!\n");
								}
						} while (x!='Z'&&x!='z'); 
								break;

					  case 'K':
					  case 'k': 
					  printf("###########################################################\n");
					  break;
					
					  default: printf("\n\aChybne zadana volba!\n");
					}

				} while (edit!='K'&&edit!='k');
				break;

		case 'k':
		case 'K': break;

		default: printf("\n\aChybne zadana volba!\n");
printf("###########################################################\n");
	}
}while (volba!='k'&&volba!='K');


	return 0;
}