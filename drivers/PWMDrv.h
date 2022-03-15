/*
 * PWMDvr.h
 *
 *  Created on: 16/02/22
 *      Author: Fernando J. Aragón
 */
/******************************************************************************/
/*Include files                                                             ***/
/******************************************************************************/
#include <stdint.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PIN_E20             ( 20 )
#define CHANNEL_0           ( 0 )
#define CLOCK_PLLFLL        ( 1 )
#define CLOCK_OSCERCLK      ( 2 )
#define CLOCK_MCGIRCLK      ( 3 )
#define MODULE              ( 100 )
#define DUTY                (MODULE/2) //50%
#define CLOCK_INT           ( 1 )
#define TOIE                ( 0 )
#define UPDOWN_COUNTING     ( 1 )
#define PWM_MODE            (TPM_CnSC_MSB_MASK|TPM_CnSC_ELSB_MASK)
#define INCAP_MODE          (TPM_CnSC_ELSA_MASK|TPM_CnSC_CHIE_MASK)
#define ELSA                (TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK)
#define TOTAL_BRIGHTNESS    (sizeof(LEDControl)/sizeof(LEDControl[0]))
//#define TPM_SOURCE_CLOCK    ( 48000000u )
#define CONVERSION_TIME     ( 10000 )
#define TEMPO               ( 108 )
#define DOTTED_NOTE         ( 1.5 )

/*Prescale values*/
#define PRE_1       ( 0 )
#define PRE_2       ( 1 )
#define PRE_4       ( 2 )
#define PRE_8       ( 3 )
#define PRE_16      ( 4 )
#define PRE_32      ( 5 )
#define PRE_64      ( 6 )
#define PRE_128     ( 7 )

//#define PWM_CLOCK      (48000000 / 128)    // 375 Khz
//#define PWM_PERIOD          10                 // 50msec
//#define PWM_PERIOD_VAL ((PWM_CLOCK * PWM_PERIOD) / 1000) // PWM_PERIOD_VAL
//
//#define PWM_DUTY_CYCLE(x) (PWM_PERIOD_VAL - (PWM_PERIOD_VAL / x))
//
//typedef struct
//{
//    uint16_t Period;
//    uint16_t DutyCycle;
//    char Brigtness[5];
//}pwm_str;
//
//static pwm_str LEDControl[] = {
//    //   PERIOD         DUTY CYCLE     % DUTY CYCLE    HIGH (LED_OFF)  LOW (LED_ON)
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(10),   "10%",     //  90msec         10msec
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(20),   "20%",
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(30),   "30%",
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(40),   "40%",
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(50),   "50%",
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(60),   "60%",
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(70),   "70%",
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(85),   "85%",
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(90),   "90%",
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(92),   "92%",
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(94),   "94%",
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(96),   "96%",
//    PWM_PERIOD_VAL, PWM_DUTY_CYCLE(100),  "100%",
//};

 /******************************************************************************
 * Extern Variables Section
 ******************************************************************************/
 
 /******************************************************************************
 * Extern Constants Section
 ******************************************************************************/

/*******************************************************************************
 * Function Prototypes Section
 ******************************************************************************/
void vfnPWM_Init();
void vfnTPMChannelDisable(uint32_t dwChNum, uint32_t dwMode);
void vfnPWMConfig(uint32_t dwModule, uint32_t dwPrescale, uint8_t bCntType);
void vfnTPMChannelInnit(uint32_t dwChNum, uint32_t dwMode);
void vfnFuncPWM(uint32_t dwChNum, uint32_t dwValue);
/******************************************************************************/


/*******************************************************************************
 * End of File
 ******************************************************************************/
