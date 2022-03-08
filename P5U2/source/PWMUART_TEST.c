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
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
#define MAX_SONGS           ( 3U )
#define ASCII_CONVERSION    ( 0x30U )
#define ASCII_MAX			( 0x34 )
#define ASCII_MIN			( 0x30 )
volatile uint8_t bUpdateCycle;
uint8_t bCharVal;
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
	uint8_t bWholeNote = (60000 * 4) / TEMPO;
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
	    if(bCharVal < ASCII_MIN || bCharVal > ASCII_MAX)
	    {
			PRINTF("\r\nInvalid choice, pick again!\r\n");
	    }
	}while(bCharVal > ASCII_MIN && bCharVal < ASCII_MAX);

    PRINTF("%d", bCharVal);
    PRINTF("\r\n");

	while(1)
	{
		switch(bCharVal)
		{
			case 1:
				if(dwDly-- == 0)
				{
					vfnTPMChannelDisable(CHANNEL_0, PWM_MODE);	//Disable channel to modify MOD
					rgTPM1->MOD = (baZelda[bCnt]);				//Modify mod to desired frequency (note)
					vfnTPMChannelInnit(CHANNEL_0, PWM_MODE);	//Enable back channel
					bDivider = (baTimeZelda[bCnt]);				//Set delay to note intervals
					if(bDivider > 0)
					{
						dwDly = bWholeNote/bDivider;
					}
					if(bDivider < 0)
					{
						dwDly = bWholeNote/abs(bDivider);
						dwDly *= DOTTED_NOTE;
					}
					bCnt++;
					if(bCnt >= bNotes)
					{
						bCnt = 0;
					}
				}
				break;

			case 2:
				//no song yet
				break;

			case 3:
				//no song yet
				break;
		}
	}
}
