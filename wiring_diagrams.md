# ESP32 Home Automation - Wiring Diagrams

## 🔌 Complete Wiring Guide

### 📋 Component Overview

| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP32 Dev Board | 1 | Main microcontroller |
| 4-Channel Relay Module | 1 | Device switching |
| Push Buttons | 4 | Manual control |
| Jumper Wires | 20+ | Connections |
| Breadboard | 1 | Prototyping |
| Power Supply | 1 | 5V, 2A minimum |

---

## 🎯 Main Wiring Diagram

```
                    ESP32 HOME AUTOMATION SYSTEM
                    =============================

    ┌─────────────────────────────────────────────────────────────┐
    │                        ESP32 BOARD                         │
    │                                                             │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │ GPIO│  │ GPIO│  │ GPIO│  │ GPIO│  │ GPIO│  │ GPIO│      │
    │  │  2  │  │  4  │  │  5  │  │ 18  │  │ 19  │  │ 21  │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    │     │        │        │        │        │        │         │
    │     │        │        │        │        │        │         │
    │     ▼        ▼        ▼        ▼        ▼        ▼         │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │RELAY│  │RELAY│  │RELAY│  │RELAY│  │ BTN │  │ BTN │      │
    │  │  1  │  │  2  │  │  3  │  │  4  │  │  1  │  │  2  │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    │                                                             │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │ GPIO│  │ GPIO│  │ 3.3V│  │ 5V  │  │ GND │  │ GND │      │
    │  │ 22  │  │ 23  │  │     │  │     │  │     │  │     │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    │     │        │        │        │        │        │         │
    │     ▼        ▼        ▼        ▼        ▼        ▼         │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │ BTN │  │ BTN │  │POWER│  │POWER│  │GND  │  │GND  │      │
    │  │  3  │  │  4  │  │ 3.3V│  │ 5V  │  │     │  │     │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    └─────────────────────────────────────────────────────────────┘
                                │
                                ▼
    ┌─────────────────────────────────────────────────────────────┐
    │                    4-CHANNEL RELAY MODULE                   │
    │                                                             │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │ IN1 │  │ IN2 │  │ IN3 │  │ IN4 │  │ VCC │  │ GND │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    │     │        │        │        │        │        │         │
    │     │        │        │        │        │        │         │
    │     ▼        ▼        ▼        ▼        ▼        ▼         │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │RELAY│  │RELAY│  │RELAY│  │RELAY│  │ 5V  │  │GND  │      │
    │  │  1  │  │  2  │  │  3  │  │  4  │  │POWER│  │     │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    │                                                             │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │ NO1 │  │ COM1│  │ NO2 │  │ COM2│  │ NO3 │  │ COM3│      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    │     │        │        │        │        │        │         │
    │     │        │        │        │        │        │         │
    │     ▼        ▼        ▼        ▼        ▼        ▼         │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │DEVICE│  │DEVICE│  │DEVICE│  │DEVICE│  │DEVICE│  │DEVICE│  │
    │  │  1  │  │  1  │  │  2  │  │  2  │  │  3  │  │  3  │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    └─────────────────────────────────────────────────────────────┘
```

---

## 🔌 Detailed Connection Table

### ESP32 to Relay Module Connections

| ESP32 Pin | Relay Module | Wire Color | Purpose |
|-----------|---------------|------------|---------|
| GPIO 2    | IN1          | Red        | Relay 1 Control |
| GPIO 4    | IN2          | Orange     | Relay 2 Control |
| GPIO 5    | IN3          | Yellow     | Relay 3 Control |
| GPIO 18   | IN4          | Green      | Relay 4 Control |
| 5V        | VCC          | Red        | Power Supply |
| GND       | GND          | Black      | Ground |

### ESP32 to Push Button Connections

| ESP32 Pin | Button | Wire Color | Purpose |
|-----------|--------|------------|---------|
| GPIO 19   | Button 1 | Blue       | Manual Control 1 |
| GPIO 21   | Button 2 | Purple     | Manual Control 2 |
| GPIO 22   | Button 3 | Brown      | Manual Control 3 |
| GPIO 23   | Button 4 | Gray       | Manual Control 4 |
| GND       | All Buttons | Black | Common Ground |

