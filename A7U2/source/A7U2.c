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
    (void (*)(void))vfnReadADC, 		//READ ADC VALUE
    (void (*)(void))vfnConvert,   		//CONVERT V TO TEMPERATURE
    (void (*)(void))vfnRefresh			//PRINT
};

/* Global Var */
GPIO_Type *rGpioB = GPIOB;          /* Peripheral GPIOB base address */
uint16_t wVolt = 0;
uint16_t wTemp = 0;
uint8_t bCurrentState = STATE_READ;

//EDIT


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
    vfnPIT1Init(TIMER_VALUE_2s);
//    vfnPWM_Init();

    while(1)
    {
    	vfnBluBlink();
    	vfnSM();
    }
}

void vfnSM()
{
	vfnTmpStates[bCurrentState]();
}

void vfnReadADC(void)
{
	static uint16_t bCnt = 0;
	if(bCnt < SAMPLES_TAKEN)
	{
		bCnt++;
		wVolt += bfnADC_Read(ADC_CHANNEL_0);
		bCurrentState = STATE_READ;
	}
	else
	{
		bCnt = 0;
		bCurrentState = STATE_CONVERT;
	}
}

void vfnConvert(void)
{
	wVolt /= SAMPLES_TAKEN;
	wTemp = bfnConvert_V2Temp(wVolt);
	bCurrentState = STATE_REFRESH;
}

void vfnRefresh(void)
{
	if(bfnPITCheck(PIT_CHANNEL_1))
	{
		PRINTF("TEMP: %i\n\t", wTemp);
		bCurrentState = STATE_READ;
	}
	else
	{
		bCurrentState = STATE_REFRESH;
	}
}

void vfnBluBlink()
{
	if(bfnPITCheck(PIT_CHANNEL_0))
	{
		rGpioB->PTOR |= 1 << PIN_B0;
	}
}
