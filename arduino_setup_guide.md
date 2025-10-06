# Arduino IDE Setup Guide for ESP32

## Step-by-Step Configuration

### 1. Open Arduino IDE
- Launch Arduino IDE 2.0
- You'll see the main interface

### 2. Add ESP32 Board Manager URL
1. Go to **File > Preferences**
2. In the "Additional Board Manager URLs" field, add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Click **OK**

### 3. Install ESP32 Board Package
1. Go to **Tools > Board > Boards Manager**
2. Search for "ESP32"
3. Find "esp32 by Espressif Systems"
4. Click **Install** (this will take a few minutes)
5. Wait for installation to complete
6. Click **Close**

### 4. Install Required Libraries
1. Go to **Tools > Manage Libraries**
2. Search for and install these libraries one by one:
   - **Firebase ESP Client** by Mobizt
   - **ArduinoJson** by Benoit Blanchon
   - **DHT sensor library** by Adafruit
   - **Adafruit Unified Sensor** by Adafruit

### 5. Select ESP32 Board
1. Go to **Tools > Board > ESP32 Arduino**
2. Select **ESP32 Dev Module**

### 6. Configure Board Settings
1. Go to **Tools** and set:
   - **Board**: ESP32 Dev Module
   - **Upload Speed**: 921600
   - **CPU Frequency**: 240MHz (WiFi/BT)
   - **Flash Frequency**: 80MHz
   - **Flash Mode**: QIO
   - **Flash Size**: 4MB (32Mb)
   - **Partition Scheme**: Default 4MB with spiffs
   - **Core Debug Level**: None
   - **PSRAM**: Disabled

### 7. Test Installation
1. Go to **File > Examples > WiFi > WiFiScan**
2. Click **Upload** (you'll need an ESP32 connected)
3. If it compiles without errors, you're ready!

## Troubleshooting

### If you get compilation errors:
- Make sure all libraries are installed
- Check that ESP32 board package is properly installed
- Try restarting Arduino IDE

### If upload fails:
- Check USB cable (use data cable, not charging cable)
- Try different USB port
- Press and hold BOOT button while uploading
- Check COM port in Tools > Port

## Next Steps
Once Arduino IDE is configured:
1. Connect your ESP32 via USB
2. Select the correct COM port in Tools > Port
3. You're ready to upload code!
