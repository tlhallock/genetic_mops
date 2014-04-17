################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../backsolver.c \
../main.c \
../memory_manager.c \
../pareto_set.c 

OBJS += \
./backsolver.o \
./main.o \
./memory_manager.o \
./pareto_set.o 

C_DEPS += \
./backsolver.d \
./main.d \
./memory_manager.d \
./pareto_set.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


