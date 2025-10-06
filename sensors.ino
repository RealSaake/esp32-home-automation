/*
 * ESP32 Home Automation - Sensor Integration
 * Advanced sensor support for temperature, motion, light, and more
 * 
 * Sensors Supported:
 * - DHT22 (Temperature & Humidity)
 * - PIR Motion Sensor
 * - Light Sensor (LDR)
 * - Ultrasonic Distance Sensor
 * - Soil Moisture Sensor
 * - Gas/Smoke Sensor
 */

#include <WiFi.h>
#include <WebServer.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <DHT.h>
#include <ArduinoJson.h>

// Sensor pin definitions
#define DHT_PIN 25
#define PIR_PIN 26
#define LDR_PIN 27
#define ULTRASONIC_TRIG 32
#define ULTRASONIC_ECHO 33
#define MOISTURE_PIN 34
#define GAS_PIN 35

// Sensor types
#define DHT_TYPE DHT22

// Initialize sensors
DHT dht(DHT_PIN, DHT_TYPE);

// Sensor data structure
struct SensorData {
  float temperature;
  float humidity;
  bool motion;
  int lightLevel;
  float distance;
  int moisture;
  int gasLevel;
  unsigned long timestamp;
};

SensorData sensorData;

// Web server
WebServer server(80);

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Timing variables
unsigned long lastSensorRead = 0;
unsigned long lastFirebaseUpdate = 0;
const unsigned long sensorReadInterval = 2000;  // Read sensors every 2 seconds
const unsigned long firebaseUpdateInterval = 5000;  // Update Firebase every 5 seconds

// Motion detection variables
bool lastMotionState = false;
unsigned long motionStartTime = 0;
unsigned long motionDuration = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Home Automation - Sensor Integration Starting...");
  
  // Initialize sensors
  initializeSensors();
  
  // Connect to WiFi
  if (connectToWiFi()) {
    setupFirebase();
    setupWebServer();
    Serial.println("Connected to WiFi and Firebase!");
  } else {
    startAccessPoint();
    setupWebServer();
    Serial.println("Started Access Point mode");
  }
  
  Serial.println("Sensor system ready!");
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Read sensors periodically
  if (millis() - lastSensorRead > sensorReadInterval) {
    readAllSensors();
    lastSensorRead = millis();
  }
  
  // Update Firebase periodically
  if (millis() - lastFirebaseUpdate > firebaseUpdateInterval) {
    updateFirebase();
    lastFirebaseUpdate = millis();
  }
  
  delay(100);
}

void initializeSensors() {
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize PIR sensor
  pinMode(PIR_PIN, INPUT);
  
  // Initialize ultrasonic sensor
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);
  
  // Initialize analog sensors
  pinMode(LDR_PIN, INPUT);
  pinMode(MOISTURE_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  
  Serial.println("All sensors initialized");
}

void readAllSensors() {
  // Read temperature and humidity
  sensorData.temperature = dht.readTemperature();
  sensorData.humidity = dht.readHumidity();
  
  // Read motion sensor
  bool currentMotion = digitalRead(PIR_PIN) == HIGH;
  sensorData.motion = currentMotion;
  
  // Handle motion detection logic
  if (currentMotion && !lastMotionState) {
    motionStartTime = millis();
    Serial.println("Motion detected!");
  } else if (!currentMotion && lastMotionState) {
    motionDuration = millis() - motionStartTime;
    Serial.println("Motion ended. Duration: " + String(motionDuration) + "ms");
  }
  lastMotionState = currentMotion;
  
  // Read light sensor (LDR)
  int ldrValue = analogRead(LDR_PIN);
  sensorData.lightLevel = map(ldrValue, 0, 4095, 0, 100);  // Convert to percentage
  
  // Read ultrasonic distance sensor
  sensorData.distance = readUltrasonicDistance();
  
  // Read soil moisture sensor
  int moistureValue = analogRead(MOISTURE_PIN);
  sensorData.moisture = map(moistureValue, 0, 4095, 0, 100);  // Convert to percentage
  
  // Read gas/smoke sensor
  int gasValue = analogRead(GAS_PIN);
  sensorData.gasLevel = map(gasValue, 0, 4095, 0, 100);  // Convert to percentage
  
  // Update timestamp
  sensorData.timestamp = millis();
  
  // Print sensor data
  printSensorData();
}

float readUltrasonicDistance() {
  // Trigger ultrasonic sensor
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);
  
  // Read echo
  long duration = pulseIn(ULTRASONIC_ECHO, HIGH);
  
  // Calculate distance in cm
  float distance = duration * 0.034 / 2;
  
  // Filter out invalid readings
  if (distance > 400 || distance < 2) {
    return -1;  // Invalid reading
  }
  
  return distance;
}

