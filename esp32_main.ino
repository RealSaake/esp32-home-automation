/*
 * ESP32 Home Automation System
 * Complete relay control with WiFi, Firebase, and manual buttons
 * 
 * Features:
 * - 4-channel relay control
 * - Manual push button control
 * - Web server for remote control
 * - Firebase real-time sync
 * - Access Point mode for standalone operation
 * - Real-time status feedback
 */

#include <WiFi.h>
#include <WebServer.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Firebase configuration
#define API_KEY "YOUR_FIREBASE_API_KEY"
#define DATABASE_URL "YOUR_FIREBASE_DATABASE_URL"
#define USER_EMAIL "admin@home.local"
#define USER_PASSWORD "admin123"

// Relay pins (Active LOW - relay turns ON when pin is LOW)
const int relayPins[] = {2, 4, 5, 18};  // GPIO pins for 4 relays
const int buttonPins[] = {19, 21, 22, 23}; // GPIO pins for 4 buttons

// Relay states
bool relayStates[] = {false, false, false, false};
bool lastButtonStates[] = {HIGH, HIGH, HIGH, HIGH};
bool currentButtonStates[] = {HIGH, HIGH, HIGH, HIGH};

// Web server
WebServer server(80);

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Access Point credentials
const char* ap_ssid = "ESP32-HomeControl";
const char* ap_password = "homecontrol123";

// Timing variables
unsigned long lastFirebaseUpdate = 0;
const unsigned long firebaseUpdateInterval = 1000; // Update every 1 second

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Home Automation System Starting...");
  
  // Initialize relay pins
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH); // Start with relays OFF (HIGH = OFF for active LOW relays)
  }
  
  // Initialize button pins with internal pullup
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  
  // Try to connect to WiFi
  if (connectToWiFi()) {
    setupFirebase();
    setupWebServer();
    Serial.println("Connected to WiFi and Firebase!");
  } else {
    // Start Access Point mode
    startAccessPoint();
    setupWebServer();
    Serial.println("Started Access Point mode");
  }
  
  Serial.println("System ready!");
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Read button states
  readButtons();
  
  // Update Firebase periodically
  if (millis() - lastFirebaseUpdate > firebaseUpdateInterval) {
    updateFirebase();
    lastFirebaseUpdate = millis();
  }
  
  delay(50); // Small delay for stability
}

bool connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("WiFi connected! IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println();
    Serial.println("WiFi connection failed");
    return false;
  }
}

void startAccessPoint() {
  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("Access Point started: ");
  Serial.println(ap_ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}

void setupFirebase() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase authentication successful");
  } else {
    Serial.printf("Firebase auth failed: %s\n", config.signer.signupError.message.c_str());
  }
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setupWebServer() {
  // Root page
  server.on("/", handleRoot);
  
  // API endpoints
  server.on("/api/status", handleStatus);
  server.on("/api/relay", handleRelay);
  server.on("/api/all", handleAllRelays);
  
  // Serve static files
  server.on("/style.css", HTTP_GET, []() {
    server.send(200, "text/css", getCSS());
  });
  
  server.on("/script.js", HTTP_GET, []() {
    server.send(200, "application/javascript", getJavaScript());
  });
  
  server.begin();
  Serial.println("Web server started");
}

void handleRoot() {
  String html = getHTML();
  server.send(200, "text/html", html);
}

