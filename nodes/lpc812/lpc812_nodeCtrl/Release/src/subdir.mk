################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc8xx.c \
../src/main.c \
../src/mrt.c \
../src/printf-retarget.c \
../src/printf.c \
../src/pwm.c \
../src/spi.c \
../src/system_LPC8xx.c \
../src/uart.c 

OBJS += \
./src/cr_startup_lpc8xx.o \
./src/main.o \
./src/mrt.o \
./src/printf-retarget.o \
./src/printf.o \
./src/pwm.o \
./src/spi.o \
./src/system_LPC8xx.o \
./src/uart.o 

C_DEPS += \
./src/cr_startup_lpc8xx.d \
./src/main.d \
./src/mrt.d \
./src/printf-retarget.d \
./src/printf.d \
./src/pwm.d \
./src/spi.d \
./src/system_LPC8xx.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__USE_CMSIS=CMSIS_CORE_LPC8xx -DNDEBUG -D__CODE_RED -D__LPC8XX__ -I"/home/bunney/Documents/repos/DOGE/nodes/lpc812/lpc812_nodeCtrl/cmsis" -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


