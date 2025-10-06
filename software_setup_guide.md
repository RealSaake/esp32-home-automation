# ESP32 Home Automation - Software Setup Guide

## 🖥️ **Software Installation**

### **Step 1: Arduino IDE Installation**
```bash
# Download Arduino IDE 2.0+ from:
https://www.arduino.cc/en/software

# Install with default settings
# Enable "Install drivers" option
```

### **Step 2: ESP32 Board Package**
```bash
# In Arduino IDE:
File → Preferences → Additional Board Manager URLs
Add: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

# Tools → Board → Boards Manager
Search: "ESP32"
Install: "esp32 by Espressif Systems"
```

### **Step 3: Required Libraries**
```cpp
// Install these libraries via Library Manager:
WiFi.h              // Built-in ESP32 WiFi
WebServer.h         // Built-in ESP32 Web Server
FirebaseESP32.h     // Firebase integration
ArduinoJson.h       // JSON handling
```

## 🔧 **Board Configuration**

### **ESP32 Board Settings**
```
Board: "ESP32 Dev Module"
Upload Speed: 115200
CPU Frequency: 240MHz
Flash Frequency: 80MHz
Flash Mode: QIO
Flash Size: 4MB
Partition Scheme: Default 4MB
Core Debug Level: None
```

### **Port Selection**
```
Windows: COM3, COM4, etc.
Mac: /dev/cu.usbserial-*
Linux: /dev/ttyUSB0, /dev/ttyUSB1
```

## 📝 **Code Templates**

### **Template 1: Basic LED Control (Access Point Mode)**
```cpp
#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "ESP32-Network";
const char* password = "ESP32-password";

// Web server on port 80
WebServer server(80);

// GPIO pins
const int redLED = 16;
const int greenLED = 17;

// LED states
bool redState = false;
bool greenState = false;

void setup() {
  Serial.begin(115200);
  
  // Configure GPIO pins
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  
  // Create Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  // Web server routes
  server.on("/", handleRoot);
  server.on("/red/on", handleRedOn);
  server.on("/red/off", handleRedOff);
  server.on("/green/on", handleGreenOn);
  server.on("/green/off", handleGreenOff);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32 Web Server</title>";
  html += "<style>body{font-family:Arial;text-align:center;margin:50px;}";
  html += "button{background-color:#4CAF50;color:white;padding:15px 32px;";
  html += "text-align:center;font-size:16px;margin:4px 2px;cursor:pointer;border:none;border-radius:4px;}";
  html += "button:hover{background-color:#45a049;}</style></head><body>";
  html += "<h1>ESP32 Web Server</h1>";
  html += "<p>Red LED: " + String(redState ? "ON" : "OFF") + "</p>";
  html += "<button onclick='location.href=\"/red/on\"'>Red ON</button>";
  html += "<button onclick='location.href=\"/red/off\"'>Red OFF</button>";
  html += "<p>Green LED: " + String(greenState ? "ON" : "OFF") + "</p>";
  html += "<button onclick='location.href=\"/green/on\"'>Green ON</button>";
  html += "<button onclick='location.href=\"/green/off\"'>Green OFF</button>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleRedOn() {
  digitalWrite(redLED, HIGH);
  redState = true;
  server.send(200, "text/plain", "Red LED ON");
}

void handleRedOff() {
  digitalWrite(redLED, LOW);
  redState = false;
  server.send(200, "text/plain", "Red LED OFF");
}

void handleGreenOn() {
  digitalWrite(greenLED, HIGH);
  greenState = true;
  server.send(200, "text/plain", "Green LED ON");
}

void handleGreenOff() {
  digitalWrite(greenLED, LOW);
  greenState = false;
  server.send(200, "text/plain", "Green LED OFF");
}
```

### **Template 2: Station Mode (Connect to Existing WiFi)**
```cpp
#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials - CHANGE THESE
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// GPIO pins
const int led1 = 26;
const int led2 = 27;

bool led1State = false;
bool led2State = false;

void setup() {
  Serial.begin(115200);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Web server routes
  server.on("/", handleRoot);
  server.on("/26/on", handleLed1On);
  server.on("/26/off", handleLed1Off);
  server.on("/27/on", handleLed2On);
  server.on("/27/off", handleLed2Off);
  
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32 Web Server</title></head><body>";
  html += "<h1>ESP32 Web Server</h1>";
  html += "<p>LED 26: " + String(led1State ? "ON" : "OFF") + "</p>";
  html += "<button onclick='location.href=\"/26/on\"'>LED 26 ON</button>";
  html += "<button onclick='location.href=\"/26/off\"'>LED 26 OFF</button>";
  html += "<p>LED 27: " + String(led2State ? "ON" : "OFF") + "</p>";
  html += "<button onclick='location.href=\"/27/on\"'>LED 27 ON</button>";
  html += "<button onclick='location.href=\"/27/off\"'>LED 27 OFF</button>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleLed1On() {
  digitalWrite(led1, HIGH);
  led1State = true;
  server.send(200, "text/plain", "LED 26 ON");
}

void handleLed1Off() {
  digitalWrite(led1, LOW);
  led1State = false;
  server.send(200, "text/plain", "LED 26 OFF");
}

void handleLed2On() {
  digitalWrite(led2, HIGH);
  led2State = true;
  server.send(200, "text/plain", "LED 27 ON");
}

void handleLed2Off() {
  digitalWrite(led2, LOW);
  led2State = false;
  server.send(200, "text/plain", "LED 27 OFF");
}
```

## 🔥 **Firebase Setup**

### **Step 1: Create Firebase Project**
1. Go to https://console.firebase.google.com
2. Click "Create a project"
3. Enter project name: "ESP32-Home-Automation"
4. Enable Google Analytics (optional)
5. Click "Create project"

### **Step 2: Enable Realtime Database**
1. In Firebase Console → Build → Realtime Database
2. Click "Create Database"
3. Select location (choose closest to you)
4. Start in "test mode"
5. Click "Enable"

### **Step 3: Set Up Authentication**
1. Build → Authentication → Get Started
2. Sign-in method → Email/Password → Enable
3. Users → Add User
4. Create test user: email@example.com / password123

### **Step 4: Get API Credentials**
1. Project Settings → General → Your apps
2. Add app → Web app
3. Register app: "ESP32-Dashboard"
4. Copy the config object

## 📱 **Mobile Testing**

### **Access Point Mode Testing**
1. Connect to "ESP32-Network" WiFi
2. Password: "ESP32-password"
3. Open browser → http://192.168.4.1
4. Test LED controls

### **Station Mode Testing**
1. Ensure ESP32 connected to your WiFi
2. Check serial monitor for IP address
3. Open browser → http://[ESP32_IP]
4. Test LED controls

## 🐛 **Troubleshooting**

### **Common Issues**
```
Issue: ESP32 not connecting to WiFi
Solution: Check SSID/password, signal strength

Issue: Web server not accessible
Solution: Check IP address, firewall settings

Issue: LEDs not responding
Solution: Check GPIO pins, wiring, power

Issue: Code upload fails
Solution: Check board selection, port, drivers
```

### **Debug Steps**
1. Open Serial Monitor (115200 baud)
2. Check for error messages
3. Verify WiFi connection
4. Test GPIO pins with multimeter
5. Check power supply voltage
