#!/usr/bin/env python3
"""
ESP32 Home Automation - Voice Control Integration
Voice control system using speech recognition and text-to-speech
"""

import speech_recognition as sr
import pyttsx3
import requests
import time
import threading
import queue
import json
from datetime import datetime
import argparse

class VoiceController:
    def __init__(self, esp32_url="http://192.168.1.100"):
        self.esp32_url = esp32_url.rstrip('/')
        self.recognizer = sr.Recognizer()
        self.microphone = sr.Microphone()
        self.tts_engine = pyttsx3.init()
        self.command_queue = queue.Queue()
        self.is_listening = False
        self.is_running = False
        
        # Voice commands mapping
        self.commands = {
            # Relay control commands
            "turn on light": {"relay": 1, "state": True},
            "turn off light": {"relay": 1, "state": False},
            "turn on fan": {"relay": 2, "state": True},
            "turn off fan": {"relay": 2, "state": False},
            "turn on tv": {"relay": 3, "state": True},
            "turn off tv": {"relay": 3, "state": False},
            "turn on garage": {"relay": 4, "state": True},
            "turn off garage": {"relay": 4, "state": False},
            
            # Bulk control commands
            "turn on all": {"action": "all", "state": True},
            "turn off all": {"action": "all", "state": False},
            "turn on everything": {"action": "all", "state": True},
            "turn off everything": {"action": "all", "state": False},
            
            # Status commands
            "what's the status": {"action": "status"},
            "show me the status": {"action": "status"},
            "check status": {"action": "status"},
            
            # Help commands
            "help": {"action": "help"},
            "what can you do": {"action": "help"},
            "list commands": {"action": "help"},
        }
        
        # Initialize TTS
        self.setup_tts()
        
        # Calibrate microphone
        self.calibrate_microphone()
        
    def setup_tts(self):
        """Setup text-to-speech engine"""
        voices = self.tts_engine.getProperty('voices')
        if voices:
            # Try to find a female voice
            for voice in voices:
                if 'female' in voice.name.lower() or 'zira' in voice.name.lower():
                    self.tts_engine.setProperty('voice', voice.id)
                    break
        
        # Set speech rate and volume
        self.tts_engine.setProperty('rate', 150)
        self.tts_engine.setProperty('volume', 0.8)
        
    def calibrate_microphone(self):
        """Calibrate microphone for ambient noise"""
        print("Calibrating microphone for ambient noise...")
        with self.microphone as source:
            self.recognizer.adjust_for_ambient_noise(source, duration=2)
        print("Microphone calibrated!")
        
    def speak(self, text):
        """Convert text to speech"""
        print(f"Speaking: {text}")
        self.tts_engine.say(text)
        self.tts_engine.runAndWait()
        
    def listen(self):
        """Listen for voice commands"""
        try:
            with self.microphone as source:
                print("Listening...")
                audio = self.recognizer.listen(source, timeout=5, phrase_time_limit=5)
                
            print("Processing speech...")
            text = self.recognizer.recognize_google(audio).lower()
            print(f"Heard: {text}")
            return text
            
        except sr.WaitTimeoutError:
            print("No speech detected")
            return None
        except sr.UnknownValueError:
            print("Could not understand speech")
            return None
        except sr.RequestError as e:
            print(f"Speech recognition error: {e}")
            return None
            
    def process_command(self, text):
        """Process voice command and execute action"""
        if not text:
            return False
            
        # Find matching command
        for command, action in self.commands.items():
            if command in text:
                return self.execute_command(action)
                
        # If no exact match, try fuzzy matching
        return self.fuzzy_match_command(text)
        
    def fuzzy_match_command(self, text):
        """Fuzzy matching for voice commands"""
        # Extract key words
        words = text.split()
        
        # Check for relay numbers
        relay_num = None
        for word in words:
            if word.isdigit() and 1 <= int(word) <= 4:
                relay_num = int(word)
                break
                
        # Check for on/off states
        state = None
        if any(word in text for word in ['on', 'enable', 'activate', 'start']):
            state = True
        elif any(word in text for word in ['off', 'disable', 'deactivate', 'stop']):
            state = False
            
        # Check for bulk commands
        if any(word in text for word in ['all', 'everything', 'all devices']):
            if state is not None:
                return self.execute_command({"action": "all", "state": state})
                
        # Check for individual relay commands
        if relay_num and state is not None:
            return self.execute_command({"relay": relay_num, "state": state})
            
        # Check for status requests
        if any(word in text for word in ['status', 'state', 'check', 'show']):
            return self.execute_command({"action": "status"})
            
        return False
        
    def execute_command(self, action):
        """Execute the voice command"""
        try:
            if "relay" in action:
                # Control individual relay
                relay = action["relay"]
                state = action["state"]
                return self.control_relay(relay, state)
                
            elif action.get("action") == "all":
                # Control all relays
                state = action["state"]
                return self.control_all_relays(state)
                
            elif action.get("action") == "status":
                # Get status
                return self.get_status()
                
            elif action.get("action") == "help":
                # Show help
                return self.show_help()
                
        except Exception as e:
            print(f"Error executing command: {e}")
            self.speak("Sorry, I couldn't execute that command")
            return False
            
        return False
        
    def control_relay(self, relay, state):
        """Control individual relay"""
        try:
            url = f"{self.esp32_url}/api/relay?relay={relay}&state={state}"
            response = requests.get(url, timeout=5)
            
            if response.status_code == 200:
                data = response.json()
                if data.get('success'):
                    action = "turned on" if state else "turned off"
                    self.speak(f"Device {relay} has been {action}")
                    return True
                else:
                    self.speak(f"Sorry, I couldn't control device {relay}")
                    return False
            else:
                self.speak("Sorry, there was a connection error")
                return False
                
        except requests.exceptions.RequestException:
            self.speak("Sorry, I couldn't connect to the device")
            return False
            
    def control_all_relays(self, state):
        """Control all relays"""
        try:
            url = f"{self.esp32_url}/api/all?state={state}"
            response = requests.get(url, timeout=5)
            
            if response.status_code == 200:
                data = response.json()
                if data.get('success'):
                    action = "turned on" if state else "turned off"
                    self.speak(f"All devices have been {action}")
                    return True
                else:
                    self.speak("Sorry, I couldn't control all devices")
                    return False
            else:
                self.speak("Sorry, there was a connection error")
                return False
                
        except requests.exceptions.RequestException:
            self.speak("Sorry, I couldn't connect to the device")
            return False
            
    def get_status(self):
        """Get device status"""
        try:
            response = requests.get(f"{self.esp32_url}/api/status", timeout=5)
            
            if response.status_code == 200:
                data = response.json()
                
                # Count active devices
                active_count = sum(1 for key, value in data.items() if key.startswith('relay') and value)
                total_count = len([key for key in data.keys() if key.startswith('relay')])
                
                if active_count == 0:
                    self.speak("All devices are currently off")
                elif active_count == total_count:
                    self.speak("All devices are currently on")
                else:
                    self.speak(f"{active_count} out of {total_count} devices are currently on")
                    
                return True
            else:
                self.speak("Sorry, I couldn't get the device status")
                return False
                
        except requests.exceptions.RequestException:
            self.speak("Sorry, I couldn't connect to the device")
            return False
            
    def show_help(self):
        """Show available commands"""
        help_text = """
        I can help you control your home automation system. Here are some commands:
        
        To control individual devices:
        - Turn on light, turn off light
        - Turn on fan, turn off fan
        - Turn on TV, turn off TV
        - Turn on garage, turn off garage
        
        To control all devices:
        - Turn on all, turn off all
        - Turn on everything, turn off everything
        
        To check status:
        - What's the status
        - Show me the status
        - Check status
        
        You can also use device numbers:
        - Turn on device 1, turn off device 2, etc.
        """
        
        print(help_text)
        self.speak("I can help you control your home automation system. You can say things like turn on light, turn off fan, or turn on all devices")
        return True
        
    def start_listening(self):
        """Start continuous voice listening"""
        self.is_running = True
        self.is_listening = True
        
        print("Voice control started! Say 'help' for available commands.")
        self.speak("Voice control activated. How can I help you?")
        
        while self.is_running:
            try:
                # Listen for command
                text = self.listen()
                
                if text:
                    # Process command
                    if self.process_command(text):
                        print("Command executed successfully")
                    else:
                        print("Command not recognized")
                        self.speak("Sorry, I didn't understand that command")
                        
                # Small delay before next listen
                time.sleep(0.5)
                
            except KeyboardInterrupt:
                print("\nStopping voice control...")
                break
            except Exception as e:
                print(f"Error in voice control loop: {e}")
                time.sleep(1)
                
        self.is_listening = False
        self.speak("Voice control deactivated")
        
    def stop_listening(self):
        """Stop voice listening"""
        self.is_running = False
        self.is_listening = False
        
    def test_connection(self):
        """Test connection to ESP32"""
        try:
            response = requests.get(f"{self.esp32_url}/", timeout=5)
            if response.status_code == 200:
                print("✅ ESP32 connection successful")
                return True
            else:
                print(f"❌ ESP32 connection failed: HTTP {response.status_code}")
                return False
        except requests.exceptions.RequestException as e:
            print(f"❌ ESP32 connection failed: {e}")
            return False

def main():
    parser = argparse.ArgumentParser(description='ESP32 Home Automation Voice Control')
    parser.add_argument('--url', default='http://192.168.1.100', help='ESP32 IP address')
    parser.add_argument('--test', action='store_true', help='Test connection only')
    parser.add_argument('--help-commands', action='store_true', help='Show available commands')
    
    args = parser.parse_args()
    
    # Create voice controller
    controller = VoiceController(args.url)
    
    if args.test:
        # Test connection
        if controller.test_connection():
            print("Voice control system ready!")
        else:
            print("Cannot connect to ESP32. Please check the IP address.")
        return
        
    if args.help_commands:
        controller.show_help()
        return
        
    # Test connection first
    if not controller.test_connection():
        print("Cannot connect to ESP32. Please check the IP address and network connection.")
        return
        
    try:
        # Start voice control
        controller.start_listening()
    except KeyboardInterrupt:
        print("\nVoice control stopped by user")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        controller.stop_listening()

if __name__ == "__main__":
    main()
