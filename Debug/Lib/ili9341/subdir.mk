################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib/ili9341/fonts.c \
../Lib/ili9341/ili9341.c \
../Lib/ili9341/ili9341_touch.c 

OBJS += \
./Lib/ili9341/fonts.o \
./Lib/ili9341/ili9341.o \
./Lib/ili9341/ili9341_touch.o 

C_DEPS += \
./Lib/ili9341/fonts.d \
./Lib/ili9341/ili9341.d \
./Lib/ili9341/ili9341_touch.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/ili9341/%.o: ../Lib/ili9341/%.c Lib/ili9341/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra -g -DDEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Core\Inc" -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Lib\ili9341" -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Drivers\CMSIS\Include" -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Drivers\CMSIS\Core\Include" -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Drivers\STM32F4xx_HAL_Driver\Inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


