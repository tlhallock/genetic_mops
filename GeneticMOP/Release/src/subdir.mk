################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GeneticMOP.cpp \
../src/GeneticSolver.cpp \
../src/MOP.cpp \
../src/RandomSampleSolver.cpp \
../src/VectorImageSet.cpp 

OBJS += \
./src/GeneticMOP.o \
./src/GeneticSolver.o \
./src/MOP.o \
./src/RandomSampleSolver.o \
./src/VectorImageSet.o 

CPP_DEPS += \
./src/GeneticMOP.d \
./src/GeneticSolver.d \
./src/MOP.d \
./src/RandomSampleSolver.d \
./src/VectorImageSet.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


