/*
 * Firebase Configuration Header
 * Replace these values with your actual Firebase project credentials
 */

#ifndef FIREBASE_CONFIG_H
#define FIREBASE_CONFIG_H

// Firebase Project Configuration
// Get these from: Firebase Console > Project Settings > General > Your apps
#define FIREBASE_API_KEY "YOUR_FIREBASE_API_KEY_HERE"
#define FIREBASE_DATABASE_URL "https://your-project-id-default-rtdb.firebaseio.com/"

// Firebase Authentication
#define FIREBASE_USER_EMAIL "admin@home.local"
#define FIREBASE_USER_PASSWORD "admin123"

// WiFi Configuration
#define WIFI_SSID "YOUR_WIFI_NETWORK_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Access Point Configuration (fallback when WiFi fails)
#define AP_SSID "ESP32-HomeControl"
#define AP_PASSWORD "homecontrol123"

// Device Configuration
#define DEVICE_NAME "ESP32-HomeControl"
#define DEVICE_ID "esp32_001"

// Pin Definitions
#define RELAY_PIN_1 2
#define RELAY_PIN_2 4
#define RELAY_PIN_3 5
#define RELAY_PIN_4 18

#define BUTTON_PIN_1 19
#define BUTTON_PIN_2 21
#define BUTTON_PIN_3 22
#define BUTTON_PIN_4 23

// Timing Configuration
#define FIREBASE_UPDATE_INTERVAL 1000  // milliseconds
#define BUTTON_DEBOUNCE_DELAY 50       // milliseconds
#define STATUS_LED_BLINK_INTERVAL 1000 // milliseconds

// Debug Configuration
#define DEBUG_MODE true
#define SERIAL_BAUD_RATE 115200

#endif // FIREBASE_CONFIG_H