---

## ⚡ Power Distribution Diagram

```
                    POWER SUPPLY (5V, 2A)
                    =====================
                              │
                              ▼
    ┌─────────────────────────────────────────────────────────────┐
    │                    POWER DISTRIBUTION                      │
    │                                                             │
    │  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐         │
    │  │ ESP32   │  │ RELAY   │  │ SENSORS │  │ STATUS  │         │
    │  │  5V     │  │ MODULE  │  │  3.3V   │  │  LEDS   │         │
    │  │  VIN    │  │   VCC   │  │         │  │         │         │
    │  └─────────┘  └─────────┘  └─────────┘  └─────────┘         │
    │       │            │            │            │             │
    │       │            │            │            │             │
    │       ▼            ▼            ▼            ▼             │
    │  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐         │
    │  │  5V     │  │  5V     │  │  3.3V   │  │  3.3V   │         │
    │  │ POWER   │  │ POWER   │  │ POWER   │  │ POWER   │         │
    │  └─────────┘  └─────────┘  └─────────┘  └─────────┘         │
    │                                                             │
    │  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐         │
    │  │ GND     │  │ GND     │  │ GND     │  │ GND     │         │
    │  │ COMMON  │  │ COMMON  │  │ COMMON  │  │ COMMON  │         │
    │  └─────────┘  └─────────┘  └─────────┘  └─────────┘         │
    └─────────────────────────────────────────────────────────────┘
```

---

## 🏠 Device Connection Examples

### Light Bulb Connection
```
                    LIGHT BULB CONNECTION
                    =====================

    ┌─────────┐    ┌─────────┐    ┌─────────┐
    │   AC    │    │ RELAY   │    │ LIGHT   │
    │ POWER   │────│ MODULE  │────│  BULB   │
    │ (120V)  │    │ CHANNEL │    │         │
    └─────────┘    └─────────┘    └─────────┘
         │              │              │
         │              │              │
         ▼              ▼              ▼
    ┌─────────┐    ┌─────────┐    ┌─────────┐
    │  HOT    │    │   NO   │    │  HOT    │
    │  WIRE   │    │ (Normally│    │  WIRE   │
    │         │    │  Open)  │    │         │
    └─────────┘    └─────────┘    └─────────┘
```

### Fan Connection
```
                    FAN CONNECTION
                    ===============

    ┌─────────┐    ┌─────────┐    ┌─────────┐
    │   AC    │    │ RELAY   │    │   FAN   │
    │ POWER   │────│ MODULE  │────│ MOTOR   │
    │ (120V)  │    │ CHANNEL │    │         │
    └─────────┘    └─────────┘    └─────────┘
         │              │              │
         │              │              │
         ▼              ▼              ▼
    ┌─────────┐    ┌─────────┐    ┌─────────┐
    │  HOT    │    │   NO   │    │  HOT    │
    │  WIRE   │    │ (Normally│    │  WIRE   │
    │         │    │  Open)  │    │         │
    └─────────┘    └─────────┘    └─────────┘
```

---

## 🔧 Breadboard Layout

