# Project Description

This IoT project is to measure power consumption using PZEM-004T v3 sensor and NodeMCU ESP32 

Before start to monitoring, the PZEM-004T sensor must be modified first because the voltage on the sensor is 5V while the ESP32 is 3.3V. Modifications are made to the sensor by adding a resistor with a value of 1k ohms. Data read by the sensor is voltage, current, power and energy data. Data on ESP32 is converted into JSON file format and then sent to the MQTT broker (in this project I use Eclipse Mosquitto) via WiFi.
