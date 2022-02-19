/*
 * PWMDvr.c
 *
 *  Created on: 16/02/22
 *      Author: Fernando J. Aragón
 */
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "MKL25Z4.h"
#include "PWMDrv.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define     MCGFLLCLK           ( 1 )       //Macro for selecting the MCGFLLCLK clock option in TPMSRC clock select option
#define     MUX_ALT_3           ( 3 )       //MUX alt 3 is set for TPM1_CH1
#define     PWM_PIN             ( 21 )      //Pin number used for the PWM signal
#define     DBGMODE_MASK        ( 3 << 6 )  //Mask to activate debug mode
/*******************************************************************************
 * Local Function Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Local Variables
 ******************************************************************************/
//typedef struct
//{
//    uint16_t Period;
//    uint16_t DutyCycleCtrl;
//    char Brigtness[5];
//}pwm_str;
//
//static pwm_str DutyCycleCtrl[] = {
//    //   PERIOD         DUTY CYCLE     % DUTY CYCLE    HIGH (LED_OFF)  LOW (LED_ON)
//    PERIOD_VALUE, PWM_DUTY_CYCLE(10),   "10%",     //  90msec         10msec
//    PERIOD_VALUE, PWM_DUTY_CYCLE(20),   "20%",
//    PERIOD_VALUE, PWM_DUTY_CYCLE(30),   "30%",
//    PERIOD_VALUE, PWM_DUTY_CYCLE(40),   "40%",
//    PERIOD_VALUE, PWM_DUTY_CYCLE(50),   "50%",
//    PERIOD_VALUE, PWM_DUTY_CYCLE(60),   "60%",
//    PERIOD_VALUE, PWM_DUTY_CYCLE(70),   "70%",
//    PERIOD_VALUE, PWM_DUTY_CYCLE(80),   "80%",
//    PERIOD_VALUE, PWM_DUTY_CYCLE(90),   "90%",
//    PERIOD_VALUE, PWM_DUTY_CYCLE(100),  "100%",
//};
/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * vfnPWM_Init
 * DESCRIPTION: Initializes the pin, clock and debug mode 
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnPWM_Init()
{
    SIM_Type *rclkSIM_gpio = SIM;
    PORT_Type *rPortE = PORTE;
    // Clock enable of the peripherals in SIM
    //Enable PORTE
    rclkSIM_gpio->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    //Set PORTB pin 18 as output for the PWM        
    rPortE->PCR[PWM_PIN] |= PORT_PCR_MUX(MUX_ALT_3);    

    //Select MCGFLLCLK as clock 
    rclkSIM_gpio->SOPT2 |= SIM_SOPT2_TPMSRC(MCGFLLCLK);
    //Enable PWM channel 1
    rclkSIM_gpio->SCGC6 |= SIM_SCGC6_TPM1_MASK;         

    TPM_Type *rTPM1_base = TPM1;
    TPM_Type *rTPM2_base = TPM2;

    //PWM configured as edge aligned
    rTPM1_base->SC &= ~(TPM_SC_CPWMS_MASK);
    rTPM1_base->SC |= TPM_SC_CMOD(1);
    rTPM1_base->SC |= TPM_SC_PS_MASK;

    rTPM1_base->CONF = DBGMODE_MASK;                            // Set debug mode to increment the counter
    //Configured as Edge Aligned PWM
    rTPM2_base->CONTROLS->CnSC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0) |
                  TPM_CnSC_MSB(1)  | TPM_CnSC_MSA(0));
}

//void control_LED_brightness(void)
//{
//    static uint8_t brightness_lvl = BRIGHTNESS_LVLS;
//
//    brightness_lvl++;
//    if (brightness_lvl >= BRIGHTNESS_LVLS)
//    {
//        brightness_lvl = 0;
//    }
//    TPM1_CNT = 0;
//    TPM1_MOD = LEDControl[brightness_lvl].Period;
//    TPM1_C0V = LEDControl[brightness_lvl].DutyCycleCtrl;
//}
