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

GPIO_Type *rGpioD = GPIOD; 			/* Peripheral GPIOA base address */
extern GPIO_Type *rGpioB;			/* Peripheral GPIOB base address */
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

	PORT_Type *rPortD = PORTD;			/* Peripheral PORTA base pointer */
	PORT_Type *rPortB = PORTB;			/* Peripheral PORTB base pointer */

	/*Enable Clks*/
	rclkSIM_gpio->SCGC5 = SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

	rPortD->PCR[1] = PORT_PCR_MUX(1);	/*Alternative pin MUX*/
	rPortB->PCR[18] = PORT_PCR_MUX(1);
	rPortB->PCR[19] = PORT_PCR_MUX(1);
    rPortB->PCR[PIN_B0] = PORT_PCR_MUX(1);


    rGpioD->PDDR =1u << BLU;			/*Port Data Direction Register*/
    rGpioB->PDDR = 1u << RED | 1u << GRN | 1u << PIN_B0;

	rGpioB->PSOR = 1u << RED; 			/*Turn OFF*/
	rGpioB->PSOR |= 1u << GRN;
	rGpioD->PSOR |= 1u << BLU;

    rGpioB->PSOR |= 1u << PIN_B0;
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
	rGpioB->PCOR = 1u << RED;
	rGpioB->PSOR = 1u << GRN;
	rGpioD->PSOR = 1u << BLU;
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
	rGpioB->PSOR = 1u << RED;
	rGpioB->PCOR = 1u << GRN;
	rGpioD->PSOR = 1u << BLU;
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
	rGpioD->PCOR = 1u << BLU;
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
	rGpioB->PSOR = 1u << RED;
	rGpioB->PSOR = 1u << GRN;
	rGpioD->PSOR = 1u << BLU;
}
