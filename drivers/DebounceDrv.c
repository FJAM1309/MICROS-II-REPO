/*
 * DebounceDrv.c
 *
 *  Created on: 16/02/22
 *      Author: Fernando J. Aragón
 */
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "DebounceDrv.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Local Function Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Local Variables
 ******************************************************************************/
GPIO_Type *rDEBGpioE = GPIOE;           /* Peripheral GPIOE base address */
uint8_t gbPreviousState   = (1 << PIN_E0) | (1 << PIN_E1);
uint8_t gbCurrentState       = (1 << PIN_E0) | (1 << PIN_E1);
/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * vfnbuttonInit
 * DESCRIPTION: Initializes pins for each btn
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnbuttonInit(void)
{
    SIM_Type *rclkSIM_gpio = SIM;       /* Peripheral SIM base pointer */
    PORT_Type *rPortE = PORTE;          /* Peripheral PORTE base pointer */
    //Enable Clks
    rclkSIM_gpio->SCGC5 |= SIM_SCGC5_PORTE_MASK;

    rPortE->PCR[1] = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK; /*Alternative pin MUX*/

    rDEBGpioE->PDDR &= ~(1u << PIN_E1);         /*Port Data Direction Register*/
    rDEBGpioE->PDDR &= ~(1u << PIN_E0);
}
/*******************************************************************************
 * bfndebounceUpdate
 * DESCRIPTION: Waits for btn to stabilize and checks for a change of state
 * 
 * RETURNS:
 * 1 if theres a change in btn state
 ******************************************************************************/
uint8_t bfndebounceUpdate (void)  //Returns 0
{
    static uint8_t bCnt = 0;
    static uint8_t bCnt2 = 0;
    uint8_t bButtonStateLocal = ((rDEBGpioE->PDIR & (1u << PIN_E0)) | (rDEBGpioE->PDIR & (1u << PIN_E1))) ;  //Read btn state
    uint8_t bButtonChange = bButtonStateLocal ^ gbPreviousState;

    if (bButtonChange & (1 << PIN_E0))
    {
        bCnt = 3;
    }
    /*cada 20ms*/
    else
    {
        if (bCnt)
        {
            bCnt--;
        }
        else
        {
            gbCurrentState &= ~(1 << PIN_E0); //Clear var
            gbCurrentState |= ((1 << PIN_E0) & bButtonStateLocal) ; //gbCurrentState= 1 | 1
        }
    }
    //btn 2
    if (bButtonChange & (1 << PIN_E1))
    {
        bCnt2 = 3;
    }
    /*20ms*/
    else
    {
        if (bCnt2)
        {
            bCnt2--;
        }
        else
        {
            gbCurrentState &= ~(1 << PIN_E1);
            gbCurrentState |= ((1 << PIN_E1) & bButtonStateLocal);
        }
    }
    gbPreviousState = bButtonStateLocal;
    return gbCurrentState;
}
/*******************************************************************************
 * bfnBtnState
 * DESCRIPTION: Read specific pin for each button and return state 
 *
 * RETURNS:
 * Button state
 ******************************************************************************/
uint8_t bfnBtnState(void)
{
    if(bFlag == 0)
    {
    bFlag = 1; //[MC]
    bVarDebounce = bfndebounceUpdate();
    bChange = bVarDebounce ^ bLastValue; //VERIFICAR SI HAY CAMBIO
    if(bChange)
    {
        if((bChange & (1 << PIN_E0)) && //[MC]
        (!(bVarDebounce & (1 << PIN_E0)))) 
        {
            return 1;
        }
        if((bChange & (1 << PIN_E1)) && //[MC]
        (!(bVarDebounce & (1 << PIN_E1))))
        {
            return 1;
        }
    }
    bLastValue = bVarDebounce;
    return 0;
    }
}