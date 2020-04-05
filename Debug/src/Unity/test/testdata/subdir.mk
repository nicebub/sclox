################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Unity/test/testdata/testRunnerGenerator.c \
../src/Unity/test/testdata/testRunnerGeneratorSmall.c \
../src/Unity/test/testdata/testRunnerGeneratorWithMocks.c 

OBJS += \
./src/Unity/test/testdata/testRunnerGenerator.o \
./src/Unity/test/testdata/testRunnerGeneratorSmall.o \
./src/Unity/test/testdata/testRunnerGeneratorWithMocks.o 

C_DEPS += \
./src/Unity/test/testdata/testRunnerGenerator.d \
./src/Unity/test/testdata/testRunnerGeneratorSmall.d \
./src/Unity/test/testdata/testRunnerGeneratorWithMocks.d 


# Each subdirectory must supply rules for building sources it contributes
src/Unity/test/testdata/%.o: ../src/Unity/test/testdata/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -I/Users/scotty/eclipsewksp2/sclox/src/Unity/src -I"/Users/scotty/eclipsewksp2/classCreate" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


