################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/common_funcs.c 

OBJS += \
./common/common_funcs.o 

C_DEPS += \
./common/common_funcs.d 


# Each subdirectory must supply rules for building sources it contributes
common/%.o: ../common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC C Compiler'
	arm-none-eabi-gcc -O0 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -funsigned-bitfields -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


