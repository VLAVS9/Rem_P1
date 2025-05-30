################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c \
../utilities/fsl_debug_console.c 

C_DEPS += \
./utilities/fsl_assert.d \
./utilities/fsl_debug_console.d 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_debug_console.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K66F -DFREEDOM -DMCUXPRESSO_SDK -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\RedesEm\PracticasK66\Rem_P1\source" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\RedesEm\PracticasK66\Rem_P1\mdio" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\RedesEm\PracticasK66\Rem_P1\phy" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\RedesEm\PracticasK66\Rem_P1\utilities" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\RedesEm\PracticasK66\Rem_P1\drivers" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\RedesEm\PracticasK66\Rem_P1\device" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\RedesEm\PracticasK66\Rem_P1\component\uart" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\RedesEm\PracticasK66\Rem_P1\component\lists" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\RedesEm\PracticasK66\Rem_P1\CMSIS" -I"C:\Users\vonta\OneDrive\Documentos\Desarrollo\RedesEm\PracticasK66\Rem_P1\board" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o ./utilities/fsl_debug_console.d ./utilities/fsl_debug_console.o

.PHONY: clean-utilities