void printSensorData() {
  Serial.println("=== SENSOR DATA ===");
  Serial.println("Temperature: " + String(sensorData.temperature) + "°C");
  Serial.println("Humidity: " + String(sensorData.humidity) + "%");
  Serial.println("Motion: " + String(sensorData.motion ? "YES" : "NO"));
  Serial.println("Light Level: " + String(sensorData.lightLevel) + "%");
  Serial.println("Distance: " + String(sensorData.distance) + "cm");
  Serial.println("Moisture: " + String(sensorData.moisture) + "%");
  Serial.println("Gas Level: " + String(sensorData.gasLevel) + "%");
  Serial.println("==================");
}

bool connectToWiFi() {
  WiFi.begin("YOUR_WIFI_SSID", "YOUR_WIFI_PASSWORD");
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
  WiFi.softAP("ESP32-Sensors", "sensors123");
  Serial.print("Access Point started: ESP32-Sensors");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}

void setupFirebase() {
  config.api_key = "YOUR_FIREBASE_API_KEY";
  config.database_url = "YOUR_FIREBASE_DATABASE_URL";
  
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase authentication successful");
  } else {
    Serial.printf("Firebase auth failed: %s\n", config.signer.signupError.message.c_str());
  }
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setupWebServer() {
  // Root page with sensor dashboard
  server.on("/", handleSensorDashboard);
  
  // API endpoints
  server.on("/api/sensors", handleSensorData);
  server.on("/api/sensors/temperature", handleTemperature);
  server.on("/api/sensors/humidity", handleHumidity);
  server.on("/api/sensors/motion", handleMotion);
  server.on("/api/sensors/light", handleLight);
  server.on("/api/sensors/distance", handleDistance);
  server.on("/api/sensors/moisture", handleMoisture);
  server.on("/api/sensors/gas", handleGas);
  
  server.begin();
  Serial.println("Web server started");
}

void handleSensorDashboard() {
  String html = getSensorDashboardHTML();
  server.send(200, "text/html", html);
}

