#include "board.h"
#include "fsl_uart.h"
#include "PITDrv.h"
#include "PWMDrv.h"
#include"LEDDrv.h"
#include "fsl_debug_console.h"
#include "fsl_tpm.h"
#include "stdio.h"
#include "stdlib.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*Colors*/
#define R 0x52 //'R'
#define G 0x47 //'G'
#define B 0x42 //'B'

/*Tpm*/
#define CHANNEL_1 1U
#define CHANNEL_2 2U

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl1InterruptEnable
#define TPM_CHANNEL_FLAG kTPM_Chnl1Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM2_IRQn
#define TPM_LED_HANDLER TPM2_IRQHandler

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)

/* UART instance and clock */
#define DEMO_UART UART0
#define DEMO_UART_CLKSRC BUS_CLK
#define DEMO_UART_CLK_FREQ CLOCK_GetFreq(BUS_CLK)
#define DEMO_UART_IRQn UART0_IRQn
#define DEMO_UART_IRQHandler UART0_IRQHandler

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 3

/*! @brief Ring buffer to save received data. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool brightnessUp = true; /* Indicate LED is brighter or dimmer */
volatile uint8_t updatedDutycycle = 10U;
volatile uint8_t getCharValue = 0U;

uint8_t g_tipString[] =
    "Uart functional API interrupt example\r\nBoard receives characters then sends them out\r\nNow please input:\r\n";

uint8_t g_SelectionMsg[] =
    "\r\nInput was: ";

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
  Ring buffer empty: (rxIndex == txIndex)
*/
uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */
uint8_t bColor;
uint8_t bNum1;
uint8_t bNum2;
uint8_t bDuty;
uint8_t bFlag;
/*******************************************************************************
 * Code
 ******************************************************************************/

uint8_t bfnCombine(uint8_t a, uint8_t b)
{

    char s1[1];
    char s2[1];

    a -= 48;
    b -= 48;
    // Convert both the integers to string
    sprintf(s1, "%d", a);
    sprintf(s2, "%d", b);

    // Concatenate both strings
    strcat(s1, s2);

    // Convert the concatenated string
    // to integer
    uint8_t String2Int = atoi(s1);

    // return the formed integer
    return String2Int;
}