```
                    BREADBOARD LAYOUT
                    ==================

    ┌─────────────────────────────────────────────────────────────┐
    │                    BREADBOARD (830 TIE)                    │
    │                                                             │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │ ESP32│  │     │  │     │  │     │  │     │  │     │      │
    │  │     │  │     │  │     │  │     │  │     │  │     │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    │     │        │        │        │        │        │         │
    │     │        │        │        │        │        │         │
    │     ▼        ▼        ▼        ▼        ▼        ▼         │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │     │  │     │  │     │  │     │  │     │  │     │      │
    │  │     │  │     │  │     │  │     │  │     │  │     │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    │                                                             │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │     │  │     │  │     │  │     │  │     │  │     │      │
    │  │     │  │     │  │     │  │     │  │     │  │     │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    │     │        │        │        │        │        │         │
    │     │        │        │        │        │        │         │
    │     ▼        ▼        ▼        ▼        ▼        ▼         │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │ BTN │  │ BTN │  │ BTN │  │ BTN │  │     │  │     │      │
    │  │  1  │  │  2  │  │  3  │  │  4  │  │     │  │     │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    │                                                             │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │     │  │     │  │     │  │     │  │     │  │     │      │
    │  │     │  │     │  │     │  │     │  │     │  │     │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    │     │        │        │        │        │        │         │
    │     │        │        │        │        │        │         │
    │     ▼        ▼        ▼        ▼        ▼        ▼         │
    │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐      │
    │  │     │  │     │  │     │  │     │  │     │  │     │      │
    │  │     │  │     │  │     │  │     │  │     │  │     │      │
    │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘      │
    └─────────────────────────────────────────────────────────────┘
```

---

## ⚠️ Safety Guidelines

### Electrical Safety
- **Always turn off power** before making connections
- **Use proper wire gauges** for current requirements
- **Double-check connections** before powering on
- **Use insulated tools** when working with live circuits
- **Follow local electrical codes** and regulations

### Component Safety
- **Check voltage ratings** before connecting devices
- **Use appropriate fuses** for protection
- **Ensure proper grounding** for safety
- **Test with multimeter** before final connections

### Installation Tips
- **Label all wires** for easy identification
- **Use wire nuts** for secure connections
- **Keep wires organized** and away from heat sources
- **Install in appropriate enclosures** for protection

---

## 🔍 Testing Procedures

### 1. Continuity Testing
```
    ┌─────────┐    ┌─────────┐    ┌─────────┐
    │ MULTI-  │    │   WIRE  │    │ COMPONENT│
    │ METER   │────│   UNDER │────│   UNDER │
    │ (OHMS)  │    │  TEST   │    │  TEST   │
    └─────────┘    └─────────┘    └─────────┘
```

### 2. Voltage Testing
```
    ┌─────────┐    ┌─────────┐    ┌─────────┐
    │ MULTI-  │    │   WIRE  │    │ POWER   │
    │ METER   │────│   UNDER │────│ SOURCE  │
    │ (VOLTS) │    │  TEST   │    │         │
    └─────────┘    └─────────┘    └─────────┘
```

### 3. Function Testing
- Test each relay individually
- Verify button functionality
- Check WiFi connectivity
- Test web dashboard access
- Validate Firebase connection

---

## 📋 Wiring Checklist

### Pre-Installation
- [ ] All components present and verified
- [ ] Tools and materials ready
- [ ] Safety equipment available
- [ ] Power turned off at breaker

### During Installation
- [ ] ESP32 connections made
- [ ] Relay module connected
- [ ] Push buttons wired
- [ ] Power connections secure
- [ ] Ground connections verified

### Post-Installation
- [ ] All connections double-checked
- [ ] Power applied and tested
- [ ] Each relay tested individually
- [ ] Web dashboard accessible
- [ ] Firebase synchronization working

---

## 🛠️ Troubleshooting Guide

### Common Issues

| Problem | Cause | Solution |
|---------|-------|----------|
| ESP32 won't start | Power issue | Check 5V supply and connections |
| Relays not working | Wrong GPIO pins | Verify pin assignments in code |
| Buttons not responding | Pull-up resistors | Check button wiring and code |
| WiFi not connecting | Wrong credentials | Verify SSID and password |
| Web dashboard not loading | Network issue | Check IP address and connectivity |

### Debug Steps
1. **Check power supply** - Ensure 5V, 2A minimum
2. **Verify connections** - Use multimeter to test continuity
3. **Check code** - Ensure correct pin assignments
4. **Test individually** - Test each component separately
5. **Check network** - Verify WiFi credentials and signal strength

---

## 📞 Support

For additional help with wiring and connections:
- Check the main README.md for detailed setup instructions
- Review the Arduino code comments for pin assignments
- Test each connection with a multimeter
- Start with simple connections and build complexity gradually

**Remember: Safety first! Always turn off power before making connections.**
