# ESP32 4-Channel Relay Control System

## 🔌 **Hardware Setup**

### **Relay Module Connection**
```
ESP32 GPIO → Relay Module
GPIO 2  → Relay 1 Control
GPIO 4  → Relay 2 Control  
GPIO 5  → Relay 3 Control
GPIO 18 → Relay 4 Control
GND     → Relay Module GND
5V      → Relay Module VCC (External Supply)
```

### **Push Button Setup**
```
ESP32 GPIO → Push Button → GND
GPIO 19 → Button 1 → GND
GPIO 21 → Button 2 → GND
GPIO 22 → Button 3 → GND
GPIO 23 → Button 4 → GND
```

## 💻 **Complete Relay Control Code**

```cpp
#include <WiFi.h>
#include <WebServer.h>

// WiFi Access Point credentials
const char* ssid = "ESP32-Hotspot";
const char* password = "12345678";

WebServer server(80);

// Relay control pins (Active LOW)
const int relay1 = 2;
const int relay2 = 4;
const int relay3 = 5;
const int relay4 = 18;

// Push button pins
const int button1 = 19;
const int button2 = 21;
const int button3 = 22;
const int button4 = 23;

// Relay states
bool relay1State = false;
bool relay2State = false;
bool relay3State = false;
bool relay4State = false;

// Button states
bool button1State = false;
bool button2State = false;
bool button3State = false;
bool button4State = false;

void setup() {
  Serial.begin(115200);
  
  // Configure relay pins as outputs
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  
  // Configure button pins as inputs with pull-up
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  
  // Initialize relays as OFF (HIGH for Active LOW)
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  
  // Create Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  // Web server routes
  server.on("/", handleRoot);
  server.on("/relay1/on", handleRelay1On);
  server.on("/relay1/off", handleRelay1Off);
  server.on("/relay2/on", handleRelay2On);
  server.on("/relay2/off", handleRelay2Off);
  server.on("/relay3/on", handleRelay3On);
  server.on("/relay3/off", handleRelay3Off);
  server.on("/relay4/on", handleRelay4On);
  server.on("/relay4/off", handleRelay4Off);
  server.on("/all/on", handleAllOn);
  server.on("/all/off", handleAllOff);
  server.on("/status", handleStatus);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  
  // Check manual button presses
  checkButtons();
  
  delay(50); // Small delay for stability
}

void checkButtons() {
  // Check button 1
  if (digitalRead(button1) == LOW && !button1State) {
    button1State = true;
    relay1State = !relay1State;
    digitalWrite(relay1, relay1State ? LOW : HIGH);
    Serial.println("Button 1 pressed - Relay 1: " + String(relay1State ? "ON" : "OFF"));
    delay(200); // Debounce
  }
  if (digitalRead(button1) == HIGH) {
    button1State = false;
  }
  
  // Check button 2
  if (digitalRead(button2) == LOW && !button2State) {
    button2State = true;
    relay2State = !relay2State;
    digitalWrite(relay2, relay2State ? LOW : HIGH);
    Serial.println("Button 2 pressed - Relay 2: " + String(relay2State ? "ON" : "OFF"));
    delay(200);
  }
  if (digitalRead(button2) == HIGH) {
    button2State = false;
  }
  
  // Check button 3
  if (digitalRead(button3) == LOW && !button3State) {
    button3State = true;
    relay3State = !relay3State;
    digitalWrite(relay3, relay3State ? LOW : HIGH);
    Serial.println("Button 3 pressed - Relay 3: " + String(relay3State ? "ON" : "OFF"));
    delay(200);
  }
  if (digitalRead(button3) == HIGH) {
    button3State = false;
  }
  
  // Check button 4
  if (digitalRead(button4) == LOW && !button4State) {
    button4State = true;
    relay4State = !relay4State;
    digitalWrite(relay4, relay4State ? LOW : HIGH);
    Serial.println("Button 4 pressed - Relay 4: " + String(relay4State ? "ON" : "OFF"));
    delay(200);
  }
  if (digitalRead(button4) == HIGH) {
    button4State = false;
  }
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32 Home Automation</title>";
  html += "<style>";
  html += "body{font-family:Arial;text-align:center;margin:20px;background-color:#f0f0f0;}";
  html += "h1{color:#333;margin-bottom:30px;}";
  html += ".relay-container{display:inline-block;margin:20px;padding:20px;";
  html += "background-color:white;border-radius:10px;box-shadow:0 2px 5px rgba(0,0,0,0.1);}";
  html += "button{background-color:#4CAF50;color:white;padding:15px 25px;";
  html += "text-align:center;font-size:16px;margin:5px;cursor:pointer;border:none;border-radius:5px;}";
  html += "button:hover{background-color:#45a049;}";
  html += ".off{background-color:#f44336;}";
  html += ".off:hover{background-color:#da190b;}";
  html += ".status{font-size:18px;font-weight:bold;margin:10px 0;}";
  html += ".on{color:#4CAF50;}";
  html += ".off{color:#f44336;}";
  html += "</style></head><body>";
  
  html += "<h1>🏠 ESP32 Home Automation</h1>";
  
  // Relay 1
  html += "<div class='relay-container'>";
  html += "<h3>Relay 1</h3>";
  html += "<div class='status " + String(relay1State ? "on" : "off") + "'>";
  html += "Status: " + String(relay1State ? "ON" : "OFF") + "</div>";
  if (relay1State) {
    html += "<button class='off' onclick='location.href=\"/relay1/off\"'>Turn OFF</button>";
  } else {
    html += "<button onclick='location.href=\"/relay1/on\"'>Turn ON</button>";
  }
  html += "</div>";
  
  // Relay 2
  html += "<div class='relay-container'>";
  html += "<h3>Relay 2</h3>";
  html += "<div class='status " + String(relay2State ? "on" : "off") + "'>";
  html += "Status: " + String(relay2State ? "ON" : "OFF") + "</div>";
  if (relay2State) {
    html += "<button class='off' onclick='location.href=\"/relay2/off\"'>Turn OFF</button>";
  } else {
    html += "<button onclick='location.href=\"/relay2/on\"'>Turn ON</button>";
  }
  html += "</div>";
  
  // Relay 3
  html += "<div class='relay-container'>";
  html += "<h3>Relay 3</h3>";
  html += "<div class='status " + String(relay3State ? "on" : "off") + "'>";
  html += "Status: " + String(relay3State ? "ON" : "OFF") + "</div>";
  if (relay3State) {
    html += "<button class='off' onclick='location.href=\"/relay3/off\"'>Turn OFF</button>";
  } else {
    html += "<button onclick='location.href=\"/relay3/on\"'>Turn ON</button>";
  }
  html += "</div>";
  
  // Relay 4
  html += "<div class='relay-container'>";
  html += "<h3>Relay 4</h3>";
  html += "<div class='status " + String(relay4State ? "on" : "off") + "'>";
  html += "Status: " + String(relay4State ? "ON" : "OFF") + "</div>";
  if (relay4State) {
    html += "<button class='off' onclick='location.href=\"/relay4/off\"'>Turn OFF</button>";
  } else {
    html += "<button onclick='location.href=\"/relay4/on\"'>Turn ON</button>";
  }
  html += "</div>";
  
  // Master controls
  html += "<div style='margin-top:30px;'>";
  html += "<button style='background-color:#2196F3;' onclick='location.href=\"/all/on\"'>All ON</button>";
  html += "<button style='background-color:#f44336;' onclick='location.href=\"/all/off\"'>All OFF</button>";
  html += "<button style='background-color:#FF9800;' onclick='location.reload()'>Refresh Status</button>";
  html += "</div>";
  
  html += "<p style='margin-top:30px;color:#666;'>";
  html += "Manual buttons also work! Try pressing the physical buttons.";
  html += "</p>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// Relay control handlers
void handleRelay1On() {
  digitalWrite(relay1, LOW);
  relay1State = true;
  server.send(200, "text/plain", "Relay 1 ON");
}

void handleRelay1Off() {
  digitalWrite(relay1, HIGH);
  relay1State = false;
  server.send(200, "text/plain", "Relay 1 OFF");
}

void handleRelay2On() {
  digitalWrite(relay2, LOW);
  relay2State = true;
  server.send(200, "text/plain", "Relay 2 ON");
}

void handleRelay2Off() {
  digitalWrite(relay2, HIGH);
  relay2State = false;
  server.send(200, "text/plain", "Relay 2 OFF");
}

void handleRelay3On() {
  digitalWrite(relay3, LOW);
  relay3State = true;
  server.send(200, "text/plain", "Relay 3 ON");
}

void handleRelay3Off() {
  digitalWrite(relay3, HIGH);
  relay3State = false;
  server.send(200, "text/plain", "Relay 3 OFF");
}

void handleRelay4On() {
  digitalWrite(relay4, LOW);
  relay4State = true;
  server.send(200, "text/plain", "Relay 4 ON");
}

void handleRelay4Off() {
  digitalWrite(relay4, HIGH);
  relay4State = false;
  server.send(200, "text/plain", "Relay 4 OFF");
}

void handleAllOn() {
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  relay1State = true;
  relay2State = true;
  relay3State = true;
  relay4State = true;
  server.send(200, "text/plain", "All Relays ON");
}

void handleAllOff() {
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  relay1State = false;
  relay2State = false;
  relay3State = false;
  relay4State = false;
  server.send(200, "text/plain", "All Relays OFF");
}

void handleStatus() {
  String status = "Relay1:" + String(relay1State) + ",Relay2:" + String(relay2State) + 
                  ",Relay3:" + String(relay3State) + ",Relay4:" + String(relay4State);
  server.send(200, "text/plain", status);
}
```