void handleSensorData() {
  String json = "{";
  json += "\"temperature\":" + String(sensorData.temperature) + ",";
  json += "\"humidity\":" + String(sensorData.humidity) + ",";
  json += "\"motion\":" + String(sensorData.motion ? "true" : "false") + ",";
  json += "\"lightLevel\":" + String(sensorData.lightLevel) + ",";
  json += "\"distance\":" + String(sensorData.distance) + ",";
  json += "\"moisture\":" + String(sensorData.moisture) + ",";
  json += "\"gasLevel\":" + String(sensorData.gasLevel) + ",";
  json += "\"timestamp\":" + String(sensorData.timestamp);
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleTemperature() {
  server.send(200, "application/json", "{\"temperature\":" + String(sensorData.temperature) + "}");
}

void handleHumidity() {
  server.send(200, "application/json", "{\"humidity\":" + String(sensorData.humidity) + "}");
}

void handleMotion() {
  server.send(200, "application/json", "{\"motion\":" + String(sensorData.motion ? "true" : "false") + "}");
}

void handleLight() {
  server.send(200, "application/json", "{\"lightLevel\":" + String(sensorData.lightLevel) + "}");
}

void handleDistance() {
  server.send(200, "application/json", "{\"distance\":" + String(sensorData.distance) + "}");
}

void handleMoisture() {
  server.send(200, "application/json", "{\"moisture\":" + String(sensorData.moisture) + "}");
}

void handleGas() {
  server.send(200, "application/json", "{\"gasLevel\":" + String(sensorData.gasLevel) + "}");
}

void updateFirebase() {
  if (Firebase.ready()) {
    FirebaseJson json;
    json.set("temperature", sensorData.temperature);
    json.set("humidity", sensorData.humidity);
    json.set("motion", sensorData.motion);
    json.set("lightLevel", sensorData.lightLevel);
    json.set("distance", sensorData.distance);
    json.set("moisture", sensorData.moisture);
    json.set("gasLevel", sensorData.gasLevel);
    json.set("timestamp", sensorData.timestamp);
    
    if (Firebase.RTDB.setJSON(&fbdo, "/sensors/esp32", &json)) {
      // Success
    } else {
      Serial.println("Firebase update failed: " + fbdo.errorReason());
    }
  }
}

String getSensorDashboardHTML() {
  return R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Sensor Dashboard</title>
    <style>
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
        
        .header {
            text-align: center;
            margin-bottom: 30px;
            background: rgba(255, 255, 255, 0.1);
            padding: 20px;
            border-radius: 15px;
            backdrop-filter: blur(10px);
        }
        
        .header h1 {
            color: white;
            font-size: 2.5em;
            margin-bottom: 10px;
        }
        
        .sensor-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        
        .sensor-card {
            background: rgba(255, 255, 255, 0.95);
            padding: 25px;
            border-radius: 15px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
            text-align: center;
            transition: transform 0.3s ease;
        }
        
        .sensor-card:hover {
            transform: translateY(-5px);
        }
        
        .sensor-card h3 {
            margin-bottom: 15px;
            color: #333;
            font-size: 1.3em;
        }
        
        .sensor-value {
            font-size: 2em;
            font-weight: bold;
            margin-bottom: 10px;
        }
        
        .temperature { color: #e74c3c; }
        .humidity { color: #3498db; }
        .motion { color: #f39c12; }
        .light { color: #f1c40f; }
        .distance { color: #9b59b6; }
        .moisture { color: #2ecc71; }
        .gas { color: #e67e22; }
        
        .status-indicator {
            display: inline-block;
            width: 12px;
            height: 12px;
            border-radius: 50%;
            margin-right: 8px;
        }
        
        .status-on { background: #2ecc71; }
        .status-off { background: #e74c3c; }
        
        .footer {
            text-align: center;
            color: white;
            margin-top: 30px;
            opacity: 0.8;
        }
        
        @media (max-width: 768px) {
            .sensor-grid {
                grid-template-columns: 1fr;
            }
            
            .header h1 {
                font-size: 2em;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <header class="header">
            <h1>🌡️ Sensor Dashboard</h1>
            <p>Real-time environmental monitoring</p>
        </header>
        
        <div class="sensor-grid">
            <div class="sensor-card">
                <h3>🌡️ Temperature</h3>
                <div class="sensor-value temperature" id="temperature">--°C</div>
                <p>Current temperature</p>
            </div>
            
            <div class="sensor-card">
                <h3>💧 Humidity</h3>
                <div class="sensor-value humidity" id="humidity">--%</div>
                <p>Air humidity level</p>
            </div>
            
            <div class="sensor-card">
                <h3>🏃 Motion</h3>
                <div class="sensor-value motion" id="motion">
                    <span class="status-indicator status-off"></span>No Motion
                </div>
                <p>Motion detection</p>
            </div>
            
            <div class="sensor-card">
                <h3>💡 Light</h3>
                <div class="sensor-value light" id="light">--%</div>
                <p>Light intensity</p>
            </div>
            
            <div class="sensor-card">
                <h3>📏 Distance</h3>
                <div class="sensor-value distance" id="distance">--cm</div>
                <p>Ultrasonic distance</p>
            </div>
            
            <div class="sensor-card">
                <h3>🌱 Moisture</h3>
                <div class="sensor-value moisture" id="moisture">--%</div>
                <p>Soil moisture level</p>
            </div>
            
            <div class="sensor-card">
                <h3>⚠️ Gas</h3>
                <div class="sensor-value gas" id="gas">--%</div>
                <p>Gas/smoke detection</p>
            </div>
        </div>
        
        <footer class="footer">
            <p>ESP32 Home Automation - Sensor Integration</p>
            <p>Real-time environmental monitoring system</p>
        </footer>
    </div>
    
    <script>
        class SensorDashboard {
            constructor() {
                this.init();
            }
            
            init() {
                this.startUpdates();
            }
            
            startUpdates() {
                setInterval(() => this.updateSensors(), 2000);
                this.updateSensors(); // Initial update
            }
            
            async updateSensors() {
                try {
                    const response = await fetch('/api/sensors');
                    const data = await response.json();
                    
                    document.getElementById('temperature').textContent = data.temperature.toFixed(1) + '°C';
                    document.getElementById('humidity').textContent = data.humidity.toFixed(1) + '%';
                    document.getElementById('light').textContent = data.lightLevel + '%';
                    document.getElementById('distance').textContent = data.distance.toFixed(1) + 'cm';
                    document.getElementById('moisture').textContent = data.moisture + '%';
                    document.getElementById('gas').textContent = data.gasLevel + '%';
                    
                    const motionElement = document.getElementById('motion');
                    if (data.motion) {
                        motionElement.innerHTML = '<span class="status-indicator status-on"></span>Motion Detected';
                    } else {
                        motionElement.innerHTML = '<span class="status-indicator status-off"></span>No Motion';
                    }
                } catch (error) {
                    console.error('Error updating sensors:', error);
                }
            }
        }
        
        // Initialize dashboard
        document.addEventListener('DOMContentLoaded', () => {
            new SensorDashboard();
        });
    </script>
</body>
</html>
)rawliteral";
}
