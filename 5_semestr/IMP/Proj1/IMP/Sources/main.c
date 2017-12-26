/******************************************************************************/
/*                                                                            */
/* Projekt z predmetu IMP                                                     */
/*                                                                            */
/* ARM/FITkit3: Aplikace modulu Watchdog Timer (WDOG)                         */
/*                                                                            */
/* Reseni vytvoril(a) a odevzdava: (xbucha02, Petr Buchal)                    */
/*                                                                            */
/* Prevzal jsem kod demo aplikace FITkitu z informacniho systemu,             */
/* ponechal inicializacni cast a naprogramoval funkcionalitu WDOGu.           */
/* O demostraci modulu watchdogu jsem se zaslouzil 100 % a kod je original.   */
/* Z puvodniho kodu demo aplikace zustalo cca 30%.   		                  */
/*                                                                            */
/* Datum posledni zmeny: 26.12.2017                                           */
/*                                                                            */
/******************************************************************************/

#include "MK60D10.h"
#include <stdio.h>
#include <string.h>

/*******    Parametry aplikace    *******/
//nastavuji se celkem tri hodnoty, ktere ovlinuji chod aplikace
//pri vyberu kazde hodoty nehodici zakomentujte a vybranou odkomentujte
//pokud pracujete s okenkovym rezimem velikost periody musi byt vetsi nez velikost okna

/***    Vyber modu    ***/
int wdog_stctrlh = 0b0101;    //periodicky rezim
//int wdog_stctrlh = 0b1101;    //okenkovy rezim

/***    Velikost periody    ***/
int wdog_tovalh = 0; int wdog_tovall = 8191;   //Casova delka periody ~ cca 41s
//int wdog_tovalh = 0; int wdog_tovall = 32767;   //Casova delka periody ~ cca 2m 42s
//int wdog_tovalh = 0; int wdog_tovall = 65535;   //Casova delka periody ~ cca 5m 28s

/***    Velikost okna    ***/
int wdog_winh = 0; int wdog_winl = 4095;   //Casova delka okna ~ cca 20s
//int wdog_winh = 0; int wdog_winl = 16383;   //Casova delka okna ~ cca 1m 22s
//int wdog_winh = 0; int wdog_winl = 32767;   //Casova delka okna ~ cca 2m 42s


/*******    Cast aplikace prevzana a poupravena z ukazkove aplikace prezentovane na demo cviceni    *******/

/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))

/* Constants specifying delay loop duration */
#define	DELAY_T 200000

/* Mapping of LEDs and buttons to specific port pins: */
// Note: only D9, SW3 and SW5 are used in this sample app
#define LED_D9  0x20 // Port B, bit 5
#define LED_D10 0x10 // Port B, bit 4
#define LED_D11 0x8 // Port B, bit 3
#define LED_D12 0x4 // Port B, bit 2

#define BTN_SW2 0x400 // Port E, bit 10
#define BTN_SW3 0x1000 // Port E, bit 12
#define BTN_SW4 0x8000000 // Port E, bit 27
#define BTN_SW5 0x4000000 // Port E, bit 26
#define BTN_SW6 0x800 // Port E, bit 11

int pressed_up = 0;
int pressed_down = 0;

void MCUInit(void)
{
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
}

void PortsInit(void)
{
    /* Turn on all port clocks*/
    SIM->SCGC5 = SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;

    /* Set corresponding PTB pins (connected to LED's) for GPIO functionality*/
    PORTB->PCR[5] = PORT_PCR_MUX(0x01); // D9
    PORTB->PCR[4] = PORT_PCR_MUX(0x01); // D10
    PORTB->PCR[3] = PORT_PCR_MUX(0x01); // D11
    PORTB->PCR[2] = PORT_PCR_MUX(0x01); // D12

    PORTE->PCR[10] = PORT_PCR_MUX(0x01); // SW2
    PORTE->PCR[12] = PORT_PCR_MUX(0x01); // SW3
    PORTE->PCR[27] = PORT_PCR_MUX(0x01); // SW4
    PORTE->PCR[26] = PORT_PCR_MUX(0x01); // SW5
    PORTE->PCR[11] = PORT_PCR_MUX(0x01); // SW6

    /* Change corresponding PTB port pins as outputs*/
    PTB->PDDR = GPIO_PDDR_PDD( 0x3C );
    PTB->PDOR |= GPIO_PDOR_PDO( 0x3C); // turn all LEDs OFF
}

void LPTMR0_IRQHandler(void)
{
    LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK; // writing 1 to TCF tclear the flag
    GPIOB_PDOR ^= LED_D10; // invert D10 state
}

void WDOG_EWM_IRQHandler(void)
{
	;
}

void LPTMR0Init()
{
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;

    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;     // Turn OFF LPTMR to perform setup

    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) // 0000 is div 2
                 | LPTMR_PSR_PBYP_MASK  // LPO feeds directly to LPT
                 | LPTMR_PSR_PCS(1)) ; // use the choice of clock

    LPTMR0_CMR = 0x200;  // Set compare value

    LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK   // Clear any pending interrupt (now)
                 | LPTMR_CSR_TIE_MASK   // LPT interrupt enabled
                );

    NVIC_EnableIRQ(LPTMR0_IRQn);
    NVIC_EnableIRQ(WDOG_EWM_IRQn);

    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;   // Turn ON LPTMR0 and start counting
}

