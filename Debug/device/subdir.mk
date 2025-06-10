################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device/system_MK66F18.c 

C_DEPS += \
./device/system_MK66F18.d 

OBJS += \
./device/system_MK66F18.o 


# Each subdirectory must supply rules for building sources it contributes
device/%.o: ../device/%.c device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K66F -DFREEDOM -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -DSDK_OS_BAREMETAL -I"/Users/elian/Documents/MCUXpresso/Examples/Rem_P1/mdio" -I"/Users/elian/Documents/MCUXpresso/Examples/Rem_P1/phy" -I"/Users/elian/Documents/MCUXpresso/Examples/Rem_P1/utilities" -I"/Users/elian/Documents/MCUXpresso/Examples/Rem_P1/drivers" -I"/Users/elian/Documents/MCUXpresso/Examples/Rem_P1/device" -I"/Users/elian/Documents/MCUXpresso/Examples/Rem_P1/component/uart" -I"/Users/elian/Documents/MCUXpresso/Examples/Rem_P1/component/lists" -I"/Users/elian/Documents/MCUXpresso/Examples/Rem_P1/CMSIS" -I"/Users/elian/Documents/MCUXpresso/Examples/Rem_P1/source" -I"/Users/elian/Documents/MCUXpresso/Examples/Rem_P1/board" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-device

clean-device:
	-$(RM) ./device/system_MK66F18.d ./device/system_MK66F18.o

.PHONY: clean-device

