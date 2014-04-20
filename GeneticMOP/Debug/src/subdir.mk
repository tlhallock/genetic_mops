################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DepthFirstRecursiveStencil.cpp \
../src/GeneticMOP.cpp \
../src/GeneticSolver.cpp \
../src/IncrementalSampleSolver.cpp \
../src/MOP.cpp \
../src/RandomSampleSolver.cpp \
../src/RtreeImageSet.cpp \
../src/TestImageSet.cpp \
../src/VectorImageSet.cpp \
../src/math_functions.cpp 

CC_SRCS += \
../src/utils.cc 

OBJS += \
./src/DepthFirstRecursiveStencil.o \
./src/GeneticMOP.o \
./src/GeneticSolver.o \
./src/IncrementalSampleSolver.o \
./src/MOP.o \
./src/RandomSampleSolver.o \
./src/RtreeImageSet.o \
./src/TestImageSet.o \
./src/VectorImageSet.o \
./src/math_functions.o \
./src/utils.o 

CC_DEPS += \
./src/utils.d 

CPP_DEPS += \
./src/DepthFirstRecursiveStencil.d \
./src/GeneticMOP.d \
./src/GeneticSolver.d \
./src/IncrementalSampleSolver.d \
./src/MOP.d \
./src/RandomSampleSolver.d \
./src/RtreeImageSet.d \
./src/TestImageSet.d \
./src/VectorImageSet.d \
./src/math_functions.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/school/genetic_mops/Rtree/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/school/genetic_mops/Rtree/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


