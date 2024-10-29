# Task Management with FreeRTOS
This project demonstrates a C program designed to manage task execution on a MCU(Arduino uno, ESP32...) for efficient sensor data reading, processing, and LED notification. The primary goal of this code is to illustrate how FreeRTOS is used to manage concurrent tasks while optimizing energy consumption by utilizing the MCU’s sleep mode. This functionality is achieved through an interrupt-driven approach where the MCU stays in a low-power state until it receives a sensor data request from the I2C master.

## Project Overview
In this project:
- MCU handles sensor data acquisition, processing, and notification tasks.
- FreeRTOS is utilized to manage task scheduling and prioritize tasks based on their urgency and functionality.
- ISR (Interrupt Service Routine) wakes up the MCU from sleep mode upon receiving a request from the I2C master.
- Energy efficiency is optimized by keeping the MCU in sleep mode when tasks are not in operation.

## Features
- Task Management with FreeRTOS: Handles multiple tasks such as reading sensor data, processing it, and controlling LED indicators.
- Power Management: Minimizes power consumption by keeping MCU in deep sleep mode until interrupted by I2C requests.
- LED Notification: Uses LEDs to notify the user of the sensor data status and task completion.

## Prerequisites
- Hardware: MCU (I2C Slave), MCU (I2C Master), Sensor, and LEDs for notifications.
- Software: FreeRTOS libraries, IDE-env.
  
## Getting Started
1. Installation
Clone this repository to your local machine:
```bash
git clone https://github.com/Khalid-Bo/FreeRTOS_Slave_node.git
```
2. Compile and Flash the Code to the MCU using appropiate IDE-env

## Usage
1. Connect your Sensor and LEDs to the MCU as described in the HW setup.
2. Once flashed, the MCU will remain in sleep mode by default.
3. The I2C master can trigger the MCU to wake up, at which point it will:
   * Read sensor data.
   * Process the data and execute necessary tasks.
   * Indicate task completion using LEDs.

## Code Structure
1. FreeRTOS_ISR_Slave_node.c: Contains the main program logic with FreeRTOS task definitions, ISR handling, and task scheduling.
2. Task Functions :
    * Read_Sensor_Data : Defines sensor reading task.
    * Process_Sensor_Data : Defins the procees task on the sensor data.
    * Notify_Output: Manages LED notifications based on processed sensor data.

## Contributing
- Feel free to fork this repository and submit pull requests with improvements or additional features.