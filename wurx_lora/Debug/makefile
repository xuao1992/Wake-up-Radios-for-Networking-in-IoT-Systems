################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs810/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./event/event.obj" \
"./intern_temp_sensor/intern_temp_sensor.obj" \
"./misc/misc.obj" \
"./spi/spi.obj" \
"./sx1276/sx1276_driver.obj" \
"./timers/timers.obj" \
"./uart/uart.obj" \
"../lnk_msp430fr5969.cmd" \
$(GEN_CMDS__FLAG) \
-llibmpu_init.a \
-llibmath.a \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include event/subdir_vars.mk
-include intern_temp_sensor/subdir_vars.mk
-include misc/subdir_vars.mk
-include spi/subdir_vars.mk
-include sx1276/subdir_vars.mk
-include timers/subdir_vars.mk
-include uart/subdir_vars.mk
-include subdir_rules.mk
-include event/subdir_rules.mk
-include intern_temp_sensor/subdir_rules.mk
-include misc/subdir_rules.mk
-include spi/subdir_rules.mk
-include sx1276/subdir_rules.mk
-include timers/subdir_rules.mk
-include uart/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
wurx_lora.out \

EXE_OUTPUTS__QUOTED += \
"wurx_lora.out" \

BIN_OUTPUTS += \
wurx_lora.hex \

BIN_OUTPUTS__QUOTED += \
"wurx_lora.hex" \


# All Target
all: wurx_lora.out

# Tool invocations
wurx_lora.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccs810/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/bin/cl430" -vmspx --abi=eabi --data_model=restricted --use_hw_mpy=F5 --advice:power=all --advice:hw_config=all -g --define=__MSP430FR5969__ --define=_MPU_ENABLE --display_error_number --diag_warning=225 --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=full -z -m"wurx_lora.map" --stack_size=160 --heap_size=320 --cinit_hold_wdt=on -i"C:/ti/ccs810/ccsv8/ccs_base/msp430/include" -i"C:/ti/ccs810/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/lib" -i"C:/ti/ccs810/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/include" -i"C:/ti/ccs810/ccsv8/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"C:/ti/ccs810/ccsv8/ccs_base/msp430/lib/FR59xx" --reread_libs --priority --define=_MPU_ENABLE --warn_sections --diag_wrap=off --display_error_number --xml_link_info="wurx_lora_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "wurx_lora.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

wurx_lora.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: MSP430 Hex Utility'
	"C:/ti/ccs810/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/bin/hex430" --memwidth=8 --romwidth=8 -o "wurx_lora.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "event\event.obj" "intern_temp_sensor\intern_temp_sensor.obj" "misc\misc.obj" "spi\spi.obj" "sx1276\sx1276_driver.obj" "timers\timers.obj" "uart\uart.obj" 
	-$(RM) "main.d" "event\event.d" "intern_temp_sensor\intern_temp_sensor.d" "misc\misc.d" "spi\spi.d" "sx1276\sx1276_driver.d" "timers\timers.d" "uart\uart.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

