# Arduino Dual-Mode Robot Controller



Combination Bluetooth-controlled and autonomous line-following robot using Arduino. Supports real-time mode switching and track recovery systems.

## 📋 Table of Contents

- [Features](#-features)
- [Hardware Requirements](#-hardware-requirements)
- [Pin Configuration](#-pin-configuration)
- [Installation](#-installation)
- [Usage](#-usage)
- [Motor Control](#-motor-control-functions)
- [Troubleshooting](#-troubleshooting)
- [License](#-license)

## 🚀 Features

- **Dual Operation Modes**
  - Bluetooth remote control (Manual mode)
  - Autonomous line following (Auto mode)
- Three-point IR sensor array
- Adaptive track recovery system
- Serial command interface
- PWM-based motor control

## 🔧 Hardware Requirements

| Component              | Quantity |
| ---------------------- | -------- |
| Arduino Uno/Nano       | 1        |
| L298N Motor Driver     | 1        |
| IR Line Sensors        | 3        |
| DC Gear Motors         | 2        |
| HC-05 Bluetooth Module | 1        |
| 7.4V LiPo Battery      | 1        |
| Robot Chassis          | 1        |

## 📍 Pin Configuration

| Arduino Pin | Connected Component |
| ----------- | ------------------- |
| 2           | Motor A IN2         |
| 3           | Motor B ENB         |
| 4           | Motor A IN1         |
| 5           | Motor B IN4         |
| 6           | Motor B IN3         |
| 9           | Motor A ENA         |
| 10          | Right Sensor        |
| 11          | Center Sensor       |
| 13          | Left Sensor         |

## 📥 Installation

1. Connect components according to the pin configuration table.
2. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/arduino-robot-controller.git
   ```
3. Open `robot_controller.ino` in Arduino IDE.
4. Upload the code to the Arduino board.
5. Power up the robot and open Serial Monitor (9600 baud).

## 🕹️ Usage

### 🔄 Mode Switching

Send via Serial Monitor or Bluetooth:

```
LINE_FOLLOWER    # Activate autonomous mode
BLUETOOTH_MODE   # Return to manual control
```

### 📱 Bluetooth Commands

```
up       - Move forward
down     - Move backward
left     - Sharp left turn
right    - Sharp right turn
STOP     - Emergency stop
```

### 🚦 Line Following Logic

| Sensor State | Robot Action             |
| ------------ | ------------------------ |
| Center HIGH  | Move straight            |
| Left HIGH    | Turn left                |
| Right HIGH   | Turn right               |
| All LOW      | Reverse + last direction |
| All HIGH     | Prioritize left turn     |

## 🎮 Motor Control Functions

| Command        | ENA | ENB | IN1  | IN2  | IN3  | IN4  |
| -------------- | --- | --- | ---- | ---- | ---- | ---- |
| moveForward()  | 60  | 60  | LOW  | HIGH | HIGH | LOW  |
| moveBackward() | 60  | 60  | HIGH | LOW  | LOW  | HIGH |
| turnLeft()     | 60  | 30  | LOW  | HIGH | LOW  | HIGH |
| turnRight()    | 30  | 60  | HIGH | LOW  | HIGH | LOW  |
| stopMotors()   | 0   | 0   | LOW  | LOW  | LOW  | LOW  |

## 🚨 Troubleshooting

### Motors Not Responding:

- Check motor driver power connections.
- Verify enable pin (ENA/ENB) voltages.
- Confirm Bluetooth module pairing.

### Erratic Line Following:

- Clean sensor lenses.
- Adjust sensor height (5-10mm from surface).
- Calibrate on actual track surface.

### Serial Communication Issues:

```bash
# Check available ports
ls /dev/tty.*  # Mac/Linux
# Or in Device Manager (Windows)
```

## 📜 License

MIT License - See `LICENSE` for full text.



