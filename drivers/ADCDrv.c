/*
 * ADCDrv.c
 *
 *  Created on: 16/02/22
 *      Author: Fernando J. Aragón
 */
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <MKL25Z4.h>
#include "ADCDrv.h"
#include "A7U2.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Local Function Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Local Variables
 ******************************************************************************/
ADC_Type *rADC = ADC0;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*******************************************************************************
 * vfnADC_Innit
 * DESCRIPTION: Initializes the clock and port for the ADC, 8 bits, clk/2 
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnADC_Innit()
{
    SIM_Type *rclkSIM_gpio = SIM;

    rclkSIM_gpio->SCGC6 |= SIM_SCGC6_ADC0_MASK;     //en clk
    rADC->SC1[0] = 0;                               /*Default Set up*/
    // ADC0_CFG1[3:2] = 00  - Mode 8 bits |  ADC0_CFG1[6:5] = 01  - Clock divide select clk/2
    rADC->CFG1 |= (ADC_CFG1_MODE(0)) | (ADC_CFG1_ADIV(1));
    // ADC0_CFG1 = 10     - ADICLK Input Clock select
    rADC->CFG1 |= ADC_CFG1_ADICLK(0);
    // ADC0_CFG2[4] = 0 - MUXSEL MASK ADxxa channels are selected.
    rADC->CFG2 &= ADC_CFG2_MUXSEL_MASK;
    // ADC0_SC3[3] = 1 - Continuous Conversion Enable
    rADC->SC3 |= (1 << ADC_SC3_ADCO_SHIFT);

//    // ADC0_SC3[AVGE] = 1     // Hardware Average Enable
//  rADC->SC3 |= ADC_SC3_AVGE(1);
//    // ADC0_AVGS = 01       - Hardware average select (8 samples)
//  rADC->SC3 |= ADC_SC3_AVGS(1);
//    // ADC0_ADCH = 00000    - Channel select
//  rADC->SC1[0] &= ~ADC_SC1_DIFF_MASK;
}

/*******************************************************************************
 * vfnADC_Channel_Innit
 * DESCRIPTION: Initializes the gpio pin and channel for the ADC, PIN_E20, as input
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnADC_Channel_Innit()
{
    SIM_Type *rclkSIM_gpio = SIM;
    GPIO_Type *rGpioE = GPIOE;
    PORT_Type *rPortE = PORTE;

    // SCGC5 Port E
    rclkSIM_gpio-> SCGC5 |= SIM_SCGC5_PORTE_MASK;
    // Port E pin 20
    rPortE->PCR[PIN_E20] |= PORT_PCR_MUX(0);
    rGpioE->PDDR &= ~(1 << PIN_E20);
}


/*******************************************************************************
 * bfnADC_Read
 * DESCRIPTION: Starts conversion and reads ADC value in specified channel (bChannel)
 *
 * RETURNS:
 * bValue = ADC value read from bChannel
 ******************************************************************************/
uint8_t bfnADC_Read(uint8_t bChannel)
{
    uint8_t bValue;

    rADC->SC1[0] = bChannel & ADC_SC1_ADCH_MASK;    /*Write to SC1A the channel to start conversion*/
    while(!(rADC->SC1[0] & ADC_SC1_COCO_MASK))
    {
        /*run until conversion is done*/
    }
    bValue = (uint8_t)rADC->R[0];    // ADC0_Rn      - Results register
    return bValue;
}

uint8_t bfnConvert_V2Temp(uint8_t bVoltage)
{
    uint8_t bTemp = 0;
    bTemp = bVoltage/CONVERSION_VALUE;
    return bTemp;
}
