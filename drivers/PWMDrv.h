/*
 * PWMDvr.h
 *
 *  Created on: 16/02/22
 *      Author: Fernando J. Aragón
 */
/******************************************************************************/
/*Include files                                                             ***/
/******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PWM_CLOCK      ( 48000000 / 128 )                                   // 375 Khz
#define PWM_PERIOD          ( 10 )                                          // 50msec
#define PERIOD_VALUE (( PWM_CLOCK * PWM_PERIOD) / 1000 )                    // PERIOD_VALUE
#define BRIGHTNESS_LVLS (sizeof(LEDControl) / sizeof(LEDControl[0]))        // 

typedef struct
{
    uint16_t Period;
    uint16_t DutyCycleCtrl;
    char Brigtness[5];
}pwm_str;

static pwm_str DutyCycleCtrl[] = {
    //   PERIOD         DUTY CYCLE     % DUTY CYCLE    HIGH (LED_OFF)  LOW (LED_ON)
    PERIOD_VALUE, PWM_DUTY_CYCLE(10),   "10%",     //  90msec         10msec
    PERIOD_VALUE, PWM_DUTY_CYCLE(20),   "20%",
    PERIOD_VALUE, PWM_DUTY_CYCLE(30),   "30%",
    PERIOD_VALUE, PWM_DUTY_CYCLE(40),   "40%",
    PERIOD_VALUE, PWM_DUTY_CYCLE(50),   "50%",
    PERIOD_VALUE, PWM_DUTY_CYCLE(60),   "60%",
    PERIOD_VALUE, PWM_DUTY_CYCLE(70),   "70%",
    PERIOD_VALUE, PWM_DUTY_CYCLE(80),   "80%",
    PERIOD_VALUE, PWM_DUTY_CYCLE(90),   "90%",
    PERIOD_VALUE, PWM_DUTY_CYCLE(100),  "100%",
};

 
/*******************************************************************************
 * Local Function Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Local Variables
 ******************************************************************************/
 
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
void control_LED_brightness(void);
/******************************************************************************/


/*******************************************************************************
 * End of File
 ******************************************************************************/