################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/peripherals.c \
../board/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/peripherals.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/peripherals.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\PWMUART_TEST\board" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\PWMUART_TEST\source" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\PWMUART_TEST" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\PWMUART_TEST\startup" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\PWMUART_TEST\CMSIS" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\PWMUART_TEST\drivers" -I"C:\Users\lfara\Documents\MCUXpressoIDE_11.4.1_6260\workspace\PWMUART_TEST\utilities" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


