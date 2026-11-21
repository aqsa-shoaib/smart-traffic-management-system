# Smart Traffic Management System

An Arduino-based traffic signal controller for a four-way intersection, developed as a classroom engineering project. The system coordinates two sets of traffic lights, accepts pedestrian button requests, estimates simulated congestion using light sensors, and uses servo motors to represent opening an additional lane. A 16×2 LCD shows the current traffic condition.

## Circuit

![Arduino traffic controller circuit](images/circuit-diagram.png)

## Features

- Alternating red, yellow, and green signals for two traffic directions
- Two pedestrian buttons that request a longer green-light interval
- Two photoresistors for simulated traffic-level detection
- Two servo motors representing an additional lane opening or closing
- LCD display showing moderate or heavy traffic
- Repeated button polling during signal timing to capture brief presses

## Hardware and software

- Arduino Uno
- Breadboard, LEDs, and current-limiting resistors
- Two pushbuttons with appropriate pull-down resistors
- Two photoresistors and voltage-divider resistors
- Two servo motors
- 16×2 character LCD and contrast potentiometer
- Arduino C++ with built-in `Servo` and `LiquidCrystal` libraries

## Pin connections

| Component | Arduino pin(s) |
| --- | --- |
| Direction 1: red, yellow, green | D13, D12, D11 |
| Direction 2: red, yellow, green | D10, D9, D8 |
| LCD: RS, Enable, D4–D7 | D7, D6, D2–D5 |
| Pedestrian buttons | A2, A3 |
| Photoresistors | A0, A1 |
| Servo motors | A4, A5 |

## Running the project

1. Open `SmartTrafficSystem.ino` with the Arduino IDE.
2. Connect the components using the provided circuit screenshot and pin table.
3. Select **Arduino Uno** as the board and upload the sketch, or use the sketch in a compatible circuit simulator.
4. Observe the light sequence. Press a pedestrian button to request a longer green phase for the corresponding direction.
5. Adjust the photoresistor inputs to simulate a change in traffic conditions.

The LCD displays `Heavy Traffic` and `Extra Lane Open` when both sensor readings fall below the threshold of `20`. Otherwise, it displays moderate traffic and normal lane operation. The sensor threshold and servo angles may need adjustment for the physical wiring or simulator.

## Program structure

`readButtons()` records button presses. `updateTrafficLevel()` reads the photoresistors, updates the LCD, and moves the servos when the traffic state changes. `showSignals()` controls the six signal LEDs. `holdPhase()` checks the inputs while each timed signal state is active.

## Design notes

Photoresistors measure light, **not actual vehicle counts**. In this model, the light readings represent simulated traffic conditions. The servos demonstrate a lane-opening concept, not a road-safe physical barrier. This educational prototype is not suitable for controlling real road traffic.

The included `.brd` file is an **Autodesk EAGLE board-design file**, not a Tinkercad simulation file. The screenshot documents the original breadboard-style circuit. The cleaned-up sketch preserves the original pin mapping but contains code refinements; the updated sketch has not been verified against the original simulator wiring.

## Credits

**Aqsa Shoaib** — original classroom project, January 5, 2025. This repository includes a reorganized version of the original sketch.
