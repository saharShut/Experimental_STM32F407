################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra -g -DDEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Core\Inc" -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Lib\ili9341" -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Drivers\CMSIS\Include" -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Drivers\CMSIS\Core\Include" -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\Users\Alex\eclipse-workspace\LVGS_Test\Drivers\STM32F4xx_HAL_Driver\Inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


