teensyReadRC_Receiver
=====================

Test code for reading rc receivers using a Teensy 3. Two variants are 
provided to read two types of receivers:
 * standard receivers where each pin contains a separate channel
 * PPM receivers where all channel signals are combined into a single 
stream

Note: this code uses interrupts, so it will not disturb the conventional
flow of a conventional Arduino program.

Note: Teensy 3 supports 5V-tolerant inputs, so the receiver's channels may 
be directly plugged into the Teensy.

Note: This project may be compiled with the [Arduino-Makefile](https://github.com/sudar/Arduino-Makefile) project or conventionally within the Arduino
IDE.

This code has been successfully tested with the following receivers:
 * Lemon-RX DSMX receiver (PPM)
 * Spektrum AR610 DSMX receiver (standard)

