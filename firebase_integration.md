# ESP32 Firebase Home Automation Integration

## 🔥 **Firebase Setup Guide**

### **Step 1: Create Firebase Project**
1. Go to https://console.firebase.google.com
2. Click "Create a project"
3. Project name: "ESP32-Home-Automation"
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
4. Create test user: `test@example.com` / `password123`

### **Step 4: Get API Credentials**
1. Project Settings → General → Your apps
2. Add app → Web app
3. Register app: "ESP32-Dashboard"
4. Copy the config object

## 💻 **Complete Firebase Integration Code**

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Firebase credentials
#define FIREBASE_HOST "your-project-id-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "your-firebase-auth-key"

// Web server
WebServer server(80);

// Relay control pins
const int relay1 = 2;
const int relay2 = 4;
const int relay3 = 5;
const int relay4 = 18;

// Button pins
const int button1 = 19;
const int button2 = 21;
const int button3 = 22;
const int button4 = 23;

// Relay states
bool relay1State = false;
bool relay2State = false;
bool relay3State = false;
bool relay4State = false;

// Firebase object
FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  
  // Configure pins
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  
  // Initialize relays as OFF
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
  // Set initial values in Firebase
  Firebase.setBool(firebaseData, "/relay1", false);
  Firebase.setBool(firebaseData, "/relay2", false);
  Firebase.setBool(firebaseData, "/relay3", false);
  Firebase.setBool(firebaseData, "/relay4", false);
  
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
  
  // Listen for Firebase changes
  listenToFirebase();
  
  delay(50);
}

void checkButtons() {
  // Check button 1
  if (digitalRead(button1) == LOW) {
    relay1State = !relay1State;
    digitalWrite(relay1, relay1State ? LOW : HIGH);
    Firebase.setBool(firebaseData, "/relay1", relay1State);
    Serial.println("Button 1 pressed - Relay 1: " + String(relay1State ? "ON" : "OFF"));
    delay(200);
  }
  
  // Check button 2
  if (digitalRead(button2) == LOW) {
    relay2State = !relay2State;
    digitalWrite(relay2, relay2State ? LOW : HIGH);
    Firebase.setBool(firebaseData, "/relay2", relay2State);
    Serial.println("Button 2 pressed - Relay 2: " + String(relay2State ? "ON" : "OFF"));
    delay(200);
  }
  
  // Check button 3
  if (digitalRead(button3) == LOW) {
    relay3State = !relay3State;
    digitalWrite(relay3, relay3State ? LOW : HIGH);
    Firebase.setBool(firebaseData, "/relay3", relay3State);
    Serial.println("Button 3 pressed - Relay 3: " + String(relay3State ? "ON" : "OFF"));
    delay(200);
  }
  
  // Check button 4
  if (digitalRead(button4) == LOW) {
    relay4State = !relay4State;
    digitalWrite(relay4, relay4State ? LOW : HIGH);
    Firebase.setBool(firebaseData, "/relay4", relay4State);
    Serial.println("Button 4 pressed - Relay 4: " + String(relay4State ? "ON" : "OFF"));
    delay(200);
  }
}

