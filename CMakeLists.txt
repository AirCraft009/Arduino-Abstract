cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_OBJCOPY avr-objcopy)
set(CMAKE_SIZE avr-size)

project(Arduino-Abstract C)

set(MCU atmega328p)
set(F_CPU 16000000UL)

add_executable(Arduino-Abstract.elf
        src/External-Sensors/HC-SR04/HCSR04.c
        src/GPIO/Pins/Pin.c
        src/GPIO/Serial/Serial.c
        src/GPIO/Serial/Serial.h
        src/Timers/timer.h
        src/main.c

)

target_compile_options(Arduino-Abstract.elf PRIVATE
        -mmcu=${MCU}
        -DF_CPU=${F_CPU}
        -Os
        -Wall
        -Wextra
        -std=c11
)

target_link_options(Arduino-Abstract.elf PRIVATE
        -mmcu=${MCU}
)

add_custom_command(
        TARGET Arduino-Abstract.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom
        Arduino-Abstract.elf Arduino-Abstract.hex
        COMMAND ${CMAKE_SIZE} Arduino-Abstract.elf
)

add_custom_target(flash
        COMMAND avrdude -c arduino -p m328p -P COM7 -b 115200
        -U flash:w:Arduino-Abstract.hex
        DEPENDS Arduino-Abstract.elf
)