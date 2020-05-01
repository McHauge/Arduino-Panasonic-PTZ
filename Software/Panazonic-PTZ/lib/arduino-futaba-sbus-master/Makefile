CLOCK_FREQUENCY=16000000UL
PROCESSOR=atmega328p
INCLUDES=-I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/mega -I/usr/share/arduino/libraries/SoftwareSerial
CFLAGS=-Os -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -DARDUINO=10802 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR 
OBJECTS=src/FutabaSBUS.o

all: compile

compile: $(OBJECTS)

.cpp.o:
	/usr/share/arduino/hardware/tools/avr/bin/avr-g++ $(CFLAGS) $(INCLUDES) -Isrc -DF_CPU=$(CLOCK_FREQUENCY) -mmcu=$(PROCESSOR) -c  $< -o $@

clean:
	rm -rf *.o
	rm -rf *.elf
	rm -rf *.hex

