################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Unity/test/tests/testparameterized.c \
../src/Unity/test/tests/testunity.c 

OBJS += \
./src/Unity/test/tests/testparameterized.o \
./src/Unity/test/tests/testunity.o 

C_DEPS += \
./src/Unity/test/tests/testparameterized.d \
./src/Unity/test/tests/testunity.d 


# Each subdirectory must supply rules for building sources it contributes
src/Unity/test/tests/%.o: ../src/Unity/test/tests/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -I/Users/scotty/eclipsewksp2/sclox/src/Unity/src -I"/Users/scotty/eclipsewksp2/classCreate" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


