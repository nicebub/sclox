################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Unity/test/expectdata/testsample_cmd.c \
../src/Unity/test/expectdata/testsample_def.c \
../src/Unity/test/expectdata/testsample_head1.c \
../src/Unity/test/expectdata/testsample_mock_cmd.c \
../src/Unity/test/expectdata/testsample_mock_def.c \
../src/Unity/test/expectdata/testsample_mock_head1.c \
../src/Unity/test/expectdata/testsample_mock_new1.c \
../src/Unity/test/expectdata/testsample_mock_new2.c \
../src/Unity/test/expectdata/testsample_mock_param.c \
../src/Unity/test/expectdata/testsample_mock_run1.c \
../src/Unity/test/expectdata/testsample_mock_run2.c \
../src/Unity/test/expectdata/testsample_mock_yaml.c \
../src/Unity/test/expectdata/testsample_new1.c \
../src/Unity/test/expectdata/testsample_new2.c \
../src/Unity/test/expectdata/testsample_param.c \
../src/Unity/test/expectdata/testsample_run1.c \
../src/Unity/test/expectdata/testsample_run2.c \
../src/Unity/test/expectdata/testsample_yaml.c 

OBJS += \
./src/Unity/test/expectdata/testsample_cmd.o \
./src/Unity/test/expectdata/testsample_def.o \
./src/Unity/test/expectdata/testsample_head1.o \
./src/Unity/test/expectdata/testsample_mock_cmd.o \
./src/Unity/test/expectdata/testsample_mock_def.o \
./src/Unity/test/expectdata/testsample_mock_head1.o \
./src/Unity/test/expectdata/testsample_mock_new1.o \
./src/Unity/test/expectdata/testsample_mock_new2.o \
./src/Unity/test/expectdata/testsample_mock_param.o \
./src/Unity/test/expectdata/testsample_mock_run1.o \
./src/Unity/test/expectdata/testsample_mock_run2.o \
./src/Unity/test/expectdata/testsample_mock_yaml.o \
./src/Unity/test/expectdata/testsample_new1.o \
./src/Unity/test/expectdata/testsample_new2.o \
./src/Unity/test/expectdata/testsample_param.o \
./src/Unity/test/expectdata/testsample_run1.o \
./src/Unity/test/expectdata/testsample_run2.o \
./src/Unity/test/expectdata/testsample_yaml.o 

C_DEPS += \
./src/Unity/test/expectdata/testsample_cmd.d \
./src/Unity/test/expectdata/testsample_def.d \
./src/Unity/test/expectdata/testsample_head1.d \
./src/Unity/test/expectdata/testsample_mock_cmd.d \
./src/Unity/test/expectdata/testsample_mock_def.d \
./src/Unity/test/expectdata/testsample_mock_head1.d \
./src/Unity/test/expectdata/testsample_mock_new1.d \
./src/Unity/test/expectdata/testsample_mock_new2.d \
./src/Unity/test/expectdata/testsample_mock_param.d \
./src/Unity/test/expectdata/testsample_mock_run1.d \
./src/Unity/test/expectdata/testsample_mock_run2.d \
./src/Unity/test/expectdata/testsample_mock_yaml.d \
./src/Unity/test/expectdata/testsample_new1.d \
./src/Unity/test/expectdata/testsample_new2.d \
./src/Unity/test/expectdata/testsample_param.d \
./src/Unity/test/expectdata/testsample_run1.d \
./src/Unity/test/expectdata/testsample_run2.d \
./src/Unity/test/expectdata/testsample_yaml.d 


# Each subdirectory must supply rules for building sources it contributes
src/Unity/test/expectdata/%.o: ../src/Unity/test/expectdata/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -I/Users/scotty/eclipsewksp2/sclox/src/Unity/src -I"/Users/scotty/eclipsewksp2/classCreate" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


