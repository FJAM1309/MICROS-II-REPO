/**
 * @file    ADC_Test.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "ADCDrv.h"
/* TODO: insert other definitions and declarations here. */
#define ADC_Channel_0 	( 0 )
#define SAMPLES_TAKEN	( 10 )

/* Structs */

typedef enum
{
    STATE_READ,
    STATE_CONVERT,
    STATE_WAIT,
    STATE_WRITE
}states;

static void (* vfnTmpStates[])(void) =   //arreglo de apuntadores LM35
{
    (uint8_t (*)(uint8_t bChannel))bfnADC_Read, //READ ADC VALUE
    (uint8_t (*)(uint8_t bVoltage))bfnConvert_V2Temp,   //CONVERT V TO TEMPERATURE
    (void (*)(void))vfnLCDWaitState,
    (void (*)(void))bShowLCD
};

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    vfnADC_Innit();
    vfnADC_Channel_Innit();
    uint16_t wTemp = 0;
    volatile uint8_t bCnt = 0;
    while(1)
    {
    	if(bCnt < SAMPLES_TAKEN)
    	{
        	wTemp = bfnConvert_V2Temp(bfnADC_Read(ADC_Channel_0), SAMPLES_TAKEN);
        	++bCnt;
    	}
    	else
    	{
			PRINTF("TEMP: %i\n", wTemp);
			bCnt = 0;
    	}
    }
}
