/*
 * LCDDrv.c
 *
 *  Created on: 20/02/22
 *      Author: Fernando J. Aragón
 */
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "LCDDrv.h"
#include "stdint.h"
#include <MKL25Z4.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define JUMP_VALUE          ( 0xC0 )
#define CURSOR_HOME_VALUE   ( 0x03 )
#define SHIFT	            ( 0x04 )
#define LSB_MASK            ( 0x0F )
#define MSB_MASK			( 0xF0 )
/*******************************************************************************
 * Local Function Prototypes
 ******************************************************************************/
void vfnLCDDriver();
void vfnDly500nsState();
void vfnState0MSN();
void vfnState1EDown();
void vfnState2LSN();
void vfnStateExecution();
void vfnState_Idle();

void (* vfnDriverLCD[])(void) =
{
    (void (*)(void))vfnState0MSN,
    (void (*)(void))vfnState1EDown,
    (void (*)(void))vfnState2LSN,
    (void (*)(void))vfnDly500nsState,
    (void (*)(void))vfnStateExecution,
    (void (*)(void))vfnState_Idle
};

/*******************************************************************************
 * Local Variables
 ******************************************************************************/

extern GPIO_Type *rGpioB;
extern GPIO_Type *rGpioC;

uint8_t gbLCDWelcomeMSG[LCD_Y][LCD_X];

uint8_t Data_Out = 0x00;
uint8_t bCursor = 0x00;
volatile  uint8_t bCnt = DELAY;
static uint8_t bDataSize = 0x00;
static uint8_t *bpLCDData = 0x00;
static uint8_t bLCDX = 0x00;
static uint8_t bLCDY = 0x00;
static uint8_t bRegister_Select = 0x00;

typedef enum
{
    MSN_STATE,
    DOWN_STATE,
    LSN_STATE,
    DELAY_STATE,
    EXECUTION_STATE,
    IDLE_STATE  
}InitStates; 

static uint8_t bPreviousState;
static uint8_t bCurrentState = IDLE_STATE;
static uint8_t bNextState = MSN_STATE;

static uint8_t baLCDConfig[]=
{
    LCD_INIT_CMD0,  //initial 3 steps
    LCD_INIT_CMD1,  //4bits
    LCD_INIT_CMD2,  //specify lines and fonts
    LCD_INIT_CMD3,  //display off
    LCD_INIT_CMD4,  //display clear
    LCD_INIT_CMD5   // mode set
};

static uint8_t balJump[]=
{
    JUMP_VALUE          //Jump
};

static uint8_t bCursor_Home[]=
{
    CURSOR_HOME_VALUE   //Cursor home
};

/*******************************************************************************
* vfnLCDDriver
* DESCRIPTION: Array with the funtion pointers
*
* RETURNS:
* void
******************************************************************************/
void vfnLCDDriver()
{
    vfnDriverLCD[bCurrentState]();
}
/*******************************************************************************
* vfnLCDInit
* DESCRIPTION: Set the first config parameters for LCD init 4 bits
*
* RETURNS:
* void
******************************************************************************/
void vfnLCDInit()
{
    if(bCurrentState == IDLE_STATE)
    { 
    bDataSize = sizeof(baLCDConfig);
    bpLCDData = &baLCDConfig[bLCDX];
    bCurrentState = MSN_STATE;
    } 
}
/*******************************************************************************
* bfnLCDBusy
* DESCRIPTION: Verify if the State machine is busy
*
* RETURNS:
* void
******************************************************************************/
uint8_t bfnLCDBusy()
{
    if (bCurrentState == IDLE_STATE)
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}
/*******************************************************************************
* vfnLCDUpDate
* DESCRIPTION: Overwrites (if not busy) the LCD
*
* RETURNS:
* void
******************************************************************************/
void vfnLCDUpDate()
{
    if(!bfnLCDBusy())
    {
        bDataSize = sizeof(gbLCDWelcomeMSG);
        bDataSize = LCD_X;
        bpLCDData = &gbLCDWelcomeMSG[bLCDY][bLCDX];
        rGpioC->PDOR |= RS;
        bRegister_Select = 0x01;
        bCurrentState = MSN_STATE;
    }
}
/*******************************************************************************
* vfnLCDGotoxy
* DESCRIPTION: Select the Line X or Y
*
* RETURNS:
* void
******************************************************************************/
void vfnLCDGotoxy(void) //0000-0100
{
    bpLCDData = &balJump[bLCDX];
    bDataSize = sizeof(balJump)/sizeof(balJump[0]);
}
/*******************************************************************************
* vfnLCDGotoxy_Home
* DESCRIPTION: Go to the first position
*
* RETURNS:
* void
******************************************************************************/
void vfnLCDGotoxy_Home()
{
    bpLCDData = &bCursor_Home[bLCDX];
    bDataSize = sizeof(bCursor_Home)/sizeof(bCursor_Home[0]);
}
/*******************************************************************************
* vfn
* DESCRIPTION:
*
* RETURNS:
* void
******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
/***********************STATE Machine******************************************/
////////////////////////////////////////////////////////////////////////////////

