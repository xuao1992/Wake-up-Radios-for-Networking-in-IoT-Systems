################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
uart/uart.obj: ../uart/uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs810/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/bin/cl430" -vmspx --abi=eabi --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccs810/ccsv8/ccs_base/msp430/include" --include_path="C:/Users/ax1a17/git/Wake-up-Radios-for-Networking-in-IoT-Systems/wurx_lora/intern_temp_sensor" --include_path="C:/Users/ax1a17/git/Wake-up-Radios-for-Networking-in-IoT-Systems/wurx_lora/mac" --include_path="C:/Users/ax1a17/git/Wake-up-Radios-for-Networking-in-IoT-Systems/wurx_lora/misc" --include_path="C:/Users/ax1a17/git/Wake-up-Radios-for-Networking-in-IoT-Systems/wurx_lora/uart" --include_path="C:/Users/ax1a17/git/Wake-up-Radios-for-Networking-in-IoT-Systems/wurx_lora/wurx" --include_path="C:/Users/ax1a17/git/Wake-up-Radios-for-Networking-in-IoT-Systems/wurx_lora" --include_path="C:/Users/ax1a17/git/Wake-up-Radios-for-Networking-in-IoT-Systems/wurx_lora/timers" --include_path="C:/Users/ax1a17/git/Wake-up-Radios-for-Networking-in-IoT-Systems/wurx_lora/spi" --include_path="C:/Users/ax1a17/git/Wake-up-Radios-for-Networking-in-IoT-Systems/wurx_lora/event" --include_path="C:/Users/ax1a17/git/Wake-up-Radios-for-Networking-in-IoT-Systems/wurx_lora/sx1276" --include_path="C:/ti/ccs810/ccsv8/tools/compiler/ti-cgt-msp430_18.1.2.LTS/include" --advice:power=all --advice:hw_config=all -g --define=__MSP430FR5969__ --define=_MPU_ENABLE --display_error_number --diag_warning=225 --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=full --preproc_with_compile --preproc_dependency="uart/uart.d_raw" --obj_directory="uart" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