void handleStatus() {
  String json = "{";
  for (int i = 0; i < 4; i++) {
    json += "\"relay" + String(i + 1) + "\":" + String(relayStates[i] ? "true" : "false");
    if (i < 3) json += ",";
  }
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleRelay() {
  if (server.hasArg("relay") && server.hasArg("state")) {
    int relay = server.arg("relay").toInt() - 1;
    bool state = server.arg("state") == "true";
    
    if (relay >= 0 && relay < 4) {
      setRelay(relay, state);
      server.send(200, "application/json", "{\"success\":true}");
    } else {
      server.send(400, "application/json", "{\"error\":\"Invalid relay\"}");
    }
  } else {
    server.send(400, "application/json", "{\"error\":\"Missing parameters\"}");
  }
}

void handleAllRelays() {
  if (server.hasArg("state")) {
    bool state = server.arg("state") == "true";
    for (int i = 0; i < 4; i++) {
      setRelay(i, state);
    }
    server.send(200, "application/json", "{\"success\":true}");
  } else {
    server.send(400, "application/json", "{\"error\":\"Missing state parameter\"}");
  }
}

void readButtons() {
  for (int i = 0; i < 4; i++) {
    currentButtonStates[i] = digitalRead(buttonPins[i]);
    
    // Check for button press (LOW to HIGH transition)
    if (lastButtonStates[i] == LOW && currentButtonStates[i] == HIGH) {
      // Button was pressed
      relayStates[i] = !relayStates[i];
      setRelay(i, relayStates[i]);
      Serial.println("Button " + String(i + 1) + " pressed - Relay " + String(i + 1) + " " + (relayStates[i] ? "ON" : "OFF"));
    }
    
    lastButtonStates[i] = currentButtonStates[i];
  }
}

void setRelay(int relay, bool state) {
  if (relay >= 0 && relay < 4) {
    relayStates[relay] = state;
    digitalWrite(relayPins[relay], state ? LOW : HIGH); // Active LOW relay
    Serial.println("Relay " + String(relay + 1) + " " + (state ? "ON" : "OFF"));
  }
}

void updateFirebase() {
  if (Firebase.ready()) {
    FirebaseJson json;
    for (int i = 0; i < 4; i++) {
      json.set("relay" + String(i + 1), relayStates[i]);
    }
    json.set("timestamp", millis());
    
    if (Firebase.RTDB.setJSON(&fbdo, "/devices/esp32", &json)) {
      // Success
    } else {
      Serial.println("Firebase update failed: " + fbdo.errorReason());
    }
  }
}

String getHTML() {
  return R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Home Control</title>
    <link rel="stylesheet" href="/style.css">
</head>
<body>
    <div class="container">
        <header>
            <h1>🏠 Home Automation Control</h1>
            <div class="status-indicator">
                <span id="connection-status">🟢 Connected</span>
            </div>
        </header>
        
        <div class="controls">
            <div class="relay-grid">
                <div class="relay-card" data-relay="1">
                    <h3>Device 1</h3>
                    <div class="relay-control">
                        <label class="switch">
                            <input type="checkbox" id="relay1">
                            <span class="slider"></span>
                        </label>
                        <span class="status" id="status1">OFF</span>
                    </div>
                </div>
                
                <div class="relay-card" data-relay="2">
                    <h3>Device 2</h3>
                    <div class="relay-control">
                        <label class="switch">
                            <input type="checkbox" id="relay2">
                            <span class="slider"></span>
                        </label>
                        <span class="status" id="status2">OFF</span>
                    </div>
                </div>
                
                <div class="relay-card" data-relay="3">
                    <h3>Device 3</h3>
                    <div class="relay-control">
                        <label class="switch">
                            <input type="checkbox" id="relay3">
                            <span class="slider"></span>
                        </label>
                        <span class="status" id="status3">OFF</span>
                    </div>
                </div>
                
                <div class="relay-card" data-relay="4">
                    <h3>Device 4</h3>
                    <div class="relay-control">
                        <label class="switch">
                            <input type="checkbox" id="relay4">
                            <span class="slider"></span>
                        </label>
                        <span class="status" id="status4">OFF</span>
                    </div>
                </div>
            </div>
            
            <div class="bulk-controls">
                <button id="all-on" class="btn btn-success">All ON</button>
                <button id="all-off" class="btn btn-danger">All OFF</button>
            </div>
        </div>
        
        <footer>
            <p>ESP32 Home Automation System | Real-time Control</p>
        </footer>
    </div>
    
    <script src="/script.js"></script>
</body>
</html>
)rawliteral";
}

String getCSS() {
  return R"rawliteral(
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

body {
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    min-height: 100vh;
    color: #333;
}

.container {
    max-width: 1200px;
    margin: 0 auto;
    padding: 20px;
}

header {
    text-align: center;
    margin-bottom: 30px;
    background: rgba(255, 255, 255, 0.1);
    padding: 20px;
    border-radius: 15px;
    backdrop-filter: blur(10px);
}

header h1 {
    color: white;
    font-size: 2.5em;
    margin-bottom: 10px;
}

.status-indicator {
    color: white;
    font-weight: bold;
}

.relay-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
    gap: 20px;
    margin-bottom: 30px;
}

.relay-card {
    background: rgba(255, 255, 255, 0.95);
    padding: 25px;
    border-radius: 15px;
    box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
    text-align: center;
    transition: transform 0.3s ease;
}

.relay-card:hover {
    transform: translateY(-5px);
}

.relay-card h3 {
    margin-bottom: 20px;
    color: #333;
    font-size: 1.3em;
}

.relay-control {
    display: flex;
    flex-direction: column;
    align-items: center;
    gap: 15px;
}

.switch {
    position: relative;
    display: inline-block;
    width: 60px;
    height: 34px;
}

.switch input {
    opacity: 0;
    width: 0;
    height: 0;
}

.slider {
    position: absolute;
    cursor: pointer;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background-color: #ccc;
    transition: .4s;
    border-radius: 34px;
}

.slider:before {
    position: absolute;
    content: "";
    height: 26px;
    width: 26px;
    left: 4px;
    bottom: 4px;
    background-color: white;
    transition: .4s;
    border-radius: 50%;
}

