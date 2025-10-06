# ESP32 Home Automation - Complete Hardware Setup Guide

## 🎯 **Essential Components List**

### **Core Components (From Transcripts)**
```
ESP32 DEV KIT V1 - $15
├── GPIO pins: 16, 17, 26, 27 (for LEDs/relays)
├── Built-in WiFi (Access Point + Station modes)
└── USB programming capability

4-Channel 5V Relay Module (Active LOW) - $8
├── Control pins: GPIO 2, 4, 5, 18
├── Power: 5V supply
└── Relay type: SPDT (Single Pole Double Throw)

LEDs and Resistors - $3
├── Red LED (GPIO 16) + 220Ω resistor
├── Green LED (GPIO 17) + 220Ω resistor
└── Status indicators

Push Buttons (x4) - $5
├── Manual control for each relay
├── Connected to GPIO pins + GND
└── Pull-up resistors (10kΩ internal)

Breadboard and Wires - $18
├── Breadboard 830 points - $8
├── Jumper wires (Male-Female) - $5
├── Jumper wires (Male-Male) - $5
└── USB cable - $3
```

### **Tools Required**
```
Multimeter - $25
Wire strippers - $15
Safety glasses - $10
Insulated screwdrivers - $20
```

**Total Cost: ~$125**

## 🔌 **Wiring Diagrams**

### **Basic LED Control Setup**
```
ESP32 Pinout:
GPIO 16 → 220Ω Resistor → Red LED (+) → Red LED (-) → GND
GPIO 17 → 220Ω Resistor → Green LED (+) → Green LED (-) → GND
```

### **4-Channel Relay Setup**
```
ESP32 Pinout:
GPIO 2 → Relay 1 Control
GPIO 4 → Relay 2 Control  
GPIO 5 → Relay 3 Control
GPIO 18 → Relay 4 Control
GND → Relay Module GND
5V → Relay Module VCC
```

### **Push Button Setup**
```
ESP32 Pinout:
GPIO 19 → Button 1 → GND
GPIO 21 → Button 2 → GND
GPIO 22 → Button 3 → GND
GPIO 23 → Button 4 → GND
```

## ⚡ **Power Requirements**
- ESP32: 3.3V (USB powered)
- Relay Module: 5V, 2A supply
- LEDs: 3.3V (ESP32 powered)
- Total: 5V, 2A power supply recommended

## 🔧 **Assembly Steps**

### **Step 1: ESP32 Setup**
1. Insert ESP32 into breadboard
2. Connect USB cable for programming
3. Verify power LED is on

### **Step 2: LED Connections**
1. Connect GPIO 16 to 220Ω resistor
2. Connect resistor to Red LED anode (+)
3. Connect Red LED cathode (-) to GND
4. Repeat for Green LED on GPIO 17

### **Step 3: Relay Module**
1. Connect 5V supply to relay module VCC
2. Connect GND to relay module GND
3. Connect control pins to ESP32 GPIOs
4. Connect relay outputs to devices

### **Step 4: Push Buttons**
1. Connect one side of each button to GPIO pins
2. Connect other side to GND
3. Use internal pull-up resistors

## 🛡️ **Safety Considerations**
- Always use insulated tools
- Turn off power when making connections
- Use proper wire gauges for current
- Test with multimeter before powering on
- Keep water away from electronics

## 📋 **Pre-Assembly Checklist**
- [ ] All components present
- [ ] Tools ready
- [ ] Workspace clear and well-lit
- [ ] Safety equipment on
- [ ] ESP32 not connected to power yet
