/*
 * main implementation
 */
/******************************************************************************/
/*Include files                                                             ***/
/******************************************************************************/
//#include "derivative.h"
#include <MKL25Z4.h>
#include "stdio.h"
#include "LCDDrv.h"
#include "LEDDrv.h"
#include "ADCDrv.h"
//#include "UART.h"
//#include "LEDsDvr.h"
//#include "MGCDvr.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TIME_MESSAGE 2000
#define PORTC_MASK 	(0x7F)
#define	PORTB_MASK	(0X0F)

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
GPIO_Type *rGpioB = GPIOB;
GPIO_Type *rGpioC = GPIOC;
GPIO_Type *rGpioD = GPIOD;
GPIO_Type *rGpioE = GPIOE;

/*******************************************************************************
 * Local Variables
 ******************************************************************************/
typedef enum
{
    STATE_LCD_WAIT,
    STATE_WELCOME_MSG,
    STATE_MSG_DLY_STATE,
    STATE_INFO
}eStates;

static void vfnLCDWaitState();
static void vfnWelcomeMsgState();
static void vfnMsgDlyState();
static void vfninformacion();

static void (* vfnMSG_States[])(void) =
{
	(void (*)(void))vfnLCDWaitState,
	(void (*)(void))vfnWelcomeMsgState,
	(void (*)(void))vfnMsgDlyState,
	(void (*)(void))vfninformacion
};

static unsigned char bCurrentState = STATE_LCD_WAIT;
static unsigned char bPreviousState;
static unsigned char bNextState = STATE_WELCOME_MSG;
static volatile unsigned short wDly = TIME_MESSAGE;
unsigned char bData = 0x00;
/*******************************************************************************
 * Function Prototypes Section
 ******************************************************************************/
void vfnMemSet ( unsigned char* bpDst, unsigned char bData, unsigned short wSize );

/*******************************************************************************
 * Code
 ******************************************************************************/

int main (void)
{
	vfnADC_Innit();                        //ADC INIT
	vfnADC_Channel_Innit();                //ADC CHANNEL INNIT
	vfnLEDsInit();
	vfnLCDInit();
	while(1)
	{	
		vfnLCDDriver();
		vfnMSG_States[bCurrentState]();
	}
	return 0;
}
/*******************************************************************************
* vfnLCDWaitState
* DESCRIPTION: 
*
* RETURNS:
* void
*******************************************************************************/
static void vfnLCDWaitState()
{
	if (!bfnLCDBusy())
	{
		bCurrentState = bNextState;
	}
}
/*******************************************************************************
* vfnWelcomeMsgState
* DESCRIPTION: Copies welcome msg
*
* RETURNS:
* void
*******************************************************************************/
static void vfnWelcomeMsgState()
{
	vfnMemSet(&gbLCDWelcomeMSG[0][0],' ',(sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));
	sprintf(&gbLCDWelcomeMSG[0][0],"   Welcome      ",(sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));	
	sprintf(&gbLCDWelcomeMSG[1][0],"   MARIO        ");
	vfnLCDUpDate();
	wDly = TIME_MESSAGE;
	bCurrentState = STATE_LCD_WAIT;
	bNextState = STATE_MSG_DLY_STATE;
	bPreviousState = STATE_INFO;
}
/*******************************************************************************
* vfnMsgDlyState
* DESCRIPTION:
*
* RETURNS:
* void
*******************************************************************************/
static void vfnMsgDlyState()
{
	if ((wDly--)==0)
	{
		bCurrentState = bPreviousState;
	}
}

/*******************************************************************************
* vfninformacion
* DESCRIPTION: 
*
* RETURNS:
* void
******************************************************************************/
static void vfninformacion()
{
	vfnMemSet(&gbLCDWelcomeMSG[0][0],' ',(sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));
	sprintf(&gbLCDWelcomeMSG[0][0],"  Test LCD      ",(sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));	
	sprintf(&gbLCDWelcomeMSG[1][0],"      %d         ",bData);
	vfnLCDUpDate();
	wDly = TIME_MESSAGE;
	bCurrentState = STATE_LCD_WAIT;
	bNextState = STATE_MSG_DLY_STATE;
	bPreviousState = STATE_INFO;
	//count++;;
}
/*******************************************************************************
* vfnMemSet
* DESCRIPTION: Rellena el bloque apuntado por bpDst con el dato indicado
*
* RETURNS:
* void
******************************************************************************/
void vfnMemSet ( unsigned char* bpDst, unsigned char bData, unsigned short wSize )
{
	if ((wSize--) == 0)
	{
		*bpDst++ = bData;
	}
}
