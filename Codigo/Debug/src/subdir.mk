################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Bayes.cpp \
../src/Perceptron.cpp \
../src/SA.cpp 

OBJS += \
./src/Bayes.o \
./src/Perceptron.o \
./src/SA.o 

CPP_DEPS += \
./src/Bayes.d \
./src/Perceptron.d \
./src/SA.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


