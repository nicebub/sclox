################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Unity/examples/example_3/test/TestProductionCode.c \
../src/Unity/examples/example_3/test/TestProductionCode2.c 

OBJS += \
./src/Unity/examples/example_3/test/TestProductionCode.o \
./src/Unity/examples/example_3/test/TestProductionCode2.o 

C_DEPS += \
./src/Unity/examples/example_3/test/TestProductionCode.d \
./src/Unity/examples/example_3/test/TestProductionCode2.d 


# Each subdirectory must supply rules for building sources it contributes
src/Unity/examples/example_3/test/%.o: ../src/Unity/examples/example_3/test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -I/Users/scotty/eclipsewksp2/sclox/src/Unity/src -I"/Users/scotty/eclipsewksp2/classCreate" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


