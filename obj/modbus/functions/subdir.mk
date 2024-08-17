################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../modbus/functions/mbfunccoils.c \
../modbus/functions/mbfuncdiag.c \
../modbus/functions/mbfuncdisc.c \
../modbus/functions/mbfuncholding.c \
../modbus/functions/mbfuncinput.c \
../modbus/functions/mbfuncother.c \
../modbus/functions/mbutils.c 

OBJS += \
./modbus/functions/mbfunccoils.o \
./modbus/functions/mbfuncdiag.o \
./modbus/functions/mbfuncdisc.o \
./modbus/functions/mbfuncholding.o \
./modbus/functions/mbfuncinput.o \
./modbus/functions/mbfuncother.o \
./modbus/functions/mbutils.o 

C_DEPS += \
./modbus/functions/mbfunccoils.d \
./modbus/functions/mbfuncdiag.d \
./modbus/functions/mbfuncdisc.d \
./modbus/functions/mbfuncholding.d \
./modbus/functions/mbfuncinput.d \
./modbus/functions/mbfuncother.d \
./modbus/functions/mbutils.d 


# Each subdirectory must supply rules for building sources it contributes
modbus/functions/%.o: ../modbus/functions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\Debug" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\Core" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\User" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\Peripheral\inc" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\functions" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\include" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\port" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\rtu" -I"C:\Users\Bogdan\mrs_community_workspace\CH32V307ModbusRTU\modbus\tcp" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


