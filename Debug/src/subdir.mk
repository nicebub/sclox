################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AstPrinter.c \
../src/CExceptionConfig.c \
../src/Environment.c \
../src/Expr.c \
../src/ExprArray.c \
../src/ExprIntHashMap.c \
../src/ExprIntegerHashMap.c \
../src/HashMap.c \
../src/HashMapStack.c \
../src/Interpreter.c \
../src/Lox.c \
../src/LoxCallable.c \
../src/LoxClass.c \
../src/LoxFunction.c \
../src/LoxInstance.c \
../src/Object.c \
../src/ObjectArray.c \
../src/Parser.c \
../src/Resolver.c \
../src/Scanner.c \
../src/Stack.c \
../src/Stmt.c \
../src/StmtArray.c \
../src/StrObjHashMap.c \
../src/StringBooleanHashMap.c \
../src/StringObjectHashMap.c \
../src/Token.c \
../src/TokenArray.c \
../src/additions.c \
../src/hash.c \
../src/memory.c \
../src/memoryArray.c \
../src/sclox.c \
../src/str.c 

OBJS += \
./src/AstPrinter.o \
./src/CExceptionConfig.o \
./src/Environment.o \
./src/Expr.o \
./src/ExprArray.o \
./src/ExprIntHashMap.o \
./src/ExprIntegerHashMap.o \
./src/HashMap.o \
./src/HashMapStack.o \
./src/Interpreter.o \
./src/Lox.o \
./src/LoxCallable.o \
./src/LoxClass.o \
./src/LoxFunction.o \
./src/LoxInstance.o \
./src/Object.o \
./src/ObjectArray.o \
./src/Parser.o \
./src/Resolver.o \
./src/Scanner.o \
./src/Stack.o \
./src/Stmt.o \
./src/StmtArray.o \
./src/StrObjHashMap.o \
./src/StringBooleanHashMap.o \
./src/StringObjectHashMap.o \
./src/Token.o \
./src/TokenArray.o \
./src/additions.o \
./src/hash.o \
./src/memory.o \
./src/memoryArray.o \
./src/sclox.o \
./src/str.o 

C_DEPS += \
./src/AstPrinter.d \
./src/CExceptionConfig.d \
./src/Environment.d \
./src/Expr.d \
./src/ExprArray.d \
./src/ExprIntHashMap.d \
./src/ExprIntegerHashMap.d \
./src/HashMap.d \
./src/HashMapStack.d \
./src/Interpreter.d \
./src/Lox.d \
./src/LoxCallable.d \
./src/LoxClass.d \
./src/LoxFunction.d \
./src/LoxInstance.d \
./src/Object.d \
./src/ObjectArray.d \
./src/Parser.d \
./src/Resolver.d \
./src/Scanner.d \
./src/Stack.d \
./src/Stmt.d \
./src/StmtArray.d \
./src/StrObjHashMap.d \
./src/StringBooleanHashMap.d \
./src/StringObjectHashMap.d \
./src/Token.d \
./src/TokenArray.d \
./src/additions.d \
./src/hash.d \
./src/memory.d \
./src/memoryArray.d \
./src/sclox.d \
./src/str.d 


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


