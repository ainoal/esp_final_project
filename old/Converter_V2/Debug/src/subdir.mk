################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/converter_model.c \
../src/converter_sim.c \
../src/converter_sim_V2.c \
../src/matrix_operations.c \
../src/pi_controller.c 

OBJS += \
./src/converter_model.o \
./src/converter_sim.o \
./src/converter_sim_V2.o \
./src/matrix_operations.o \
./src/pi_controller.o 

C_DEPS += \
./src/converter_model.d \
./src/converter_sim.d \
./src/converter_sim_V2.d \
./src/matrix_operations.d \
./src/pi_controller.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I"C:\Users\ainol\workspaces\final_work\esp_final_project_bsp\ps7_cortexa9_0\include" -I"C:\Users\ainol\workspaces\final_work\zybo_platform" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


