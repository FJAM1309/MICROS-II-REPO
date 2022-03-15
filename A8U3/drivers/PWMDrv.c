/*
 * PWMDvr.c
 *
 *  Created on: 16/02/22
 *      Author: Fernando J. Aragón
 */
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "MKL25Z4.h"
#include "PWMDrv.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Local Function Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Local Variables
 ******************************************************************************/
TPM_Type *rgTPM1 = TPM1;
SIM_Type *rclkSIM_TPM = SIM;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * vfnPWM_Init
 * DESCRIPTION: Initializes clock and pin
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnTPMInit()
{
    SIM_Type *rclkSIM_TPM   =  SIM;          /* Peripheral SIM base pointer   */
    PORT_Type *rPortE       = PORTE;         /* Peripheral PORTB base pointer */
    rclkSIM_TPM->SCGC5     |= SIM_SCGC5_PORTE_MASK;
    rPortE->PCR[PIN_E20]    = PORT_PCR_MUX(3);/* TPM1_CH0 */
}

void vfnPWMConfig(uint32_t dwModule, uint32_t dwPrescale, uint8_t bCntType)
{
    vfnTPMInit();
    rclkSIM_TPM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM1_MASK;

    /*CLOCK_PLLFLL*/ /*CLOCK_OSCERCLK*//*CLOCK_MCGIRCLK*/
    rclkSIM_TPM->SOPT2 |= SIM_SOPT2_TPMSRC(CLOCK_PLLFLL);

    rgTPM1->MOD  =  dwModule;
    rgTPM1->SC  |= (TPM_SC_CMOD(CLOCK_INT) | TPM_SC_PS(dwPrescale));

    if(bCntType)
    {
        rgTPM1->SC |= TPM_SC_CPWMS_MASK;
    }
    else
    {
        rgTPM1->SC |= TPM_SC_TOIE_MASK;
    }
}

/*******************************************************************************
 * vfnTPMChannelInnit
 * DESCRIPTION: Initialize TPM channel
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnTPMChannelInnit(uint32_t dwChNum, uint32_t dwMode)
{
    rgTPM1->CONTROLS[dwChNum].CnSC |= dwMode;
}

void vfnTPMChannelDisable(uint32_t dwChNum, uint32_t dwMode)
{
    rgTPM1->CONTROLS[dwChNum].CnSC &= ~(dwMode);
}
/*******************************************************************************
 * vfnFuncPWM
 * DESCRIPTION: PWM value
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnFuncPWM(uint32_t dwChNum, uint32_t dwValue)
{
    rgTPM1->CONTROLS[dwChNum].CnV |= dwValue;
}
