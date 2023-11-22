################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/btn_setup.c \
../src/converter_model.c \
../src/pi_controller.c \
../src/pi_controller_gen.c \
../src/rtos_converter.c \
../src/state_machine.c \
../src/timer_setup.c \
../src/uart_setup.c \
../src/user_actions.c 

OBJS += \
./src/btn_setup.o \
./src/converter_model.o \
./src/pi_controller.o \
./src/pi_controller_gen.o \
./src/rtos_converter.o \
./src/state_machine.o \
./src/timer_setup.o \
./src/uart_setup.o \
./src/user_actions.o 

C_DEPS += \
./src/btn_setup.d \
./src/converter_model.d \
./src/pi_controller.d \
./src/pi_controller_gen.d \
./src/rtos_converter.d \
./src/state_machine.d \
./src/timer_setup.d \
./src/uart_setup.d \
./src/user_actions.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I"C:\Users\z103112\workspace\esp_final_project\rtos_IRQ_bsp\ps7_cortexa9_0\include" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


