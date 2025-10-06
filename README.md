# ESP32 Home Automation System

A complete home automation solution using ESP32 microcontroller with Firebase integration, web dashboard, and manual control capabilities.

## 🚀 Features

- **4-Channel Relay Control** - Control up to 4 devices independently
- **Manual Button Control** - Physical buttons for local control
- **Web Dashboard** - Modern, responsive web interface
- **Firebase Integration** - Real-time cloud synchronization
- **Access Point Mode** - Standalone operation without WiFi
- **Real-time Status** - Live updates and device monitoring
- **Mobile Responsive** - Works on phones, tablets, and desktops

## 📋 Hardware Requirements

### Essential Components
- **ESP32 Development Board** (ESP32-WROOM-32 or similar)
- **4-Channel Relay Module** (5V, Active LOW)
- **4x Push Buttons** (momentary, normally open)
- **Jumper Wires** (male-to-male, male-to-female)
- **Breadboard** (830 tie points recommended)
- **Power Supply** (5V, 2A minimum)
- **USB Cable** (for programming)

### Optional Components
- **Status LEDs** (for visual feedback)
- **Resistors** (10kΩ for pull-ups)
- **Enclosure** (project box for protection)
- **Power Distribution Board**

## 🔧 Software Requirements

- **Arduino IDE** (1.8.19 or later)
- **ESP32 Board Package** (2.0.0 or later)
- **Required Libraries:**
  - Firebase ESP Client
  - WebServer (included with ESP32)
  - WiFi (included with ESP32)

## 📦 Installation

### 1. Arduino IDE Setup

1. Install Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Open Arduino IDE and go to **File > Preferences**
3. Add ESP32 board manager URL:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to **Tools > Board > Boards Manager**
5. Search for "ESP32" and install "esp32 by Espressif Systems"

### 2. Library Installation

1. Open Arduino IDE
2. Go to **Tools > Manage Libraries**
3. Install the following libraries:
   - **Firebase ESP Client** by Mobizt
   - **ArduinoJson** by Benoit Blanchon (dependency)

### 3. Firebase Setup

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Create a new project
3. Enable **Realtime Database**
4. Set database rules to:
   ```json
   {
     "rules": {
       ".read": true,
       ".write": true
     }
   }
   ```
5. Go to **Project Settings > General**
6. Copy your **Web API Key** and **Database URL**

### 4. Configuration

1. Open `esp32_main.ino` in Arduino IDE
2. Update `firebase_config.h` with your credentials:
   ```cpp
   #define FIREBASE_API_KEY "your-api-key-here"
   #define FIREBASE_DATABASE_URL "https://your-project-id.firebaseio.com/"
   #define WIFI_SSID "your-wifi-name"
   #define WIFI_PASSWORD "your-wifi-password"
   ```

## 🔌 Wiring Diagram

### ESP32 to Relay Module
```
ESP32 GPIO 2  → Relay Module IN1
ESP32 GPIO 4  → Relay Module IN2
ESP32 GPIO 5  → Relay Module IN3
ESP32 GPIO 18 → Relay Module IN4
ESP32 GND     → Relay Module GND
ESP32 5V      → Relay Module VCC
```

### ESP32 to Push Buttons
```
ESP32 GPIO 19 → Button 1 (one terminal)
ESP32 GPIO 21 → Button 2 (one terminal)
ESP32 GPIO 22 → Button 3 (one terminal)
ESP32 GPIO 23 → Button 4 (one terminal)
ESP32 GND     → All buttons (other terminal)
```

### Power Connections
```
5V Power Supply → Relay Module VCC
5V Power Supply → ESP32 VIN
GND → Relay Module GND
GND → ESP32 GND
```

## 🚀 Usage

### 1. Upload Code

1. Connect ESP32 to computer via USB
2. Select board: **Tools > Board > ESP32 Arduino > ESP32 Dev Module**
3. Select port: **Tools > Port > [Your ESP32 Port]**
4. Click **Upload** button

### 2. Access Web Dashboard

1. **WiFi Mode**: Connect to your WiFi and find ESP32 IP address in Serial Monitor
2. **Access Point Mode**: Connect to "ESP32-HomeControl" network (password: homecontrol123)
3. Open web browser and go to ESP32 IP address
4. Use the web dashboard to control devices

### 3. Manual Control

- Press physical buttons to toggle devices
- Buttons work independently of web interface
- Status updates in real-time

## 📱 Web Dashboard Features

- **Real-time Control** - Instant device switching
- **Status Monitoring** - Live device status updates
- **Bulk Operations** - Turn all devices on/off
- **Mobile Responsive** - Works on all devices
- **Keyboard Shortcuts** - Ctrl+1-4 for quick control
- **Connection Status** - Visual connection indicator

## 🔧 API Endpoints

### Get Status
```
GET /api/status
Response: {"relay1": true, "relay2": false, "relay3": true, "relay4": false}
```

### Control Single Relay
```
GET /api/relay?relay=1&state=true
Response: {"success": true}
```

### Control All Relays
```
GET /api/all?state=true
Response: {"success": true}
```

## 🛠️ Troubleshooting

### Common Issues

1. **WiFi Connection Failed**
   - Check SSID and password
   - Ensure 2.4GHz network (ESP32 doesn't support 5GHz)
   - Check signal strength

2. **Firebase Connection Failed**
   - Verify API key and database URL
   - Check database rules
   - Ensure internet connectivity

3. **Relays Not Working**
   - Check wiring connections
   - Verify power supply (5V, 2A minimum)
   - Test with multimeter

4. **Web Dashboard Not Loading**
   - Check IP address in Serial Monitor
   - Try different browser
   - Clear browser cache

### Debug Mode

Enable debug mode in `firebase_config.h`:
```cpp
#define DEBUG_MODE true
```

## 📊 Firebase Database Structure

```json
{
  "devices": {
    "esp32": {
      "relay1": true,
      "relay2": false,
      "relay3": true,
      "relay4": false,
      "timestamp": 1234567890
    }
  }
}
```

## 🔒 Security Considerations

- Change default WiFi credentials
- Use strong Firebase database rules
- Consider HTTPS for production use
- Implement authentication for web dashboard
- Regular firmware updates

## 📈 Future Enhancements

- **Scheduling** - Time-based device control
- **Scenes** - Predefined device combinations
- **Voice Control** - Alexa/Google Home integration
- **Sensors** - Temperature, motion, light sensors
- **Notifications** - Push notifications for events
- **Multi-device** - Support for multiple ESP32 units

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🆘 Support

- **Issues**: Report bugs and request features
- **Discussions**: Ask questions and share ideas
- **Wiki**: Detailed documentation and guides

## 🙏 Acknowledgments

- ESP32 community for excellent documentation
- Firebase team for real-time database
- Arduino community for libraries and examples
- Contributors and testers

---

**Built with ❤️ for Smart Home Automation**
