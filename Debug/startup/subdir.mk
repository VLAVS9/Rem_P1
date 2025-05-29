################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_mk66f18.c 

C_DEPS += \
./startup/startup_mk66f18.d 

OBJS += \
./startup/startup_mk66f18.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K66F -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\Embebidos\K66\REm_enet_txrx_transfer\source" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\Embebidos\K66\REm_enet_txrx_transfer\mdio" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\Embebidos\K66\REm_enet_txrx_transfer\phy" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\Embebidos\K66\REm_enet_txrx_transfer\utilities" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\Embebidos\K66\REm_enet_txrx_transfer\drivers" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\Embebidos\K66\REm_enet_txrx_transfer\device" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\Embebidos\K66\REm_enet_txrx_transfer\component\uart" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\Embebidos\K66\REm_enet_txrx_transfer\component\lists" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\Embebidos\K66\REm_enet_txrx_transfer\CMSIS" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\Embebidos\K66\REm_enet_txrx_transfer\board" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\Embebidos\K66\REm_enet_txrx_transfer\frdmk66f\driver_examples\enet\txrx_transfer" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-startup

clean-startup:
	-$(RM) ./startup/startup_mk66f18.d ./startup/startup_mk66f18.o

.PHONY: clean-startup

