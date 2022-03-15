################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/LEDDrv.c \
../drivers/PITDrv.c \
../drivers/PWMDrv.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_flash.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpsci.c \
../drivers/fsl_smc.c \
../drivers/fsl_tpm.c \
../drivers/fsl_uart.c 

OBJS += \
./drivers/LEDDrv.o \
./drivers/PITDrv.o \
./drivers/PWMDrv.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_flash.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpsci.o \
./drivers/fsl_smc.o \
./drivers/fsl_tpm.o \
./drivers/fsl_uart.o 

C_DEPS += \
./drivers/LEDDrv.d \
./drivers/PITDrv.d \
./drivers/PWMDrv.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_flash.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpsci.d \
./drivers/fsl_smc.d \
./drivers/fsl_tpm.d \
./drivers/fsl_uart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DDEBUG -DFRDM_KL25Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\frdmkl25z_driver_examples_uart_interrupt\board" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\frdmkl25z_driver_examples_uart_interrupt\source" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\frdmkl25z_driver_examples_uart_interrupt" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\frdmkl25z_driver_examples_uart_interrupt\CMSIS" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\frdmkl25z_driver_examples_uart_interrupt\drivers" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\frdmkl25z_driver_examples_uart_interrupt\utilities" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\frdmkl25z_driver_examples_uart_interrupt\startup" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


