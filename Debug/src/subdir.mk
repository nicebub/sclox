################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AstPrinter.c \
../src/Environment.c \
../src/Expr.c \
../src/ExprArray.c \
../src/HashMap.c \
../src/Interpreter.c \
../src/Lox.c \
../src/Object.c \
../src/ObjectArray.c \
../src/Parser.c \
../src/Scanner.c \
../src/Stmt.c \
../src/StmtArray.c \
../src/Token.c \
../src/TokenArray.c \
../src/additions.c \
../src/hash.c \
../src/sclox.c 

OBJS += \
./src/AstPrinter.o \
./src/Environment.o \
./src/Expr.o \
./src/ExprArray.o \
./src/HashMap.o \
./src/Interpreter.o \
./src/Lox.o \
./src/Object.o \
./src/ObjectArray.o \
./src/Parser.o \
./src/Scanner.o \
./src/Stmt.o \
./src/StmtArray.o \
./src/Token.o \
./src/TokenArray.o \
./src/additions.o \
./src/hash.o \
./src/sclox.o 

C_DEPS += \
./src/AstPrinter.d \
./src/Environment.d \
./src/Expr.d \
./src/ExprArray.d \
./src/HashMap.d \
./src/Interpreter.d \
./src/Lox.d \
./src/Object.d \
./src/ObjectArray.d \
./src/Parser.d \
./src/Scanner.d \
./src/Stmt.d \
./src/StmtArray.d \
./src/Token.d \
./src/TokenArray.d \
./src/additions.d \
./src/hash.d \
./src/sclox.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -I/Users/scotty/eclipsewksp2/sclox/src/Unity/src -I"/Users/scotty/eclipsewksp2/classCreate" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Expr.o: /Users/scotty/eclipsewksp2/sclox/src/Expr.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -I/Users/scotty/eclipsewksp2/sclox/src/Unity/src -I"/Users/scotty/eclipsewksp2/classCreate" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Stmt.o: /Users/scotty/eclipsewksp2/sclox/src/Stmt.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/scotty/eclipsewksp2/sclox/src" -I"/Users/scotty/eclipsewksp2/sclox/src/CException/lib" -I/Users/scotty/eclipsewksp2/sclox/src/Unity/src -I"/Users/scotty/eclipsewksp2/classCreate" -O0 -g3 -pedantic -pedantic-errors -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


