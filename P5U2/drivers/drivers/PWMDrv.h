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
#define PIN_E20				( 20 )
#define CHANNEL_0           ( 0 )
#define CLOCK_PLLFLL        ( 1 )
#define CLOCK_OSCERCLK      ( 2 )
#define CLOCK_MCGIRCLK      ( 3 )
#define MODULE              ( 16000 )
#define DUTY                (MODULE * 0.9) //90%
#define CLOCK_INT           ( 1 )
#define TOIE                ( 0 )
#define UPDOWN_COUNTING     ( 1 )
#define PWM_MODE            (TPM_CnSC_MSB_MASK|TPM_CnSC_ELSB_MASK)
#define INCAP_MODE          (TPM_CnSC_ELSA_MASK|TPM_CnSC_CHIE_MASK)
#define ELSA                (TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK)
#define TOTAL_BRIGHTNESS	(sizeof(LEDControl)/sizeof(LEDControl[0]))
#define TPM_SOURCE_CLOCK 	( 48000000u )
#define CONVERSION_TIME 	( 10000 )
#define TEMPO 				( 108 )
#define DOTTED_NOTE			( 1.5 )

/*Prescale values*/
#define PRE_1       ( 0 )
#define PRE_2       ( 1 )
#define PRE_4       ( 2 )
#define PRE_8       ( 3 )
#define PRE_16      ( 4 )
#define PRE_32      ( 5 )
#define PRE_64      ( 6 )
#define PRE_128     ( 7 )

/*NOTES*/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

/*******************************************************************************
 * Local Function Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Local Variables
 ******************************************************************************/
//typedef struct
//{
//    uint16_t Period;
//    uint16_t DutyCycle;
//    char Brigtness[5];
//}pwm_str;

//static pwm_str LEDControl[] = {
//    //   PERIOD         DUTY CYCLE     % DUTY CYCLE    HIGH (LED_OFF)  LOW (LED_ON)
//    MODULE, DUTY,   "10%",     //  90msec         10msec
//    MODULE, DUTY,   "20%",
//    MODULE, DUTY,   "30%",
//    MODULE, DUTY,   "40%",
//    MODULE, DUTY,   "50%",
//    MODULE, DUTY,   "60%",
//    MODULE, DUTY,   "70%",
//    MODULE, DUTY,   "85%",
//    MODULE, DUTY,   "90%",
//    MODULE, DUTY,  "100%",
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
