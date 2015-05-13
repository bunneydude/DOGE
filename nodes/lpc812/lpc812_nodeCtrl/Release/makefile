################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/task/subdir.mk
-include src/protocol/subdir.mk
-include src/platform/subdir.mk
-include src/nrf24/subdir.mk
-include src/network/subdir.mk
-include src/memory_map/subdir.mk
-include src/gpio/subdir.mk
-include src/dsp/subdir.mk
-include src/cobs/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: lpc812_nodeCtrl.axf

# Tool invocations
lpc812_nodeCtrl.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="lpc812_nodeCtrl.map" -Xlinker --gc-sections -mcpu=cortex-m0 -mthumb -T "LeafNode_blink_nRF_test_Release.ld" -o "lpc812_nodeCtrl.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) lpc812_nodeCtrl.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "lpc812_nodeCtrl.axf"; arm-none-eabi-objcopy -O binary "lpc812_nodeCtrl.axf" "lpc812_nodeCtrl.bin" ; arm-none-eabi-objcopy -O ihex "lpc812_nodeCtrl.axf" "lpc812_nodeCtrl.hex" ; checksum -p LPC812 -d "lpc812_nodeCtrl.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