/*******    Cast aplikace, ktera je original (zabyva se WDOG modulem)    *******/

/**
 * funkce vytiskne hodnoty registru watchdogu
 */
void printWDOGRegisters()
{
	printf("WDOG_STCTRLH: %d (dec)\n", WDOG_STCTRLH);
	printf("WDOG_STCTRLL: %d (dec)\n", WDOG_STCTRLL);
	printf("WDOG_TOVALH:  %d (dec)\n", WDOG_TOVALH);
	printf("WDOG_TOVALL:  %d (dec)\n", WDOG_TOVALL);
	printf("WDOG_WINH:    %d (dec)\n", WDOG_WINH);
	printf("WDOG_WINL:    %d (dec)\n", WDOG_WINL);
	printf("WDOG_REFRESH: %d (dec)\n", WDOG_REFRESH);
	printf("WDOG_UNLOCK:  %d (dec)\n", WDOG_UNLOCK);
	printf("WDOG_TMROUTH: %d (dec)\n", WDOG_TMROUTH);
	printf("WDOG_TMROUTL: %d (dec)\n", WDOG_TMROUTL);
	printf("WDOG_RSTCNT:  %d (dec)\n", WDOG_RSTCNT);
	printf("WDOG_PRESC:   %d (dec)\n", WDOG_PRESC);
}

/**
 * funkce tiskne vyskyt flagu v registru WDOG_STCTRLH - nenulove cislo zdali je flag nastaven na true, jinak nula
 */
void printWDOG_STCTRHL()
{
	printf("7.WAITEN:      %d (dec)\n", WDOG_STCTRLH & 0b0000000010000000);
	printf("6.STOPEN:      %d (dec)\n", WDOG_STCTRLH & 0b0000000001000000);
	printf("5.DBGEN:       %d (dec)\n", WDOG_STCTRLH & 0b0000000000100000);
	printf("4.ALLOWUPDATE: %d (dec)\n", WDOG_STCTRLH & 0b0000000000010000);
	printf("3.WINEN:       %d (dec)\n", WDOG_STCTRLH & 0b0000000000001000);
	printf("2.IRQRSTEN:    %d (dec)\n", WDOG_STCTRLH & 0b0000000000000100);
	printf("1.CLKSRC:      %d (dec)\n", WDOG_STCTRLH & 0b0000000000000010);
	printf("0.WDOGEN:      %d (dec)\n", WDOG_STCTRLH & 0b0000000000000001);
}

int main(void)
{
	//odemknuti watchdogu pro zapis
	WDOG_UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520);
    WDOG_UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928);

    //nastaveni hlavniho registru WDOG_STCTRLH ridiciho cinnost watchdogu
    WDOG_STCTRLH = wdog_stctrlh;

    //nastaveni velikosti periody
    WDOG_TOVALH = WDOG_TOVALH_TOVALHIGH(wdog_tovalh);
    WDOG_TOVALL = WDOG_TOVALL_TOVALLOW(wdog_tovall);

    //nastaveni velikosti okna
    WDOG_WINH = WDOG_WINH_WINHIGH(wdog_winh);
    WDOG_WINL = WDOG_WINL_WINLOW(wdog_winl);

    //inicializace mikrokontroleru mimo WDOG
    MCUInit();
    PortsInit();
    LPTMR0Init();

    //promenne do kterych se ukladaji procentualni hodnoty pomeru registru WDOG
    int percentage = 0;
    int percentageWin = 0;

    //printWDOGRegisters();

    //cyklus cekajici na zmacknuti tlacitek
    while (1)
    {
        if (!pressed_up && !(GPIOE_PDIR & BTN_SW5))  //pri zmacknuti tlacitka se vypise vypis
        {
        	pressed_up = 1;

        	percentage = (WDOG_TMROUTL * 100)/WDOG_TOVALL;
        	percentageWin = (WDOG_TMROUTL * 100)/WDOG_WINL;

        	if((wdog_stctrlh & 0b1000) != 0)
        	{
        		printf("WDOG_TMROUTL: %d (%d %% of WDOG_WINL), (%d %% of WDOG_TOVALL)\n", WDOG_TMROUTL, percentageWin, percentage);
        	}
        	else
        	{
        		printf("WDOG_TMROUTL: %d (%d %% of WDOG_TOVALL)\n", WDOG_TMROUTL, percentage);
        	}

        }
        else
        {
        	if (GPIOE_PDIR & BTN_SW5)
        	{
        		pressed_up = 0;
        	}
        }

        if (!pressed_down && !(GPIOE_PDIR & BTN_SW3))  //pri zmacknuti tlacitka dojde k refreshi watchdogu
        {
            pressed_down = 1;

            WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xA602);
            WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xB480);

            printf("Refreshed.\n");
        }
        else
        {
        	if (GPIOE_PDIR & BTN_SW3)
        	{
        		pressed_down = 0;
        	}
        }
    }

    return 0;
}
