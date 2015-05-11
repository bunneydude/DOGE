################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/memory_map/memory_map.c 

OBJS += \
./src/memory_map/memory_map.o 

C_DEPS += \
./src/memory_map/memory_map.d 


# Each subdirectory must supply rules for building sources it contributes
src/memory_map/%.o: ../src/memory_map/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__USE_CMSIS=CMSIS_CORE_LPC8xx -DNDEBUG -D__CODE_RED -D__LPC8XX__ -I"/home/bunney/Documents/repos/DOGE/nodes/lpc812/lpc812_nodeCtrl/cmsis" -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


