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
#define TIME_MESSAGE 	( 2000 )
#define PORTC_MASK 		( 0x7F )
#define	PORTB_MASK		( 0X0F )
#define ADC_CHANNEL0	( 0 )

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
    STATE_READ_ADC,
    STATE_DISPLAY_INFO
}eStates;

static void vfnLCDWaitState();
static void vfnWelcomeMsgState();
static void vfnMsgDlyState();
static void vfnReadADC();
static void vfnDisplayInfo();

static void (*vfnMSG_States[])(void) =
{
	(void (*)(void))vfnLCDWaitState,
	(void (*)(void))vfnWelcomeMsgState,
	(void (*)(void))vfnMsgDlyState,
	(void (*)(void))vfnReadADC,
	(void (*)(void))vfnDisplayInfo
};

static uint8_t bCurrentState = STATE_LCD_WAIT;
static uint8_t bPreviousState;
static uint8_t bNextState = STATE_WELCOME_MSG;
static volatile uint16_t wDly = TIME_MESSAGE;
uint8_t bTemperature = 0x00;
uint8_t bData = 0x00;
/*******************************************************************************
 * Function Prototypes Section
 ******************************************************************************/
void vfnMemSet (unsigned char* bpDst, unsigned char bData, unsigned short wSize);

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
//STATE_LCD_WAIT
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
//STATE_MSG_DLY_STATE
static void vfnWelcomeMsgState()
{
	vfnMemSet(&gbLCDWelcomeMSG[0][0],' ',(sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));
	sprintf(&gbLCDWelcomeMSG[0][0],"   WELCOME      ");
	sprintf(&gbLCDWelcomeMSG[1][0],"   JULIAN        ");
	vfnLCDUpDate();
	wDly = TIME_MESSAGE;
	bCurrentState = STATE_LCD_WAIT;
	bNextState = STATE_MSG_DLY_STATE;
	bPreviousState = STATE_READ_ADC;
}
/*******************************************************************************
* vfnMsgDlyState
* DESCRIPTION:
*
* RETURNS:
* void
*******************************************************************************/
//STATE_MSG_DLY_STATE
static void vfnMsgDlyState()
{
	if ((wDly--)==0)
	{
		bCurrentState = bPreviousState;
	}
}
/*******************************************************************************
* vfnReadADC
* DESCRIPTION: Read ADC value and stores it in bData
*
* RETURNS:
* void
******************************************************************************/
//STATE_READ_ADC
static void vfnReadADC()
{
	bTemperature = bfnADC_Read(ADC_CHANNEL0);
	bCurrentState = STATE_LCD_WAIT;
	bNextState = STATE_MSG_DLY_STATE;
	bPreviousState = STATE_DISPLAY_INFO;
}

/*******************************************************************************
* vfnDisplayInfo
* DESCRIPTION: 
*
* RETURNS:
* void
******************************************************************************/
//STATE_DISPLAY_INFO
static void vfnDisplayInfo()
{
	vfnMemSet(&gbLCDWelcomeMSG[0][0],' ', (sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));
	sprintf(&gbLCDWelcomeMSG[0][0],"   Temperature:     ", (sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));
	sprintf(&gbLCDWelcomeMSG[1][0],"      %d         ", bTemperature);
	vfnLCDUpDate();
	wDly = TIME_MESSAGE;
	bCurrentState = STATE_LCD_WAIT;
	bNextState = STATE_MSG_DLY_STATE;
	bPreviousState = STATE_READ_ADC;
}

/*******************************************************************************
* vfnMemSet
* DESCRIPTION: Fills bpDst block woth bData
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
