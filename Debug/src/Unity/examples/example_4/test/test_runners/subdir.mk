################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Unity/examples/example_4/test/test_runners/TestProductionCode2_Runner.c \
../src/Unity/examples/example_4/test/test_runners/TestProductionCode_Runner.c 

OBJS += \
./src/Unity/examples/example_4/test/test_runners/TestProductionCode2_Runner.o \
./src/Unity/examples/example_4/test/test_runners/TestProductionCode_Runner.o 

C_DEPS += \
./src/Unity/examples/example_4/test/test_runners/TestProductionCode2_Runner.d \
./src/Unity/examples/example_4/test/test_runners/TestProductionCode_Runner.d 


# Each subdirectory must supply rules for building sources it contributes
src/Unity/examples/example_4/test/test_runners/%.o: ../src/Unity/examples/example_4/test/test_runners/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -I/Users/scotty/eclipsewksp2/sclox/src/Unity/src -I"/Users/scotty/eclipsewksp2/classCreate" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


