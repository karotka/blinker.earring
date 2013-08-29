#!/bin/bash

avrdude -P usb -p t13 -c dragon_hvsp -U flash:w:flashdump.hex -U hfuse:w:0xfe:m
sleep 2
avrdude -P usb -p t13 -c dragon_hvsp -v
