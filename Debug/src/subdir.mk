################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AstPrinter.c \
../src/Expr.c \
../src/Interpreter.c \
../src/Lox.c \
../src/Object.c \
../src/Parser.c \
../src/Scanner.c \
../src/Token.c \
../src/sclox.c 

OBJS += \
./src/AstPrinter.o \
./src/Expr.o \
./src/Interpreter.o \
./src/Lox.o \
./src/Object.o \
./src/Parser.o \
./src/Scanner.o \
./src/Token.o \
./src/sclox.o 

C_DEPS += \
./src/AstPrinter.d \
./src/Expr.d \
./src/Interpreter.d \
./src/Lox.d \
./src/Object.d \
./src/Parser.d \
./src/Scanner.d \
./src/Token.d \
./src/sclox.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/classCreate" -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Expr.o: /Users/scotty/eclipsewksp2/sclox/src/Expr.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/classCreate" -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


