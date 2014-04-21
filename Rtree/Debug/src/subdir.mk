################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/Qtree.cc \
../src/QtreeBranch.cc \
../src/QtreeLeaf.cc \
../src/main.cc \
../src/qtree_it.cc \
../src/qtree_mop.cc \
../src/qtree_point.cc \
../src/qtree_test_utils.cc 

OBJS += \
./src/Qtree.o \
./src/QtreeBranch.o \
./src/QtreeLeaf.o \
./src/main.o \
./src/qtree_it.o \
./src/qtree_mop.o \
./src/qtree_point.o \
./src/qtree_test_utils.o 

CC_DEPS += \
./src/Qtree.d \
./src/QtreeBranch.d \
./src/QtreeLeaf.d \
./src/main.d \
./src/qtree_it.d \
./src/qtree_mop.d \
./src/qtree_point.d \
./src/qtree_test_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


