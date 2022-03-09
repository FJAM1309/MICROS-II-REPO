/**
 * @file    PWMUART_TEST.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "stdlib.h"
#include "PWMDrv.h"
#include "PITDrv.h"
#include "LEDDrv.h"
#include "LCDDrv.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
#define MAX_SONGS           ( 3U )
#define ASCII_CONVERSION    ( 0x30U )
#define SONG_MIN			( 1 )
#define SONG_MAX			( 3 )
#define TIME_MESSAGE 		( 2000 )
#define PORTC_MASK 			( 0x7F )
#define	PORTB_MASK			( 0X0F )
#define ADC_CHANNEL0		( 0 )

GPIO_Type *rGpioA = GPIOA;
GPIO_Type *rGpioB = GPIOB;
GPIO_Type *rGpioC = GPIOC;
GPIO_Type *rGpioD = GPIOD;
GPIO_Type *rGpioE = GPIOE;

typedef enum
{
    STATE_LCD_WAIT,
    STATE_WELCOME_MSG,
    STATE_MSG_DLY_STATE,
    STATE_READ_SONG,
    STATE_DISPLAY_INFO
}eStates;

static void vfnLCDWaitState();
static void vfnWelcomeMsgState();
static void vfnMsgDlyState();
static void vfnReadSong();
static void vfnDisplayInfo();

static void (*vfnMSG_States[])(void) =
{
	(void (*)(void))vfnLCDWaitState,
	(void (*)(void))vfnWelcomeMsgState,
	(void (*)(void))vfnMsgDlyState,
	(void (*)(void))vfnReadSong,
	(void (*)(void))vfnDisplayInfo
};

static uint8_t bCurrentState = STATE_LCD_WAIT;
static uint8_t bPreviousState;
static uint8_t bNextState = STATE_WELCOME_MSG;
static volatile uint16_t wDly = TIME_MESSAGE;
uint8_t baSongName[16];
volatile uint8_t bUpdateCycle;
uint8_t bCharVal = 3;
uint8_t baZelda[] = {

	  // Zelda's Lullaby - The Legend of Zelda Ocarina of Time.
	  // Score available at https://musescore.com/user/12754451/scores/2762776

		NOTE_E4,	NOTE_G4,
		NOTE_D4,	NOTE_C4,	NOTE_D4,
		NOTE_E4,	NOTE_G4,
		NOTE_D4,
		NOTE_E4,	NOTE_G4,
		NOTE_D5,	NOTE_C5,
		NOTE_G4,	NOTE_F4,	NOTE_E4,
		NOTE_D4,
		NOTE_E4,	NOTE_G4,
		NOTE_D4,	NOTE_C4,	NOTE_D4,
		NOTE_E4,	NOTE_G4,
		NOTE_D4,
		NOTE_E4,	NOTE_G4,

		NOTE_D5,	NOTE_C5,
		NOTE_G4,	NOTE_F4,	NOTE_E4,
		NOTE_F4,	NOTE_E4,	NOTE_C4,
		NOTE_F4,	NOTE_E4,	NOTE_D4,
		NOTE_E4,	NOTE_D4,	NOTE_A3,
		NOTE_G4,	NOTE_F4,	NOTE_E4,
		NOTE_F4,	NOTE_E4,	NOTE_C4,	NOTE_F4,
		NOTE_C5,
	};

	uint8_t baTimeZelda [] = {
			2,  4,  8,
			2,  8,
			2,  4,
			-2,
			2,  4,
			2,  4,  8,
			2,  8,
			-2,
			2,  4,  8,
			2,  8,
			2,  4,
			-2,
			2,  4,

			2,  4,  8,
			2,  8,  2,
			8,  8,  8,
			2,  8,  2,
			8,  8,  8,
			2,  8,  4,
			8,  8,    4,
			-2,
	};

	uint8_t bNotes = sizeof(baZelda)/sizeof(baZelda[0]);
	uint32_t dwWholeNote = (600000 * 4) / TEMPO;
	uint8_t bDivider = 0;

int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
	vfnLCDInit();
    vfnLEDsInit();
	TPM_Type *rgTPM1 = TPM1;
	/*TPM1_INIT*/
	vfnPWMConfig(MODULE, PRE_128, UPDOWN_COUNTING);
	/*PWM Mode  31.3.4 Channel Status and Control*/
	vfnTPMChannelInnit(CHANNEL_0, PWM_MODE);
	/*Channel 0, Duty */
	vfnFuncPWM(CHANNEL_0, DUTY);
	uint32_t dwDly = (baTimeZelda[0] * CONVERSION_TIME);
	uint8_t bCnt = 0;

	do
	{
		PRINTF("\r\nPlease enter a value to select song:\r\n");
	    PRINTF("Note: There are 3 songs; .\r\n");
	    PRINTF("Selection:");
	    bCharVal = GETCHAR() - ASCII_CONVERSION;
	    if(bCharVal < SONG_MIN || bCharVal > SONG_MAX)
	    {
			PRINTF("\r\nInvalid choice, pick again!\r\n");
	    }
	}while(bCharVal < SONG_MIN && bCharVal > SONG_MAX);

    PRINTF("\r\n%d", bCharVal);
    PRINTF("\r\n");

	while(1)
	{
		vfnLCDDriver();
		vfnMSG_States[bCurrentState]();
		if(dwDly-- == 0)
		{
			vfnTPMChannelDisable(CHANNEL_0, PWM_MODE);	//Disable channel to modify MOD
			rgTPM1->MOD = (baZelda[bCnt] * 100);				//Modify mod to desired frequency (note)
			vfnTPMChannelInnit(CHANNEL_0, PWM_MODE);	//Enable back channel
			bDivider = (baTimeZelda[bCnt]);				//Set delay to note intervals
			if(bDivider > 0)
			{
				dwDly = dwWholeNote/bDivider;
			}
			if(bDivider < 0)
			{
				dwDly = dwWholeNote/abs(bDivider);
				dwDly *= DOTTED_NOTE;
			}
			bCnt++;
			if(bCnt >= bNotes)
			{
				bCnt = 0;
			}
		}
	}
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
//STATE_MSG_DLY_STATE
static void vfnWelcomeMsgState()
{
	vfnMemSet(&gbLCDWelcomeMSG[0][0],' ',(sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));
	sprintf(&gbLCDWelcomeMSG[0][0],"   WELCOME      ");
	sprintf(&gbLCDWelcomeMSG[1][0],"   JULIAN       ");
	vfnLCDUpDate();
	wDly = TIME_MESSAGE;
	bCurrentState = STATE_LCD_WAIT;
	bNextState = STATE_MSG_DLY_STATE;
	bPreviousState = STATE_READ_SONG;
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
//STATE_READ_SONG
static void vfnReadSong()
{
	if(bCharVal == 1)
	{
		sprintf(&baSongName[0], " Zelda's Lullaby ", sizeof(baSongName)/sizeof(baSongName[0]));
	}
	else if(bCharVal == 2)
	{
		sprintf(&baSongName[0], " Darude Sandstorm", sizeof(baSongName)/sizeof(baSongName[0]));
	}
	else
	{
		sprintf(&baSongName[0], "      ASAP      ", sizeof(baSongName)/sizeof(baSongName[0]));
	}
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
	sprintf(&gbLCDWelcomeMSG[0][0]," Song playing:  ", (sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));
	sprintf(&gbLCDWelcomeMSG[1][0],"%s", baSongName);
	vfnLCDUpDate();
	wDly = TIME_MESSAGE;
	bCurrentState = STATE_LCD_WAIT;
	bNextState = STATE_MSG_DLY_STATE;
	bPreviousState = STATE_READ_SONG;
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
