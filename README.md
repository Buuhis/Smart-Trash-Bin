# Smart Trash Bin Project

## Overview
This project designs a **Smart Trash Bin** using an **STM32F411CEU6** microcontroller. The system integrates multiple sensors and actuators to automate trash bin operations, monitor fill levels, and provide manual control options. It also supports UART communication for status display on a laptop via MiniCom.

## Features
1. **Ultrasonic Sensor (SRF05)**:
   - Detects objects in front of the bin.
   - When an object is detected, the **SG92R servo** rotates to **135°** to open the lid.
   - After **5 seconds**, the servo returns to **0°**, closing the lid.
   
2. **Laser Distance Sensor (GY-530 VL53L0X)**:
   - Mounted on the bin lid to measure the fill level.
   - Fill level is indicated using three LEDs:
     - **Green LED**: 0% to ~40% full.
     - **Yellow LED**: ~41% to ~85% full.
     - **Red LED**: ~86% to ~100% full.
   - The fill status is displayed on an **SH1106 OLED**:
     - Green/Yellow LED: "Please give me trash :))".
     - Red LED: "I'm full :((".

3. **Manual Control**:
   - A **push button** (pull-up) allows manual opening and closing of the lid, bypassing the SRF05 sensor.

4. **UART Communication (via CP2102 module)**:
   - Connects the MCU to a laptop for status monitoring via MiniCom.
   - Commands:
     - Press **'s'**: Display trash bin status.
     - Press **'o'**: Open the lid.
     - Press **'c'**: Close the lid.
     - Press **'t'**: Show option.

## Hardware Connections
The system uses the **STM32F411CEU6** microcontroller with the following pin assignments:

| Component            | Pin Connection                     |
|----------------------|------------------------------------|
| **SRF05**            | Trig → PA11, Echo → PA12          |
| **GY-530 VL53L0X**   | SCL → PB6, SDA → PB7              |
| **SG92R Servo**      | PWM → PA8                         |
| **SH1106 OLED**      | SCL → PB10, SDA → PB3             |
| **Push Button**      | PB13 (pull-up)                    |
| **LEDs**             | Green → PB1, Yellow → PB0, Red → PA7 |

## System Workflow
1. **Object Detection**:
   - SRF05 detects an object, triggering the servo to open the lid (135°).
   - After 5 seconds, the lid closes automatically (0°).
2. **Fill Level Monitoring**:
   - VL53L0X measures the distance to the trash surface.
   - LEDs indicate the fill level, and the SH1106 OLED displays the corresponding message.
3. **Manual Operation**:
   - Pressing the button toggles the lid between open and closed states.
4. **UART Interface**:
   - Status and control commands are sent/received via UART to/from a laptop using MiniCom.

## Future Enhancements
- Add a buzzer to alert when the bin is full.
- Implement wireless communication (e.g., Wi-Fi/Bluetooth) for remote monitoring.
- Optimize power consumption for battery-operated use.