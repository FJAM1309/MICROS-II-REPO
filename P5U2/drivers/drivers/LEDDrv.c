/*
 * LEDDrv.c
 *
 *  Created on: Jan 16, 2022
 *      Author: Mario Castañeda
 */
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include"LEDDrv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Local Function Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Local Variables
 ******************************************************************************/

extern GPIO_Type *rGpioA; 			/* Peripheral GPIOA base address */
extern GPIO_Type *rGpioB;			/* Peripheral GPIOB base address */
extern GPIO_Type *rGpioC;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * vfnLEDsInit
 * DESCRIPTION: Pin leds Initialization
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnLEDsInit(void)
{
	SIM_Type *rclkSIM_gpio= SIM;		/* Peripheral SIM base pointer */

	PORT_Type *rPortB = PORTB;			/* Peripheral PORTB base pointer */
	PORT_Type *rPortC = PORTC;


	/*Enable Clks*/
	rclkSIM_gpio->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTC_MASK;

//	rPortD->PCR[BLU] = PORT_PCR_MUX(1);	/*Alternative pin MUX*/
//	rPortB->PCR[RED] = PORT_PCR_MUX(1);
//	rPortB->PCR[GRN] = PORT_PCR_MUX(1);

    rPortB->PCR[PIN_B0] = PORT_PCR_MUX(1);
    rPortB->PCR[PIN_B1] = PORT_PCR_MUX(1);
    rPortB->PCR[PIN_B2] = PORT_PCR_MUX(1);
    rPortB->PCR[PIN_B3] = PORT_PCR_MUX(1);

    rPortC->PCR[PIN_C1] = PORT_PCR_MUX(1);
    rPortC->PCR[PIN_C2] = PORT_PCR_MUX(1);

//    rGpioD->PDDR =1u << BLU;			/*Port Data Direction Register*/
//    rGpioB->PDDR = 1u << RED | 1u << GRN;

//	rGpioB->PSOR = 1u << RED; 			/*Turn OFF*/
//	rGpioB->PSOR |= 1u << GRN;
//	rGpioD->PSOR |= 1u << BLU;

    rGpioB->PDDR |= 1u << PIN_B0 | 1u << PIN_B1 | 1u << PIN_B2 | 1u << PIN_B3;

    rGpioC->PDDR |= 1u << PIN_C1 | 1u << PIN_C2;
}
/*******************************************************************************
 * vfnLEDRed
 * DESCRIPTION: Turn on RED
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnLEDRed(void)
{
	/*RED*/
	rGpioB->PSOR = 1u << RED;
	rGpioB->PCOR = 1u << GRN;
	rGpioA->PCOR = 1u << BLU;
}
/*******************************************************************************
 * vfnLEDGreen
 * DESCRIPTION: Turn on GREEN
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnLEDGreen(void)
{
	/*GREEN*/
	rGpioB->PCOR = 1u << RED;
	rGpioB->PSOR = 1u << GRN;
	rGpioA->PCOR = 1u << BLU;
}
/*******************************************************************************
 * vfnLEDBlue
 * DESCRIPTION: Turn on BLUE
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnLEDBlue(void)
{
	/*BLUE*/
	rGpioB->PSOR = 1u << RED;
	rGpioB->PSOR = 1u << GRN;
	rGpioA->PCOR = 1u << BLU;
}
/*******************************************************************************
 * vfnLEDsOff
 * DESCRIPTION: Turn off all the LEDs RGB
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnLEDsOff(void)
{
	rGpioB->PDOR &= ~(1u << RED);
	rGpioB->PDOR &= ~(1u << GRN);
	rGpioA->PDOR &= ~(1u << BLU);
}
