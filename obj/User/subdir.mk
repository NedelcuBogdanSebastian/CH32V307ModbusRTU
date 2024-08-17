################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/main.c \
../User/system_ch32v30x.c 

OBJS += \
./User/ch32v30x_it.o \
./User/main.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/ch32v30x_it.d \
./User/main.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\Debug" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\Core" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\User" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\Peripheral\inc" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\functions" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\include" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\port" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\rtu" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\tcp" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


