################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Unity/extras/fixture/test/template_fixture_tests.c \
../src/Unity/extras/fixture/test/unity_fixture_Test.c \
../src/Unity/extras/fixture/test/unity_fixture_TestRunner.c 

OBJS += \
./src/Unity/extras/fixture/test/template_fixture_tests.o \
./src/Unity/extras/fixture/test/unity_fixture_Test.o \
./src/Unity/extras/fixture/test/unity_fixture_TestRunner.o 

C_DEPS += \
./src/Unity/extras/fixture/test/template_fixture_tests.d \
./src/Unity/extras/fixture/test/unity_fixture_Test.d \
./src/Unity/extras/fixture/test/unity_fixture_TestRunner.d 


# Each subdirectory must supply rules for building sources it contributes
src/Unity/extras/fixture/test/%.o: ../src/Unity/extras/fixture/test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -I/Users/scotty/eclipsewksp2/sclox/src/Unity/src -I"/Users/scotty/eclipsewksp2/classCreate" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


