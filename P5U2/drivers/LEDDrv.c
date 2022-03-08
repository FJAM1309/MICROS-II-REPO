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

GPIO_Type *rGpioA = GPIOA; 			/* Peripheral GPIOA base address */
GPIO_Type *rGpioB = GPIOB;			/* Peripheral GPIOB base address */
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

	PORT_Type *rPortA = PORTA;			/* Peripheral PORTA base pointer */
	PORT_Type *rPortB = PORTB;			/* Peripheral PORTB base pointer */

	/*Enable Clks*/
	rclkSIM_gpio->SCGC5 = SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTA_MASK;

	rPortA->PCR[13] = PORT_PCR_MUX(1);	/*Alternative pin MUX*/
	rPortB->PCR[18] = PORT_PCR_MUX(1);
	rPortB->PCR[19] = PORT_PCR_MUX(1);

    rGpioA->PDDR =1u << BLU;			/*Port Data Direction Register*/
    rGpioB->PDDR = 1u << RED | 1u << GRN;

	rGpioB->PSOR = 1u << RED; 			/*Turn OFF*/
	rGpioB->PSOR |= 1u << GRN;
	rGpioA->PSOR |= 1u << BLU;
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
