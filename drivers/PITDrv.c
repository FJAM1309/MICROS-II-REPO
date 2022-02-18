/*
 * PITDrv.c
 *
 *  Created on: Feb 10, 2022
 *      Author: Mario Castaneda
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
 * vfnPIT0Init
 * DESCRIPTION:
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnPIT0Init(unsigned long dwValue)
{
	pSIM_RegPIT->SCGC6 |= SIM_SCGC6_PIT_MASK;
	pPIT0_Reg->MCR = PIT_MCR_FRZ_MASK;					/*Timer Enable during debug*/
	pPIT0_Reg->CHANNEL[0].LDVAL= dwValue;
	pPIT0_Reg->CHANNEL[0].TCTRL= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; 	/*Timer Enable*/
}
/*******************************************************************************
 * bfnPIT0Check
 * DESCRIPTION:
 *
 * RETURNS:
 * void
 *******************************************************************************/
unsigned char bfnPIT0Check(void)
{
	if(pPIT0_Reg->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) 	/*Timer Overflow*/
	{
		pPIT0_Reg->CHANNEL[0].TCTRL = 0;     // Disable timer
		pPIT0_Reg->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;/*Clear TOF by writting a 1*/
		pPIT0_Reg->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;     // Enable timer
		return ( 1 );
	}
	return ( 0 );
}