input:checked + .slider {
    background-color: #4CAF50;
}

input:checked + .slider:before {
    transform: translateX(26px);
}

.status {
    font-weight: bold;
    font-size: 1.1em;
    padding: 5px 15px;
    border-radius: 20px;
    background: #f0f0f0;
}

.status.on {
    background: #d4edda;
    color: #155724;
}

.status.off {
    background: #f8d7da;
    color: #721c24;
}

.bulk-controls {
    text-align: center;
    margin-bottom: 30px;
}

.btn {
    padding: 12px 30px;
    margin: 0 10px;
    border: none;
    border-radius: 25px;
    font-size: 1.1em;
    font-weight: bold;
    cursor: pointer;
    transition: all 0.3s ease;
}

.btn-success {
    background: #28a745;
    color: white;
}

.btn-success:hover {
    background: #218838;
    transform: translateY(-2px);
}

.btn-danger {
    background: #dc3545;
    color: white;
}

.btn-danger:hover {
    background: #c82333;
    transform: translateY(-2px);
}

footer {
    text-align: center;
    color: white;
    margin-top: 30px;
    opacity: 0.8;
}

@media (max-width: 768px) {
    .relay-grid {
        grid-template-columns: 1fr;
    }
    
    header h1 {
        font-size: 2em;
    }
    
    .bulk-controls {
        display: flex;
        flex-direction: column;
        gap: 10px;
    }
    
    .btn {
        margin: 5px 0;
    }
}
)rawliteral";
}

String getJavaScript() {
  return R"rawliteral(
class HomeControl {
    constructor() {
        this.relays = {};
        this.init();
    }
    
    init() {
        this.setupEventListeners();
        this.startStatusUpdates();
        this.loadInitialState();
    }
    
    setupEventListeners() {
        // Individual relay controls
        for (let i = 1; i <= 4; i++) {
            const checkbox = document.getElementById(`relay${i}`);
            checkbox.addEventListener('change', () => this.toggleRelay(i));
        }
        
        // Bulk controls
        document.getElementById('all-on').addEventListener('click', () => this.allRelays(true));
        document.getElementById('all-off').addEventListener('click', () => this.allRelays(false));
    }
    
    async toggleRelay(relayNumber) {
        const checkbox = document.getElementById(`relay${relayNumber}`);
        const state = checkbox.checked;
        
        try {
            const response = await fetch(`/api/relay?relay=${relayNumber}&state=${state}`, {
                method: 'GET'
            });
            
            if (response.ok) {
                this.updateRelayStatus(relayNumber, state);
            } else {
                // Revert checkbox on error
                checkbox.checked = !state;
                console.error('Failed to control relay');
            }
        } catch (error) {
            checkbox.checked = !state;
            console.error('Error controlling relay:', error);
        }
    }
    
    async allRelays(state) {
        try {
            const response = await fetch(`/api/all?state=${state}`, {
                method: 'GET'
            });
            
            if (response.ok) {
                for (let i = 1; i <= 4; i++) {
                    const checkbox = document.getElementById(`relay${i}`);
                    checkbox.checked = state;
                    this.updateRelayStatus(i, state);
                }
            }
        } catch (error) {
            console.error('Error controlling all relays:', error);
        }
    }
    
    updateRelayStatus(relayNumber, state) {
        const statusElement = document.getElementById(`status${relayNumber}`);
        statusElement.textContent = state ? 'ON' : 'OFF';
        statusElement.className = `status ${state ? 'on' : 'off'}`;
    }
    
    async loadInitialState() {
        try {
            const response = await fetch('/api/status');
            const data = await response.json();
            
            for (let i = 1; i <= 4; i++) {
                const state = data[`relay${i}`];
                const checkbox = document.getElementById(`relay${i}`);
                checkbox.checked = state;
                this.updateRelayStatus(i, state);
            }
        } catch (error) {
            console.error('Error loading initial state:', error);
        }
    }
    
    startStatusUpdates() {
        // Update status every 2 seconds
        setInterval(async () => {
            try {
                const response = await fetch('/api/status');
                const data = await response.json();
                
                for (let i = 1; i <= 4; i++) {
                    const state = data[`relay${i}`];
                    const checkbox = document.getElementById(`relay${i}`);
                    
                    // Only update if state changed (to avoid disrupting user interaction)
                    if (checkbox.checked !== state) {
                        checkbox.checked = state;
                        this.updateRelayStatus(i, state);
                    }
                }
            } catch (error) {
                console.error('Error updating status:', error);
                document.getElementById('connection-status').textContent = '🔴 Disconnected';
            }
        }, 2000);
    }
}

// Initialize the home control system
document.addEventListener('DOMContentLoaded', () => {
    new HomeControl();
});
)rawliteral";
}
