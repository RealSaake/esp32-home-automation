#!/usr/bin/env python3
"""
ESP32 Home Automation Test Suite
Automated testing and validation for the ESP32 home automation system
"""

import requests
import time
import json
import sys
import argparse
from datetime import datetime
import threading
import queue

class ESP32Tester:
    def __init__(self, base_url="http://192.168.1.100", timeout=5):
        self.base_url = base_url.rstrip('/')
        self.timeout = timeout
        self.test_results = []
        self.test_queue = queue.Queue()
        
    def log(self, message, level="INFO"):
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        print(f"[{timestamp}] [{level}] {message}")
        
    def add_test_result(self, test_name, passed, message=""):
        result = {
            'test_name': test_name,
            'passed': passed,
            'message': message,
            'timestamp': datetime.now().isoformat()
        }
        self.test_results.append(result)
        
        status = "✅ PASS" if passed else "❌ FAIL"
        self.log(f"{status} - {test_name}: {message}")
        
    def test_connection(self):
        """Test basic connectivity to ESP32"""
        try:
            response = requests.get(f"{self.base_url}/", timeout=self.timeout)
            if response.status_code == 200:
                self.add_test_result("Connection Test", True, "ESP32 is reachable")
                return True
            else:
                self.add_test_result("Connection Test", False, f"HTTP {response.status_code}")
                return False
        except requests.exceptions.RequestException as e:
            self.add_test_result("Connection Test", False, f"Connection failed: {str(e)}")
            return False
            
    def test_api_status(self):
        """Test API status endpoint"""
        try:
            response = requests.get(f"{self.base_url}/api/status", timeout=self.timeout)
            if response.status_code == 200:
                data = response.json()
                if all(key in data for key in ['relay1', 'relay2', 'relay3', 'relay4']):
                    self.add_test_result("API Status Test", True, "Status endpoint working correctly")
                    return True
                else:
                    self.add_test_result("API Status Test", False, "Invalid status data format")
                    return False
            else:
                self.add_test_result("API Status Test", False, f"HTTP {response.status_code}")
                return False
        except requests.exceptions.RequestException as e:
            self.add_test_result("API Status Test", False, f"Request failed: {str(e)}")
            return False
        except json.JSONDecodeError:
            self.add_test_result("API Status Test", False, "Invalid JSON response")
            return False
            
    def test_relay_control(self, relay_num, state):
        """Test individual relay control"""
        try:
            url = f"{self.base_url}/api/relay?relay={relay_num}&state={state}"
            response = requests.get(url, timeout=self.timeout)
            
            if response.status_code == 200:
                data = response.json()
                if data.get('success'):
                    self.add_test_result(f"Relay {relay_num} Control", True, f"Relay {relay_num} set to {state}")
                    return True
                else:
                    self.add_test_result(f"Relay {relay_num} Control", False, f"Control failed: {data.get('error', 'Unknown error')}")
                    return False
            else:
                self.add_test_result(f"Relay {relay_num} Control", False, f"HTTP {response.status_code}")
                return False
        except requests.exceptions.RequestException as e:
            self.add_test_result(f"Relay {relay_num} Control", False, f"Request failed: {str(e)}")
            return False
            
    def test_all_relays_control(self, state):
        """Test bulk relay control"""
        try:
            url = f"{self.base_url}/api/all?state={state}"
            response = requests.get(url, timeout=self.timeout)
            
            if response.status_code == 200:
                data = response.json()
                if data.get('success'):
                    self.add_test_result("Bulk Relay Control", True, f"All relays set to {state}")
                    return True
                else:
                    self.add_test_result("Bulk Relay Control", False, f"Bulk control failed: {data.get('error', 'Unknown error')}")
                    return False
            else:
                self.add_test_result("Bulk Relay Control", False, f"HTTP {response.status_code}")
                return False
        except requests.exceptions.RequestException as e:
            self.add_test_result("Bulk Relay Control", False, f"Request failed: {str(e)}")
            return False
            
    def test_relay_sequence(self):
        """Test a sequence of relay operations"""
        self.log("Starting relay sequence test...")
        
        # Test individual relays
        for relay in range(1, 5):
            # Turn on
            self.test_relay_control(relay, True)
            time.sleep(0.5)
            
            # Turn off
            self.test_relay_control(relay, False)
            time.sleep(0.5)
            
    def test_bulk_operations(self):
        """Test bulk operations"""
        self.log("Testing bulk operations...")
        
        # Turn all on
        self.test_all_relays_control(True)
        time.sleep(1)
        
        # Turn all off
        self.test_all_relays_control(False)
        time.sleep(1)
        
    def test_response_times(self):
        """Test API response times"""
        self.log("Testing response times...")
        
        start_time = time.time()
        response = requests.get(f"{self.base_url}/api/status", timeout=self.timeout)
        end_time = time.time()
        
        response_time = (end_time - start_time) * 1000  # Convert to milliseconds
        
        if response_time < 1000:  # Less than 1 second
            self.add_test_result("Response Time Test", True, f"Response time: {response_time:.2f}ms")
        else:
            self.add_test_result("Response Time Test", False, f"Response time too slow: {response_time:.2f}ms")
            
    def test_concurrent_requests(self):
        """Test concurrent API requests"""
        self.log("Testing concurrent requests...")
        
        def make_request():
            try:
                response = requests.get(f"{self.base_url}/api/status", timeout=self.timeout)
                return response.status_code == 200
            except:
                return False
                
        # Create multiple threads
        threads = []
        results = []
        
        for i in range(5):
            thread = threading.Thread(target=lambda: results.append(make_request()))
            threads.append(thread)
            thread.start()
            
        # Wait for all threads to complete
        for thread in threads:
            thread.join()
            
        success_count = sum(results)
        if success_count >= 4:  # At least 4 out of 5 should succeed
            self.add_test_result("Concurrent Requests Test", True, f"{success_count}/5 requests successful")
        else:
            self.add_test_result("Concurrent Requests Test", False, f"Only {success_count}/5 requests successful")
            
    def test_error_handling(self):
        """Test error handling with invalid requests"""
        self.log("Testing error handling...")
        
        # Test invalid relay number
        try:
            response = requests.get(f"{self.base_url}/api/relay?relay=99&state=true", timeout=self.timeout)
            if response.status_code == 400:
                self.add_test_result("Error Handling Test", True, "Invalid relay number handled correctly")
            else:
                self.add_test_result("Error Handling Test", False, f"Expected 400, got {response.status_code}")
        except requests.exceptions.RequestException as e:
            self.add_test_result("Error Handling Test", False, f"Request failed: {str(e)}")
            
    def test_web_dashboard(self):
        """Test web dashboard accessibility"""
        self.log("Testing web dashboard...")
        
        try:
            response = requests.get(f"{self.base_url}/", timeout=self.timeout)
            if response.status_code == 200:
                content = response.text
                if "Home Automation" in content and "relay" in content.lower():
                    self.add_test_result("Web Dashboard Test", True, "Dashboard loaded correctly")
                else:
                    self.add_test_result("Web Dashboard Test", False, "Dashboard content missing")
            else:
                self.add_test_result("Web Dashboard Test", False, f"HTTP {response.status_code}")
        except requests.exceptions.RequestException as e:
            self.add_test_result("Web Dashboard Test", False, f"Request failed: {str(e)}")
            
    def test_css_js_assets(self):
        """Test CSS and JS asset loading"""
        self.log("Testing CSS and JS assets...")
        
        assets = ['/style.css', '/script.js']
        for asset in assets:
            try:
                response = requests.get(f"{self.base_url}{asset}", timeout=self.timeout)
                if response.status_code == 200:
                    self.add_test_result(f"Asset Test - {asset}", True, f"{asset} loaded successfully")
                else:
                    self.add_test_result(f"Asset Test - {asset}", False, f"HTTP {response.status_code}")
            except requests.exceptions.RequestException as e:
                self.add_test_result(f"Asset Test - {asset}", False, f"Request failed: {str(e)}")
                
    def run_all_tests(self):
        """Run all tests in sequence"""
        self.log("Starting ESP32 Home Automation Test Suite")
        self.log("=" * 50)
        
        # Basic connectivity tests
        if not self.test_connection():
            self.log("Cannot connect to ESP32. Please check IP address and network connection.")
            return False
            
        # API tests
        self.test_api_status()
        self.test_response_times()
        self.test_concurrent_requests()
        self.test_error_handling()
        
        # Web interface tests
        self.test_web_dashboard()
        self.test_css_js_assets()
        
        # Relay control tests
        self.test_relay_sequence()
        self.test_bulk_operations()
        
        # Generate report
        self.generate_report()
        
    def generate_report(self):
        """Generate test report"""
        self.log("=" * 50)
        self.log("TEST REPORT")
        self.log("=" * 50)
        
        total_tests = len(self.test_results)
        passed_tests = sum(1 for result in self.test_results if result['passed'])
        failed_tests = total_tests - passed_tests
        
        self.log(f"Total Tests: {total_tests}")
        self.log(f"Passed: {passed_tests}")
        self.log(f"Failed: {failed_tests}")
        self.log(f"Success Rate: {(passed_tests/total_tests)*100:.1f}%")
        
        if failed_tests > 0:
            self.log("\nFAILED TESTS:")
            for result in self.test_results:
                if not result['passed']:
                    self.log(f"  - {result['test_name']}: {result['message']}")
                    
        # Save detailed report
        report_data = {
            'timestamp': datetime.now().isoformat(),
            'total_tests': total_tests,
            'passed_tests': passed_tests,
            'failed_tests': failed_tests,
            'success_rate': (passed_tests/total_tests)*100,
            'results': self.test_results
        }
        
        with open('test_report.json', 'w') as f:
            json.dump(report_data, f, indent=2)
            
        self.log(f"\nDetailed report saved to: test_report.json")
        
        return failed_tests == 0

def main():
    parser = argparse.ArgumentParser(description='ESP32 Home Automation Test Suite')
    parser.add_argument('--url', default='http://192.168.1.100', help='ESP32 IP address (default: http://192.168.1.100)')
    parser.add_argument('--timeout', type=int, default=5, help='Request timeout in seconds (default: 5)')
    parser.add_argument('--test', choices=['connection', 'api', 'relays', 'web', 'all'], default='all', help='Specific test to run')
    
    args = parser.parse_args()
    
    tester = ESP32Tester(args.url, args.timeout)
    
    if args.test == 'connection':
        success = tester.test_connection()
    elif args.test == 'api':
        tester.test_connection()
        tester.test_api_status()
        tester.test_response_times()
        success = True
    elif args.test == 'relays':
        tester.test_connection()
        tester.test_relay_sequence()
        tester.test_bulk_operations()
        success = True
    elif args.test == 'web':
        tester.test_connection()
        tester.test_web_dashboard()
        tester.test_css_js_assets()
        success = True
    else:  # all
        success = tester.run_all_tests()
        
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()
