## Introduction
This repository contains guides on modifying off-the-shelf NDIR CO2 sensors to report their data to home automation systems such as Home Assistant via the [ESPHome](https://esphome.io/) firmware flashed to a [WEMOS D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html) ESP8266 microcontroller. 

These inexpensive (around £4) tiny microcontroller boards have built-in WiFi, and can be powered by either 3.3V or 5V - perfect for sneaking inside an existing device to give it new capabilities.

The projects in this repository rely on "piggybacking" the existing CO2 sensor installed in devices by passively listening to the data sent over the UART TX line, allowing the host device to function as designed.

For sensors that use UART, this means identifying the TX pin, a source of power (usually the 5V and GND pins of the CO2 sensor can be used), and the UART communication protocol.

## Devices 
|Device|Sensors|
|-|-|
|[SA1200P](./sa1200p/README.md)|CO2, Temperature, Relative Humidity|