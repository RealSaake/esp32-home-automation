#!/usr/bin/env python3
"""
ESP32 Home Automation - Setup Script
Automated setup and configuration for the ESP32 home automation system
"""

import os
import sys
import subprocess
import json
import requests
import time
from pathlib import Path

class ESP32Setup:
    def __init__(self):
        self.project_root = Path(__file__).parent
        self.config = {}
        
    def log(self, message, level="INFO"):
        timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
        print(f"[{timestamp}] [{level}] {message}")
        
    def check_python_version(self):
        """Check if Python version is compatible"""
        if sys.version_info < (3, 7):
            self.log("Python 3.7 or higher is required", "ERROR")
            return False
        self.log(f"Python version: {sys.version}", "INFO")
        return True
        
    def install_python_dependencies(self):
        """Install Python dependencies"""
        self.log("Installing Python dependencies...")
        try:
            subprocess.run([sys.executable, "-m", "pip", "install", "-r", "requirements.txt"], 
                          check=True, capture_output=True, text=True)
            self.log("Python dependencies installed successfully", "SUCCESS")
            return True
        except subprocess.CalledProcessError as e:
            self.log(f"Failed to install Python dependencies: {e}", "ERROR")
            return False
            
    def check_arduino_ide(self):
        """Check if Arduino IDE is installed"""
        self.log("Checking for Arduino IDE...")
        
        # Common Arduino IDE paths
        arduino_paths = [
            "/Applications/Arduino.app/Contents/MacOS/Arduino",  # macOS
            "C:\\Program Files\\Arduino\\arduino.exe",         # Windows
            "C:\\Program Files (x86)\\Arduino\\arduino.exe",   # Windows 32-bit
            "/usr/bin/arduino",                                # Linux
            "/usr/local/bin/arduino"                           # Linux local
        ]
        
        for path in arduino_paths:
            if os.path.exists(path):
                self.log(f"Arduino IDE found at: {path}", "SUCCESS")
                return True
                
        self.log("Arduino IDE not found. Please install it from https://www.arduino.cc/en/software", "WARNING")
        return False
        
    def check_esp32_board_package(self):
        """Check if ESP32 board package is installed"""
        self.log("Checking ESP32 board package...")
        
        # This would require checking Arduino IDE configuration
        # For now, we'll provide instructions
        self.log("Please ensure ESP32 board package is installed in Arduino IDE:", "INFO")
        self.log("1. Open Arduino IDE", "INFO")
        self.log("2. Go to File > Preferences", "INFO")
        self.log("3. Add this URL to Additional Board Manager URLs:", "INFO")
        self.log("   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json", "INFO")
        self.log("4. Go to Tools > Board > Boards Manager", "INFO")
        self.log("5. Search for 'ESP32' and install 'esp32 by Espressif Systems'", "INFO")
        
        return True
        
    def check_firebase_config(self):
        """Check Firebase configuration"""
        self.log("Checking Firebase configuration...")
        
        config_file = self.project_root / "firebase_config.h"
        if not config_file.exists():
            self.log("firebase_config.h not found", "ERROR")
            return False
            
        with open(config_file, 'r') as f:
            content = f.read()
            
        if "YOUR_FIREBASE_API_KEY" in content:
            self.log("Firebase configuration needs to be updated", "WARNING")
            self.log("Please update firebase_config.h with your Firebase credentials", "INFO")
            return False
            
        self.log("Firebase configuration looks good", "SUCCESS")
        return True
        
    def test_esp32_connection(self, ip_address="192.168.1.100"):
        """Test connection to ESP32"""
        self.log(f"Testing connection to ESP32 at {ip_address}...")
        
        try:
            response = requests.get(f"http://{ip_address}/", timeout=5)
            if response.status_code == 200:
                self.log("ESP32 connection successful", "SUCCESS")
                return True
            else:
                self.log(f"ESP32 connection failed: HTTP {response.status_code}", "ERROR")
                return False
        except requests.exceptions.RequestException as e:
            self.log(f"ESP32 connection failed: {e}", "ERROR")
            return False
            
    def run_tests(self):
        """Run the test suite"""
        self.log("Running test suite...")
        
        try:
            result = subprocess.run([sys.executable, "test_suite.py"], 
                                  capture_output=True, text=True, timeout=60)
            
            if result.returncode == 0:
                self.log("All tests passed", "SUCCESS")
                return True
            else:
                self.log(f"Tests failed: {result.stderr}", "ERROR")
                return False
                
        except subprocess.TimeoutExpired:
            self.log("Test suite timed out", "ERROR")
            return False
        except Exception as e:
            self.log(f"Error running tests: {e}", "ERROR")
            return False
            
    def create_startup_script(self):
        """Create startup script for voice control"""
        self.log("Creating startup script...")
        
        startup_script = """#!/bin/bash
# ESP32 Home Automation - Voice Control Startup Script

echo "Starting ESP32 Home Automation Voice Control..."

# Check if Python is available
if ! command -v python3 &> /dev/null; then
    echo "Python 3 is required but not installed"
    exit 1
fi

# Check if required packages are installed
python3 -c "import speech_recognition, pyttsx3, requests" 2>/dev/null
if [ $? -ne 0 ]; then
    echo "Installing required packages..."
    pip3 install -r requirements.txt
fi

# Start voice control
echo "Starting voice control system..."
python3 voice_control.py --url http://192.168.1.100

echo "Voice control stopped"
"""
        
        script_path = self.project_root / "start_voice_control.sh"
        with open(script_path, 'w') as f:
            f.write(startup_script)
            
        # Make executable on Unix systems
        if os.name != 'nt':
            os.chmod(script_path, 0o755)
            
        self.log(f"Startup script created: {script_path}", "SUCCESS")
        return True
        
    def create_windows_batch(self):
        """Create Windows batch file for voice control"""
        self.log("Creating Windows batch file...")
        
        batch_content = """@echo off
REM ESP32 Home Automation - Voice Control Startup Script

echo Starting ESP32 Home Automation Voice Control...

REM Check if Python is available
python --version >nul 2>&1
if errorlevel 1 (
    echo Python is required but not installed
    pause
    exit /b 1
)

REM Check if required packages are installed
python -c "import speech_recognition, pyttsx3, requests" >nul 2>&1
if errorlevel 1 (
    echo Installing required packages...
    pip install -r requirements.txt
)

REM Start voice control
echo Starting voice control system...
python voice_control.py --url http://192.168.1.100

echo Voice control stopped
pause
"""
        
        batch_path = self.project_root / "start_voice_control.bat"
        with open(batch_path, 'w') as f:
            f.write(batch_content)
            
        self.log(f"Windows batch file created: {batch_path}", "SUCCESS")
        return True
        
    def generate_setup_report(self):
        """Generate setup report"""
        self.log("Generating setup report...")
        
        report = {
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
            "python_version": sys.version,
            "project_root": str(self.project_root),
            "files": {
                "arduino_code": "esp32_main.ino",
                "sensor_code": "sensors.ino",
                "web_dashboard": "web_dashboard.html",
                "mobile_app": "mobile_app.html",
                "voice_control": "voice_control.py",
                "test_suite": "test_suite.py",
                "wiring_guide": "wiring_diagrams.md",
                "requirements": "requirements.txt"
            },
            "next_steps": [
                "1. Install Arduino IDE and ESP32 board package",
                "2. Configure Firebase credentials in firebase_config.h",
                "3. Upload esp32_main.ino to your ESP32",
                "4. Connect hardware according to wiring_diagrams.md",
                "5. Test the system using test_suite.py",
                "6. Start voice control with start_voice_control.sh (Linux/Mac) or start_voice_control.bat (Windows)"
            ]
        }
        
        report_path = self.project_root / "setup_report.json"
        with open(report_path, 'w') as f:
            json.dump(report, f, indent=2)
            
        self.log(f"Setup report saved to: {report_path}", "SUCCESS")
        return True
        
    def run_full_setup(self):
        """Run complete setup process"""
        self.log("Starting ESP32 Home Automation Setup")
        self.log("=" * 50)
        
        # Check Python version
        if not self.check_python_version():
            return False
            
        # Install Python dependencies
        if not self.install_python_dependencies():
            return False
            
        # Check Arduino IDE
        self.check_arduino_ide()
        
        # Check ESP32 board package
        self.check_esp32_board_package()
        
        # Check Firebase configuration
        self.check_firebase_config()
        
        # Create startup scripts
        self.create_startup_script()
        self.create_windows_batch()
        
        # Generate setup report
        self.generate_setup_report()
        
        self.log("=" * 50)
        self.log("SETUP COMPLETE!")
        self.log("=" * 50)
        self.log("Next steps:")
        self.log("1. Install Arduino IDE and ESP32 board package")
        self.log("2. Configure Firebase credentials")
        self.log("3. Upload code to ESP32")
        self.log("4. Connect hardware")
        self.log("5. Test the system")
        self.log("6. Start voice control")
        
        return True

def main():
    setup = ESP32Setup()
    
    if len(sys.argv) > 1:
        command = sys.argv[1]
        
        if command == "test":
            setup.run_tests()
        elif command == "check":
            setup.check_python_version()
            setup.check_arduino_ide()
            setup.check_esp32_board_package()
            setup.check_firebase_config()
        elif command == "connect":
            ip = sys.argv[2] if len(sys.argv) > 2 else "192.168.1.100"
            setup.test_esp32_connection(ip)
        else:
            print("Usage: python setup.py [test|check|connect|ip_address]")
    else:
        setup.run_full_setup()

if __name__ == "__main__":
    main()
