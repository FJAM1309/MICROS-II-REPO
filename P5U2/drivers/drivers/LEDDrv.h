/*
 * LEDDvr.h
 *
 *  Created on: Jan 16, 2022
 *      Author: Mario Castañeda
 */


#ifndef LEDDVR_H_
#define LEDDVR_H_
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "clock_config.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_GPIO     BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_PIN

//Port B
#define	RED		( 18 )
#define	GRN		( 19 )
#define PIN_B0  ( 0 )
#define PIN_B1  ( 1 )
#define PIN_B2  ( 2 )
#define PIN_B3  ( 3 )
//Port C
#define PIN_C1	( 1 )
#define PIN_C2	( 2 )
//Port D
#define	BLU		( 1 )

/*******************************************************************************
 * Local Function Prototypes
 ******************************************************************************/
void vfnLEDsInit(void);
void vfnLEDRed(void);
void vfnLEDGreen(void);
void vfnLEDBlue(void);
void vfnLEDsOff(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/


#endif /* LEDDVR_H_ */
