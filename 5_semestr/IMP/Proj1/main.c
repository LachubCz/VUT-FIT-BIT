/* Header file with all the essential definitions for a given type of MCU */
#include "MK60D10.h"
#include <stdio.h>
#include <string.h>

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

int pressed_up = 0, pressed_down = 0;
unsigned int compare = 0x200;

/* A delay function */
void delay(long long bound) {

  long long i;
  for(i=0;i<bound;i++);
}

/* Initialize the MCU - basic clock settings, turning the watchdog off */
void MCUInit(void)  {
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    //WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
    //WDOG_STCTRLH &= WDOG_STCTRLH_WDOGEN(1);
   // WDOGInit();
    //WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

void PortsInit(void)
{
    /* Turn on all port clocks */
    SIM->SCGC5 = SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;

    /* Set corresponding PTB pins (connected to LED's) for GPIO functionality */
    PORTB->PCR[5] = PORT_PCR_MUX(0x01); // D9
    PORTB->PCR[4] = PORT_PCR_MUX(0x01); // D10
    PORTB->PCR[3] = PORT_PCR_MUX(0x01); // D11
    PORTB->PCR[2] = PORT_PCR_MUX(0x01); // D12

    PORTE->PCR[10] = PORT_PCR_MUX(0x01); // SW2
    PORTE->PCR[12] = PORT_PCR_MUX(0x01); // SW3
    PORTE->PCR[27] = PORT_PCR_MUX(0x01); // SW4
    PORTE->PCR[26] = PORT_PCR_MUX(0x01); // SW5
    PORTE->PCR[11] = PORT_PCR_MUX(0x01); // SW6

    /* Change corresponding PTB port pins as outputs */
    PTB->PDDR = GPIO_PDDR_PDD( 0x3C );
    PTB->PDOR |= GPIO_PDOR_PDO( 0x3C); // turn all LEDs OFF
}

void LPTMR0_IRQHandler(void)
{
    // Set new compare value set by up/down buttons
    LPTMR0_CMR = compare; // !! the CMR reg. may only be changed while TCF == 1
    LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK; // writing 1 to TCF tclear the flag
    GPIOB_PDOR ^= LED_D10; // invert D9 state
}

void LPTMR0Init(int count)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;

    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;     // Turn OFF LPTMR to perform setup

    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) // 0000 is div 2
                 | LPTMR_PSR_PBYP_MASK  // LPO feeds directly to LPT
                 | LPTMR_PSR_PCS(1)) ; // use the choice of clock

    LPTMR0_CMR = count;  // Set compare value

    LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK   // Clear any pending interrupt (now)
                 | LPTMR_CSR_TIE_MASK   // LPT interrupt enabled
                );

    NVIC_EnableIRQ(LPTMR0_IRQn);

    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;   // Turn ON LPTMR0 and start counting
}

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

void printWDOG_STCTRHL()
{
	printf("7.WAITEN:      %d ()\n", WDOG_STCTRLH & 0b0000000010000000);
	printf("6.STOPEN:      %d ()\n", WDOG_STCTRLH & 0b0000000001000000);
	printf("5.DBGEN:       %d ()\n", WDOG_STCTRLH & 0b0000000000100000);
	printf("4.ALLOWUPDATE: %d ()\n", WDOG_STCTRLH & 0b0000000000010000);
	printf("3.WINEN:       %d ()\n", WDOG_STCTRLH & 0b0000000000001000);
	printf("2.IRQRSTEN:    %d ()\n", WDOG_STCTRLH & 0b0000000000000100);
	printf("1.CLKSRC:      %d ()\n", WDOG_STCTRLH & 0b0000000000000010);
	printf("0.WDOGEN:      %d ()\n", WDOG_STCTRLH & 0b0000000000000001);
}

int main(void)
{
	//odemknuti watchdogu pro zapis

	WDOG_UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520);
    WDOG_UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928);

    //delay(10);
    //WDOG_STCTRLH = WDOG_STCTRLH_ALLOWUPDATE(1);
    //WDOG_STCTRLH = WDOG_STCTRLH_IRQRSTEN(1);
    //WDOG_STCTRLH |= WDOG_STCTRLH_WDOGEN(1);
    WDOG_STCTRLH = 0b0011;/*
    if(WDOG_TMROUTL > 2)
    {

    	WDOG_STCTRLH |= WDOG_STCTRLH_DBGEN(0x1);
    }*/

    //WDOG_STCTRLH |= WDOG_STCTRLH_ALLOWUPDATE(1);

    WDOG_TOVALH = WDOG_TOVALH_TOVALHIGH(65535);
    WDOG_TOVALL = WDOG_TOVALL_TOVALLOW(65535);

    /*
	WDOG_UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520);
    WDOG_UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928);
    WDOG_STCTRLH = WDOG_STCTRLH_ALLOWUPDATE(1);

    WDOG_STCTRLH |= WDOG_STCTRLH_CLKSRC(1);
    WDOG_STCTRLH |= WDOG_STCTRLH_WDOGEN(1);
*/
    printWDOGRegisters();
    printWDOG_STCTRHL();
    printf("__________________________\n");
    MCUInit();
    PortsInit();
    LPTMR0Init(compare);

    //printf("Hodnota registeru WDOG_STCTRHL: %x (hex)\n", WDOG_STCTRLH);
    printWDOGRegisters();
    printWDOG_STCTRHL();

    while (1) {
        // pressing the up button decreases the compare value,
        // i.e. the compare event will occur more often;
        // testing pressed_up avoids uncontrolled modification
        // of compare if the button is hold.
        if (!pressed_up && !(GPIOE_PDIR & BTN_SW5))
        {
            printWDOGRegisters();
            printWDOG_STCTRHL();
            pressed_up = 1;
            compare -= 0x40;
        }
        else if (GPIOE_PDIR & BTN_SW5) pressed_up = 0;
        // pressing the down button increases the compare value,
        // i.e. the compare event will occur less often;
        if (!pressed_down && !(GPIOE_PDIR & BTN_SW3))
        {
            pressed_down = 1;
            compare += 0x40;
            WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xA602);
            WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xB480);
            printf("DONE.\n");
        }
        else if (GPIOE_PDIR & BTN_SW3) pressed_down = 0;
        // some limits - in order to keep the LED blinking reasonable
        if (compare < 0x40) compare = 0x40;
        if (compare > 0x400) compare = 0x400;
    }
    printf("End\n");
    return 0;
}
