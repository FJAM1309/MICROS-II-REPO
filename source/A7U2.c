// * @file    A7U2.c
// * @brief   Application entry point.
// */
//#include <stdio.h>
//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "MKL25Z4.h"
//#include "fsl_debug_console.h"
///* TODO: insert other include files here. */
//
///* TODO: insert other definitions and declarations here. */
//
///*
// * @brief   Application entry point.
// */
//int main(void) {
//
//    /* Init board hardware. */
//    BOARD_InitBootPins();
//    BOARD_InitBootClocks();
//    BOARD_InitBootPeripherals();
//#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
//    /* Init FSL debug console. */
//    BOARD_InitDebugConsole();
//#endif
//
//    PRINTF("Hello World\n");
//
//    /* Force the counter to be placed into memory. */
//    volatile static int i = 0 ;
//    /* Enter an infinite loop, just incrementing a counter. */
//    while(1) {
//        i++ ;
//        /* 'Dummy' NOP to allow source level single stepping of
//            tight while() loop */
//        __asm volatile ("nop");
//    }
//    return 0 ;
//}

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
#include "A7U2.h"
#include "ADCDrv.h"
#include "DebounceDrv.h"
#include "LEDDrv.h"
#include "PITDrv.h"
#include "PWMDrv.h"

static void (*vfnTmpStates[])(void) =
{
    (uint8_t (*)(uint8_t bChannel))bfnADC_Read, 		//READ ADC VALUE
    (uint8_t (*)(uint8_t bVoltage))bfnConvert_V2Temp,   //CONVERT V TO TEMPERATURE
    (void (*)(uint16_t wTemp))vfnRefresh							//PRINT
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
    vfnLEDsInit();
    vfnADC_Innit();
    vfnADC_Channel_Innit();
    vfnbuttonInit();
    vfnPIT0Init(TIMER_VALUE_700ms);
//    vfnPWM_Init();


    volatile uint8_t bCnt = 0;
    while(1)
    {
    	vfnBluBlink();
    	vfnTmpStates[bCurrentState]();

    	if(bCnt < SAMPLES_TAKEN)
    	{
        	wTemp = bfnConvert_V2Temp(bfnADC_Read(ADC_Channel_0));
        	++bCnt;
    	}
    	else
    	{
			PRINTF("TEMP: %i\n", wTemp);
			bCnt = 0;
    	}
    }
}

void vfnBluBlink()
{
	if(bfnPIT0Check())
	{
		rGpioB->PTOR |= 1 << PIN_B0;
	}
}

void vfnRefresh(uint16_t wTemp)
{
	PRINTF("TEMP: %i\n\t", wTemp);
	bCurrentState = STATE_READ;
}

