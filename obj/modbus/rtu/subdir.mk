################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../modbus/rtu/mbcrc.c \
../modbus/rtu/mbrtu.c 

OBJS += \
./modbus/rtu/mbcrc.o \
./modbus/rtu/mbrtu.o 

C_DEPS += \
./modbus/rtu/mbcrc.d \
./modbus/rtu/mbrtu.d 


# Each subdirectory must supply rules for building sources it contributes
modbus/rtu/%.o: ../modbus/rtu/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\Debug" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\Core" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\User" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\Peripheral\inc" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\functions" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\include" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\port" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\rtu" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\tcp" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


