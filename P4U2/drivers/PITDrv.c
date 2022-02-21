/*
 * PITDrv.c
 *
 *  Created on: Feb 10, 2022
 *      Author: Fernando J. Aragón
 */
/******************************************************************************/
/***        Include files                                                   ***/
/******************************************************************************/
#include "PITDrv.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Local Function Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Local Variables
 ******************************************************************************/
SIM_Type *pSIM_RegPIT = SIM;
PIT_Type *pPIT0_Reg = PIT;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * vfnPITInit
 * DESCRIPTION: Initializes timer, clk and timer value for pit.
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnPITInit(unsigned long dwValue, uint8_t bChannel)
{
	pSIM_RegPIT->SCGC6 |= SIM_SCGC6_PIT_MASK;
	pPIT0_Reg->MCR = PIT_MCR_FRZ_MASK;					/*Timer Enable during debug*/
	pPIT0_Reg->CHANNEL[bChannel].LDVAL= dwValue;
	pPIT0_Reg->CHANNEL[bChannel].TCTRL= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; 	/*Timer Enable*/
}

/*******************************************************************************
 * bfnPIT0Check
 * DESCRIPTION: Verifies if pit counter has overflow.
 *
 * RETURNS:
 * 1 if pit timer is done, 0 if not
 *******************************************************************************/
unsigned char bfnPITCheck(uint8_t bChannel)
{
	if(pPIT0_Reg->CHANNEL[bChannel].TFLG & PIT_TFLG_TIF_MASK) 	/*Timer Overflow*/
	{
		pPIT0_Reg->CHANNEL[bChannel].TCTRL = 0;     // Disable timer
		pPIT0_Reg->CHANNEL[bChannel].TFLG = PIT_TFLG_TIF_MASK;/*Clear TOF by writting a 1*/
		pPIT0_Reg->CHANNEL[bChannel].TCTRL |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;     // Enable timer
		return ( 1 );
	}
	return ( 0 );
}