void listenToFirebase() {
  // Listen for changes in Firebase
  if (Firebase.getBool(firebaseData, "/relay1")) {
    bool newState = firebaseData.boolData();
    if (newState != relay1State) {
      relay1State = newState;
      digitalWrite(relay1, relay1State ? LOW : HIGH);
      Serial.println("Firebase: Relay 1 " + String(relay1State ? "ON" : "OFF"));
    }
  }
  
  if (Firebase.getBool(firebaseData, "/relay2")) {
    bool newState = firebaseData.boolData();
    if (newState != relay2State) {
      relay2State = newState;
      digitalWrite(relay2, relay2State ? LOW : HIGH);
      Serial.println("Firebase: Relay 2 " + String(relay2State ? "ON" : "OFF"));
    }
  }
  
  if (Firebase.getBool(firebaseData, "/relay3")) {
    bool newState = firebaseData.boolData();
    if (newState != relay3State) {
      relay3State = newState;
      digitalWrite(relay3, relay3State ? LOW : HIGH);
      Serial.println("Firebase: Relay 3 " + String(relay3State ? "ON" : "OFF"));
    }
  }
  
  if (Firebase.getBool(firebaseData, "/relay4")) {
    bool newState = firebaseData.boolData();
    if (newState != relay4State) {
      relay4State = newState;
      digitalWrite(relay4, relay4State ? LOW : HIGH);
      Serial.println("Firebase: Relay 4 " + String(relay4State ? "ON" : "OFF"));
    }
  }
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32 Firebase Home Automation</title>";
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
  
  html += "<h1>🏠 ESP32 Firebase Home Automation</h1>";
  html += "<p>Real-time sync with Firebase Database</p>";
  
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
  html += "Changes sync with Firebase in real-time!";
  html += "</p>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// Relay control handlers
void handleRelay1On() {
  digitalWrite(relay1, LOW);
  relay1State = true;
  Firebase.setBool(firebaseData, "/relay1", true);
  server.send(200, "text/plain", "Relay 1 ON");
}

void handleRelay1Off() {
  digitalWrite(relay1, HIGH);
  relay1State = false;
  Firebase.setBool(firebaseData, "/relay1", false);
  server.send(200, "text/plain", "Relay 1 OFF");
}

void handleRelay2On() {
  digitalWrite(relay2, LOW);
  relay2State = true;
  Firebase.setBool(firebaseData, "/relay2", true);
  server.send(200, "text/plain", "Relay 2 ON");
}

void handleRelay2Off() {
  digitalWrite(relay2, HIGH);
  relay2State = false;
  Firebase.setBool(firebaseData, "/relay2", false);
  server.send(200, "text/plain", "Relay 2 OFF");
}

void handleRelay3On() {
  digitalWrite(relay3, LOW);
  relay3State = true;
  Firebase.setBool(firebaseData, "/relay3", true);
  server.send(200, "text/plain", "Relay 3 ON");
}

void handleRelay3Off() {
  digitalWrite(relay3, HIGH);
  relay3State = false;
  Firebase.setBool(firebaseData, "/relay3", false);
  server.send(200, "text/plain", "Relay 3 OFF");
}

void handleRelay4On() {
  digitalWrite(relay4, LOW);
  relay4State = true;
  Firebase.setBool(firebaseData, "/relay4", true);
  server.send(200, "text/plain", "Relay 4 ON");
}

void handleRelay4Off() {
  digitalWrite(relay4, HIGH);
  relay4State = false;
  Firebase.setBool(firebaseData, "/relay4", false);
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
  Firebase.setBool(firebaseData, "/relay1", true);
  Firebase.setBool(firebaseData, "/relay2", true);
  Firebase.setBool(firebaseData, "/relay3", true);
  Firebase.setBool(firebaseData, "/relay4", true);
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
  Firebase.setBool(firebaseData, "/relay1", false);
  Firebase.setBool(firebaseData, "/relay2", false);
  Firebase.setBool(firebaseData, "/relay3", false);
  Firebase.setBool(firebaseData, "/relay4", false);
  server.send(200, "text/plain", "All Relays OFF");
}

void handleStatus() {
  String status = "Relay1:" + String(relay1State) + ",Relay2:" + String(relay2State) + 
                  ",Relay3:" + String(relay3State) + ",Relay4:" + String(relay4State);
  server.send(200, "text/plain", status);
}
```

## 🌐 **Web Dashboard HTML**

```html
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP32 Firebase Home Automation</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin: 20px;
            background-color: #f0f0f0;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            background: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        }
        .relay-container {
            display: inline-block;
            margin: 20px;
            padding: 20px;
            background-color: #f9f9f9;
            border-radius: 10px;
            border: 2px solid #ddd;
        }
        button {
            background-color: #4CAF50;
            color: white;
            padding: 15px 25px;
            text-align: center;
            font-size: 16px;
            margin: 5px;
            cursor: pointer;
            border: none;
            border-radius: 5px;
        }
        button:hover {
            background-color: #45a049;
        }
        .off {
            background-color: #f44336;
        }
        .off:hover {
            background-color: #da190b;
        }
        .status {
            font-size: 18px;
            font-weight: bold;
            margin: 10px 0;
        }
        .on {
            color: #4CAF50;
        }
        .off {
            color: #f44336;
        }
        .master-controls {
            margin-top: 30px;
            padding: 20px;
            background-color: #e9e9e9;
            border-radius: 10px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🏠 ESP32 Firebase Home Automation</h1>
        <p>Real-time control with Firebase Database</p>
        
        <div id="relay1" class="relay-container">
            <h3>Relay 1</h3>
            <div class="status off" id="status1">Status: OFF</div>
            <button onclick="toggleRelay(1)">Turn ON</button>
        </div>
        
        <div id="relay2" class="relay-container">
            <h3>Relay 2</h3>
            <div class="status off" id="status2">Status: OFF</div>
            <button onclick="toggleRelay(2)">Turn ON</button>
        </div>
        
        <div id="relay3" class="relay-container">
            <h3>Relay 3</h3>
            <div class="status off" id="status3">Status: OFF</div>
            <button onclick="toggleRelay(3)">Turn ON</button>
        </div>
        
        <div id="relay4" class="relay-container">
            <h3>Relay 4</h3>
            <div class="status off" id="status4">Status: OFF</div>
            <button onclick="toggleRelay(4)">Turn ON</button>
        </div>
        
        <div class="master-controls">
            <button style="background-color:#2196F3;" onclick="allOn()">All ON</button>
            <button style="background-color:#f44336;" onclick="allOff()">All OFF</button>
            <button style="background-color:#FF9800;" onclick="refreshStatus()">Refresh Status</button>
        </div>
        
        <p style="margin-top:30px;color:#666;">
            Changes sync with Firebase in real-time!
        </p>
    </div>

    <script>
        // Firebase configuration
        const firebaseConfig = {
            // Add your Firebase config here
        };
        
        // Initialize Firebase
        firebase.initializeApp(firebaseConfig);
        const database = firebase.database();
        
        // Listen for real-time updates
        database.ref('/').on('value', (snapshot) => {
            const data = snapshot.val();
            updateRelayStatus(1, data.relay1);
            updateRelayStatus(2, data.relay2);
            updateRelayStatus(3, data.relay3);
            updateRelayStatus(4, data.relay4);
        });
        
        function updateRelayStatus(relayNum, state) {
            const statusElement = document.getElementById(`status${relayNum}`);
            const buttonElement = document.querySelector(`#relay${relayNum} button`);
            
            if (state) {
                statusElement.textContent = "Status: ON";
                statusElement.className = "status on";
                buttonElement.textContent = "Turn OFF";
                buttonElement.className = "off";
            } else {
                statusElement.textContent = "Status: OFF";
                statusElement.className = "status off";
                buttonElement.textContent = "Turn ON";
                buttonElement.className = "";
            }
        }
        
        function toggleRelay(relayNum) {
            const currentState = document.getElementById(`status${relayNum}`).textContent.includes("ON");
            const newState = !currentState;
            
            database.ref(`/relay${relayNum}`).set(newState);
        }
        
        function allOn() {
            database.ref('/relay1').set(true);
            database.ref('/relay2').set(true);
            database.ref('/relay3').set(true);
            database.ref('/relay4').set(true);
        }
        
        function allOff() {
            database.ref('/relay1').set(false);
            database.ref('/relay2').set(false);
            database.ref('/relay3').set(false);
            database.ref('/relay4').set(false);
        }
        
        function refreshStatus() {
            location.reload();
        }
    </script>
</body>
</html>
```

## 🧪 **Testing Procedure**

### **Step 1: Firebase Setup Test**
1. Create Firebase project
2. Enable Realtime Database
3. Set up authentication
4. Get API credentials

### **Step 2: Code Upload Test**
1. Update WiFi credentials in code
2. Update Firebase credentials in code
3. Upload to ESP32
4. Check serial monitor for connection status

### **Step 3: Web Control Test**
1. Access ESP32 web interface
2. Test individual relay controls
3. Test master controls
4. Verify Firebase database updates

### **Step 4: Real-time Sync Test**
1. Open Firebase console
2. Change relay values manually
3. Verify ESP32 responds
4. Test from multiple devices

## 🔧 **Troubleshooting**

### **Common Issues**
```
Issue: Firebase connection fails
Solution: Check credentials, internet connection, database rules

Issue: Real-time sync not working
Solution: Check Firebase listeners, database permissions

Issue: Web interface not updating
Solution: Check JavaScript console, Firebase config

Issue: ESP32 not responding to Firebase
Solution: Check WiFi connection, Firebase credentials
```

## 🚀 **Next Steps**

1. **Set up Firebase project** with authentication
2. **Upload ESP32 code** with your credentials
3. **Deploy web dashboard** to GitHub Pages
4. **Test real-time sync** between devices
5. **Add voice control** with Google Assistant
