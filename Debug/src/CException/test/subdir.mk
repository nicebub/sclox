################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CException/test/TestException.c 

OBJS += \
./src/CException/test/TestException.o 

C_DEPS += \
./src/CException/test/TestException.d 


# Each subdirectory must supply rules for building sources it contributes
src/CException/test/%.o: ../src/CException/test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/classCreate" -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


