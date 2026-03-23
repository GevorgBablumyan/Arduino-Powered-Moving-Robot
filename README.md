# Arduino-Powered-Moving-Robot
# Arduino Powered Moving Robot 🤖

## Project Overview
This project demonstrates a simple Arduino-based autonomous robot capable of driving forward and making precise turns using motor encoders for feedback. The robot executes a sequence of predefined movement commands, such as driving a specific distance and turning a given angle.

---

## Features
- Drive a set distance (cm)
- Turn a specified angle (degrees)
- Motor control via PWM
- Encoder-based movement tracking for accuracy
- Serial Monitor output for debugging

---

## Hardware Requirements
- Arduino board (Uno, Nano, or compatible)
- 2 DC motors with encoders
- Motor driver (e.g., L298N)
- Wheels and robot chassis
- Power supply for motors

---

## Usage
1. Connect the motors, encoders, and motor driver to the Arduino according to the code.  
2. Upload the `robot.ino` sketch to the board via Arduino IDE.  
3. Open Serial Monitor (9600 baud) to view movement data.  
4. Modify the `actions[]` and `values[]` arrays to set new movement commands:

```cpp
char actions[] = {'d', 't','s'}; // drive, turn, stop
float values[] = {20, 90, 0};    // distance (cm), angle (degrees)