## 🧪 **Testing Procedure**

### **Step 1: Hardware Test**
1. Connect ESP32 to computer
2. Upload the code
3. Open Serial Monitor (115200 baud)
4. Check for "AP IP address: 192.168.4.1"

### **Step 2: WiFi Connection Test**
1. On phone/computer, connect to "ESP32-Hotspot"
2. Password: "12345678"
3. Open browser → http://192.168.4.1
4. Verify web dashboard loads

### **Step 3: Web Control Test**
1. Click "Turn ON" for Relay 1
2. Verify relay clicks and LED indicator
3. Click "Turn OFF" for Relay 1
4. Verify relay turns off
5. Test all 4 relays individually

### **Step 4: Manual Button Test**
1. Press physical Button 1
2. Verify Relay 1 toggles
3. Check web dashboard updates
4. Test all 4 buttons

### **Step 5: Master Control Test**
1. Click "All ON" button
2. Verify all relays turn on
3. Click "All OFF" button
4. Verify all relays turn off

## 🔧 **Troubleshooting**

### **Common Issues**
```
Issue: Relays not responding
Solution: Check wiring, power supply, Active LOW configuration

Issue: Buttons not working
Solution: Check pull-up resistors, wiring, debounce timing

Issue: Web server not accessible
Solution: Check WiFi connection, IP address, firewall

Issue: Code upload fails
Solution: Check board selection, port, drivers, boot button
```

### **Debug Commands**
```cpp
// Add to setup() for debugging
Serial.println("Relay 1 State: " + String(relay1State));
Serial.println("Button 1 State: " + String(digitalRead(button1)));
```

## 📱 **Mobile Optimization**

The web interface is already mobile-responsive with:
- Touch-friendly buttons
- Responsive design
- Clear status indicators
- Easy navigation

## 🔄 **Next Steps**

1. **Test basic functionality** with LEDs first
2. **Add relay modules** one by one
3. **Test each relay** individually
4. **Add manual buttons** for backup control
5. **Deploy to production** environment