void DEMO_UART_IRQHandler(void)
{
    uint8_t data;

    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(DEMO_UART))
    {
        data = UART_ReadByte(DEMO_UART);
        /* If ring buffer is not full, add data to ring buffer. */
        if (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) != txIndex)
        {
            demoRingBuffer[rxIndex] = data;
            rxIndex++;
            rxIndex %= DEMO_RING_BUFFER_SIZE;
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    vfnPIT0Init(ONE_SEC); 		//CONFIGURE PIT TIMER FOR 1 SEC
    /* Board pin, clock, debug console init */
    BOARD_InitPins();			//INIT PINS
    BOARD_BootClockRUN();		//INIT CLKS
    BOARD_InitDebugConsole();	//INIT DEBUG CONSOLE

    uart_config_t config;

    tpm_config_t tpmInfo;

    tpm_chnl_pwm_signal_param_t tpmParamChannel0;	//PARAMETER FOR CHANNEL 0
    tpm_chnl_pwm_signal_param_t tpmParamChannel1;	//PARAMETER FOR CHANNEL 1

    tpm_pwm_level_select_t pwmLevel = kTPM_LowTrue;

    /* Configure tpm params with frequency 24kHZ for Channel 0 & 1 */
    tpmParamChannel0.chnlNumber = (tpm_chnl_t)CHANNEL_0;
    tpmParamChannel1.chnlNumber = (tpm_chnl_t)CHANNEL_1;
    tpmParamChannel0.level = pwmLevel;
    tpmParamChannel1.level = pwmLevel;
    tpmParamChannel0.dutyCyclePercent = updatedDutycycle;
    tpmParamChannel1.dutyCyclePercent = updatedDutycycle;

    /* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
    CLOCK_SetTpmClock(1U);

    /* Select default configuration */
    TPM_GetDefaultConfig(&tpmInfo);

    /* Initialize TPM0 & TPM2 module */
    TPM_Init(TPM0, &tpmInfo);
    TPM_SetupPwm(TPM0, &tpmParamChannel1, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
    TPM_StartTimer(TPM0, kTPM_SystemClock);

    TPM_Init(TPM2, &tpmInfo);
    TPM_SetupPwm(TPM2, &tpmParamChannel0, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
    TPM_SetupPwm(TPM2, &tpmParamChannel1, 1U, kTPM_CenterAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
    TPM_StartTimer(TPM2, kTPM_SystemClock);

    UART_GetDefaultConfig(&config);
    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;

    /* Send g_tipString out. */
    UART_WriteBlocking(DEMO_UART, g_tipString, sizeof(g_tipString) / sizeof(g_tipString[0]));
    /* Enable RX interrupt. */
    UART_EnableInterrupts(DEMO_UART, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    EnableIRQ(DEMO_UART_IRQn);

    while (1)
    {
        /* Send data only when UART TX register is empty and ring buffer has data to send out. */
        while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(DEMO_UART)) && (rxIndex != txIndex))
        {
            UART_WriteByte(DEMO_UART, demoRingBuffer[txIndex]);
            txIndex++;
            txIndex %= DEMO_RING_BUFFER_SIZE;
        }

        bColor = demoRingBuffer[0];						//SEPARATE LETTER FROM ARRAY
        bNum1 = demoRingBuffer[1];						//FIRST NUMBER
		bNum2 = demoRingBuffer[2];						//SECOND NUMBER
		updatedDutycycle = bfnCombine(bNum1, bNum2);	//COMBINE NUMBERS INTO AN INT

		if(bfnPITCheck(0))
		{
			UART_WriteBlocking(DEMO_UART, g_SelectionMsg, sizeof(g_SelectionMsg) / sizeof(g_SelectionMsg[0]));
			UART_WriteBlocking(DEMO_UART, demoRingBuffer, sizeof(demoRingBuffer) / sizeof(demoRingBuffer[0]));
			//WRITE SELECTION
			if(bColor == R) //RED SELECTED
			{
				/* TURN OFF CHANNELS*/
				TPM_UpdateChnlEdgeLevelSelect(TPM0, (tpm_chnl_t)CHANNEL_1, 0U); //BLUE
				TPM_UpdateChnlEdgeLevelSelect(TPM2, (tpm_chnl_t)CHANNEL_1, 0U); //GRN

				TPM_UpdateChnlEdgeLevelSelect(TPM2, (tpm_chnl_t)CHANNEL_0, 0U); //RED

				/* UPDATE DUTY CYCLE FOR SELECTED CHANNEL */
				TPM_UpdatePwmDutycycle(TPM2, (tpm_chnl_t)CHANNEL_0, kTPM_CenterAlignedPwm,
						updatedDutycycle); //RED

				/*TURN ON SELECTED CHANNEL*/
				TPM_UpdateChnlEdgeLevelSelect(TPM2, (tpm_chnl_t)CHANNEL_0, pwmLevel); //RED
			}
			else if(bColor == G)
			{
				TPM_UpdateChnlEdgeLevelSelect(TPM0, (tpm_chnl_t)CHANNEL_1, 0U); //BLUE
				TPM_UpdateChnlEdgeLevelSelect(TPM2, (tpm_chnl_t)CHANNEL_0, 0U); //RED

				TPM_UpdateChnlEdgeLevelSelect(TPM2, (tpm_chnl_t)CHANNEL_1, 0U); //GRN

				TPM_UpdatePwmDutycycle(TPM2, (tpm_chnl_t)CHANNEL_1, kTPM_CenterAlignedPwm,
								updatedDutycycle);

				TPM_UpdateChnlEdgeLevelSelect(TPM2, (tpm_chnl_t)CHANNEL_1, pwmLevel); //GRN
			}
			else if(bColor == B)
			{
				TPM_UpdateChnlEdgeLevelSelect(TPM2, (tpm_chnl_t)CHANNEL_0, 0U); //RED
				TPM_UpdateChnlEdgeLevelSelect(TPM2, (tpm_chnl_t)CHANNEL_1, 0U); //GRN

				TPM_UpdateChnlEdgeLevelSelect(TPM0, (tpm_chnl_t)CHANNEL_1, 0U); //BLUE

				TPM_UpdatePwmDutycycle(TPM0, (tpm_chnl_t)CHANNEL_1, kTPM_CenterAlignedPwm,
						updatedDutycycle);

				TPM_UpdateChnlEdgeLevelSelect(TPM0, (tpm_chnl_t)CHANNEL_1, pwmLevel); //BLU
			}
		}
    }
}
