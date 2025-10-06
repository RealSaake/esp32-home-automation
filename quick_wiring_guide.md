# ESP32 Home Automation - Quick Wiring Guide

## ⚠️ SAFETY FIRST
- **Turn off all power** before making connections
- **Double-check all connections** before powering on
- **Use proper wire gauges** for your current requirements
- **Test with multimeter** before final connections

## 🔌 Basic Connections

### ESP32 to Relay Module
```
ESP32 Pin    →    Relay Module
GPIO 2       →    IN1
GPIO 4       →    IN2  
GPIO 5       →    IN3
GPIO 18      →    IN4
5V           →    VCC
GND          →    GND
```

### ESP32 to Push Buttons
```
ESP32 Pin    →    Button Terminal 1
GPIO 19      →    Button 1
GPIO 21      →    Button 2
GPIO 22      →    Button 3
GPIO 23      →    Button 4
GND          →    All Buttons (Terminal 2)
```

### Power Connections
```
5V Power Supply  →  Relay Module VCC
5V Power Supply  →  ESP32 VIN
GND              →  Relay Module GND
GND              →  ESP32 GND
```

## 🏠 Device Connections (Examples)

### Light Bulb Connection
```
AC Power (Hot)  →  Relay NO (Normally Open)
Relay COM       →  Light Bulb Hot
Light Bulb      →  AC Power (Neutral)
```

### Fan Connection
```
AC Power (Hot)  →  Relay NO (Normally Open)
Relay COM       →  Fan Hot
Fan             →  AC Power (Neutral)
```

## 🔍 Testing Steps

### 1. Test ESP32
- Connect USB cable
- Open Serial Monitor (115200 baud)
- Should see "ESP32 Home Automation System Starting..."

### 2. Test WiFi Connection
- Look for "WiFi connected! IP address: xxx.xxx.xxx.xxx"
- Note the IP address for web access

### 3. Test Web Interface
- Open browser to ESP32 IP address
- Should see home automation dashboard

### 4. Test Relays
- Use web interface to toggle relays
- Listen for relay clicks
- Test with multimeter

### 5. Test Buttons
- Press physical buttons
- Should see status changes in web interface
- Check Serial Monitor for button presses

## 🛠️ Troubleshooting

### ESP32 Won't Start
- Check USB cable (use data cable, not charging cable)
- Try different USB port
- Check power supply (5V, 2A minimum)
- Press and hold BOOT button while uploading

### WiFi Connection Failed
- Check SSID and password in firebase_config.h
- Ensure 2.4GHz network (ESP32 doesn't support 5GHz)
- Check signal strength
- Try Access Point mode

### Relays Not Working
- Check wiring connections
- Verify power supply (5V, 2A minimum)
- Test with multimeter
- Check relay module (should be Active LOW)

### Web Interface Not Loading
- Check IP address in Serial Monitor
- Try different browser
- Clear browser cache
- Check firewall settings

### Buttons Not Responding
- Check button wiring
- Verify pull-up resistors
- Test with multimeter
- Check code pin assignments

## 📱 Access Methods

### Method 1: WiFi Mode
1. ESP32 connects to your WiFi
2. Find IP address in Serial Monitor
3. Open browser to that IP address

### Method 2: Access Point Mode
1. Connect to "ESP32-HomeControl" network
2. Password: "homecontrol123"
3. Open browser to 192.168.4.1

## 🔧 Advanced Testing

### Test Individual Components
1. **ESP32**: Upload blink example
2. **Relays**: Test each channel individually
3. **Buttons**: Test with simple button example
4. **WiFi**: Test with WiFi scan example
5. **Firebase**: Check database updates

### Test Complete System
1. **Web Control**: Toggle all relays via web interface
2. **Button Control**: Press all buttons, check responses
3. **Bulk Control**: Test "All ON" and "All OFF" functions
4. **Status Updates**: Verify real-time status updates
5. **Firebase Sync**: Check cloud synchronization

## 🎯 Success Indicators

### ✅ System Working Correctly
- ESP32 starts and connects to WiFi
- Web dashboard loads and shows device status
- Relays click when toggled
- Buttons respond and update status
- Firebase database updates in real-time
- Mobile interface works on phone/tablet

### ❌ Common Issues
- ESP32 won't start → Check power and USB
- No WiFi connection → Check credentials and signal
- Relays don't work → Check wiring and power
- Web interface won't load → Check IP address
- Buttons don't respond → Check wiring and code

## 🆘 Getting Help

### Check These First
1. **Serial Monitor** - Look for error messages
2. **WiFi Signal** - Ensure strong 2.4GHz signal
3. **Power Supply** - Must be 5V, 2A minimum
4. **Wiring** - Double-check all connections
5. **Code** - Verify Firebase credentials

### Debug Steps
1. **Test with simple examples** first
2. **Check each component** individually
3. **Verify all connections** with multimeter
4. **Test with different components** if available
5. **Ask for help** in Arduino/ESP32 forums

## 🎉 You're Done!

Once everything is working:
1. **Enclose the system** in a project box
2. **Mount relays** in electrical boxes
3. **Connect to actual devices** (lights, fans, etc.)
4. **Enjoy your smart home!**

Remember: **Safety first!** Always turn off power before making connections.