void vfnDly500nsState(void) 
{
    if((bCnt--)==0)
    {
        bCnt = DELAY;
        bCurrentState = bNextState;
    }
}
/*******************************************************************************
* vfnState0MSN
* DESCRIPTION: Most significant bit
*
* RETURNS:
* void
******************************************************************************/
void vfnState0MSN()
{         
	rGpioB->PDOR = (*bpLCDData & MSB_MASK) >> SHIFT;
    bPreviousState = LSN_STATE;
    bCurrentState = DELAY_STATE;
    bNextState = DOWN_STATE;
	rGpioC->PDOR |= ENABLE;
}
/*******************************************************************************
* vfnState1EDown
* DESCRIPTION: Write 0 to the Enable pin
*
* RETURNS:
* void
******************************************************************************/
void vfnState1EDown(void)
{ 
	rGpioC->PDOR &=  (~ENABLE);
    bCurrentState = DELAY_STATE; 
    bNextState = bPreviousState;
}
/*******************************************************************************
* vfnState2LSN Least Significant  
* DESCRIPTION:
*
* RETURNS:
* void
******************************************************************************/
void vfnState2LSN()
{ 
	rGpioB->PDOR = (*bpLCDData & LSB_MASK);
    bPreviousState = EXECUTION_STATE;
    bCurrentState = DELAY_STATE;
    bNextState = DOWN_STATE;
	rGpioC->PDOR |= ENABLE;
}
/*******************************************************************************
* vfnStateExecution
* DESCRIPTION: Routine for the Execution Rouitne RS 
*
* RETURNS:
* void
******************************************************************************/
void vfnStateExecution()
{
    bCurrentState = DELAY_STATE;
    if((bDataSize--) != 1)
    {
        bpLCDData++;
        bNextState = MSN_STATE;
    }
    else
    {
        if(bRegister_Select)
        {
            bRegister_Select = 0x00;
            rGpioC->PDOR &= ~(RS);
            bLCDX = 0x00;
            vfnLCDGotoxy();
            bCursor = 0x01;
            bNextState = MSN_STATE;
            if(bLCDY)
            {
                bLCDY = 0x00;
                bLCDX = 0x00;
                bRegister_Select = 0x00;
                rGpioC->PDOR &= ~(RS);
                vfnLCDGotoxy_Home();
                bNextState = MSN_STATE;
                bCursor = 0x00;
            }
        }
        else
        {
            if(bCursor)
            {
                bLCDY = 0x01;
                bDataSize = LCD_X;
                bLCDX = 0x00;
                bpLCDData = &gbLCDWelcomeMSG[bLCDY][bLCDX];
                bRegister_Select = 0x01;
                rGpioC->PDOR |= RS;
                bCursor = 0x00;
            }
            else
            {
                bLCDY = 0x00;
                bNextState = IDLE_STATE;
                bLCDX = 0x00;
                bRegister_Select = 0x00;
            }
        }
    }
}
/*******************************************************************************
* vfnState_Idle
* DESCRIPTION: Funtion for IDLE State Machine
*
* RETURNS:
* void
******************************************************************************/
void vfnState_Idle()
{
    //NTD
}
