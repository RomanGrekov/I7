################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include usart_funcs/subdir.mk
-include startup/subdir.mk
-include sim900/subdir.mk
-include libs/subdir.mk
-include kb_driver/subdir.mk
-include hd44780_driver/subdir.mk
-include delay/subdir.mk
-include common/subdir.mk
-include clock/subdir.mk
-include USART/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
I7_v1_0.hex \

SECONDARY_LIST += \
I7_v1_0.lst \

SECONDARY_SIZE += \
I7_v1_0.siz \


# All Target
all: I7_v1_0.elf secondary-outputs

# Tool invocations
I7_v1_0.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Linux GCC C Linker'
	arm-none-eabi-gcc -T"/home/romang/Projects/ARM/I7_v1_0/startup/stm32_flash.ld" -Xlinker --gc-sections -Wl,-Map,I7_v1_0.map -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o "I7_v1_0.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

I7_v1_0.hex: I7_v1_0.elf
	@echo 'Invoking: ARM Sourcery Linux GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex I7_v1_0.elf  "I7_v1_0.hex"
	@echo 'Finished building: $@'
	@echo ' '

I7_v1_0.lst: I7_v1_0.elf
	@echo 'Invoking: ARM Sourcery Linux GNU Create Listing'
	arm-none-eabi-objdump -h -S I7_v1_0.elf > "I7_v1_0.lst"
	@echo 'Finished building: $@'
	@echo ' '

I7_v1_0.siz: I7_v1_0.elf
	@echo 'Invoking: ARM Sourcery Linux GNU Print Size'
	arm-none-eabi-size  --format=berkeley I7_v1_0.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) I7_v1_0.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
