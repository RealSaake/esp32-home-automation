/**
 * ESP32 Home Automation Dashboard JavaScript
 * Advanced control system with real-time updates and error handling
 */

class HomeAutomationDashboard {
    constructor() {
        this.relays = {};
        this.isConnected = false;
        this.updateInterval = null;
        this.retryCount = 0;
        this.maxRetries = 5;
        this.retryDelay = 2000;
        
        this.init();
    }
    
    init() {
        this.hideLoadingOverlay();
        this.setupEventListeners();
        this.startStatusUpdates();
        this.loadInitialState();
        this.setupServiceWorker();
    }
    
    setupEventListeners() {
        // Individual relay controls
        for (let i = 1; i <= 4; i++) {
            const checkbox = document.getElementById(`relay${i}`);
            if (checkbox) {
                checkbox.addEventListener('change', (e) => this.toggleRelay(i, e.target.checked));
            }
        }
        
        // Bulk controls
        const allOnBtn = document.getElementById('all-on');
        const allOffBtn = document.getElementById('all-off');
        const refreshBtn = document.getElementById('refresh-status');
        
        if (allOnBtn) allOnBtn.addEventListener('click', () => this.allRelays(true));
        if (allOffBtn) allOffBtn.addEventListener('click', () => this.allRelays(false));
        if (refreshBtn) refreshBtn.addEventListener('click', () => this.refreshStatus());
        
        // Keyboard shortcuts
        document.addEventListener('keydown', (e) => this.handleKeyboardShortcuts(e));
        
        // Connection status monitoring
        window.addEventListener('online', () => this.handleConnectionChange(true));
        window.addEventListener('offline', () => this.handleConnectionChange(false));
    }
    
    async toggleRelay(relayNumber, state) {
        const checkbox = document.getElementById(`relay${relayNumber}`);
        const originalState = !state;
        
        try {
            this.showToast(`Controlling device ${relayNumber}...`, 'info');
            
            const response = await this.makeRequest(`/api/relay?relay=${relayNumber}&state=${state}`, {
                method: 'GET',
                timeout: 5000
            });
            
            if (response.ok) {
                const data = await response.json();
                if (data.success) {
                    this.updateRelayStatus(relayNumber, state);
                    this.showToast(`Device ${relayNumber} turned ${state ? 'ON' : 'OFF'}`, 'success');
                } else {
                    throw new Error(data.error || 'Control failed');
                }
            } else {
                throw new Error(`HTTP ${response.status}`);
            }
        } catch (error) {
            // Revert checkbox on error
            checkbox.checked = originalState;
            this.showToast(`Failed to control device ${relayNumber}: ${error.message}`, 'error');
            console.error('Error controlling relay:', error);
        }
    }
    
    async allRelays(state) {
        const action = state ? 'ON' : 'OFF';
        this.showToast(`Turning all devices ${action}...`, 'info');
        
        try {
            const response = await this.makeRequest(`/api/all?state=${state}`, {
                method: 'GET',
                timeout: 10000
            });
            
            if (response.ok) {
                const data = await response.json();
                if (data.success) {
                    for (let i = 1; i <= 4; i++) {
                        const checkbox = document.getElementById(`relay${i}`);
                        if (checkbox) {
                            checkbox.checked = state;
                            this.updateRelayStatus(i, state);
                        }
                    }
                    this.showToast(`All devices turned ${action}`, 'success');
                } else {
                    throw new Error(data.error || 'Bulk control failed');
                }
            } else {
                throw new Error(`HTTP ${response.status}`);
            }
        } catch (error) {
            this.showToast(`Failed to control all devices: ${error.message}`, 'error');
            console.error('Error controlling all relays:', error);
        }
    }
    
    updateRelayStatus(relayNumber, state) {
        const statusElement = document.getElementById(`status${relayNumber}`);
        const indicatorElement = document.getElementById(`indicator${relayNumber}`);
        const lastUpdateElement = document.getElementById(`lastUpdate${relayNumber}`);
        
        if (statusElement) {
            statusElement.textContent = state ? 'ON' : 'OFF';
            statusElement.className = `status ${state ? 'on' : 'off'}`;
        }
        
        if (indicatorElement) {
            indicatorElement.className = `relay-indicator ${state ? 'active' : ''}`;
        }
        
        if (lastUpdateElement) {
            lastUpdateElement.textContent = new Date().toLocaleTimeString();
        }
        
        this.relays[`relay${relayNumber}`] = state;
    }
    
    async loadInitialState() {
        try {
            const response = await this.makeRequest('/api/status', {
                method: 'GET',
                timeout: 5000
            });
            
            if (response.ok) {
                const data = await response.json();
                
                for (let i = 1; i <= 4; i++) {
                    const state = data[`relay${i}`];
                    const checkbox = document.getElementById(`relay${i}`);
                    
                    if (checkbox && typeof state === 'boolean') {
                        checkbox.checked = state;
                        this.updateRelayStatus(i, state);
                    }
                }
                
                this.isConnected = true;
                this.updateConnectionStatus(true);
                this.retryCount = 0;
            } else {
                throw new Error(`HTTP ${response.status}`);
            }
        } catch (error) {
            console.error('Error loading initial state:', error);
            this.handleConnectionError();
        }
    }
    
    startStatusUpdates() {
        this.updateInterval = setInterval(async () => {
            try {
                const response = await this.makeRequest('/api/status', {
                    method: 'GET',
                    timeout: 3000
                });
                
                if (response.ok) {
                    const data = await response.json();
                    
                    for (let i = 1; i <= 4; i++) {
                        const state = data[`relay${i}`];
                        const checkbox = document.getElementById(`relay${i}`);
                        
                        if (checkbox && typeof state === 'boolean') {
                            // Only update if state changed (to avoid disrupting user interaction)
                            if (checkbox.checked !== state) {
                                checkbox.checked = state;
                                this.updateRelayStatus(i, state);
                            }
                        }
                    }
                    
                    this.isConnected = true;
                    this.updateConnectionStatus(true);
                    this.retryCount = 0;
                } else {
                    throw new Error(`HTTP ${response.status}`);
                }
            } catch (error) {
                console.error('Error updating status:', error);
                this.handleConnectionError();
            }
        }, 2000);
    }
    
    async refreshStatus() {
        this.showToast('Refreshing status...', 'info');
        await this.loadInitialState();
    }
    
    handleConnectionError() {
        this.retryCount++;
        
        if (this.retryCount >= this.maxRetries) {
            this.isConnected = false;
            this.updateConnectionStatus(false);
            this.showToast('Connection lost. Please check your network.', 'error');
        }
    }
    
    handleConnectionChange(isOnline) {
        if (isOnline && !this.isConnected) {
            this.loadInitialState();
        } else if (!isOnline) {
            this.isConnected = false;
            this.updateConnectionStatus(false);
        }
    }
    
    updateConnectionStatus(connected) {
        const statusElement = document.getElementById('connection-status');
        if (statusElement) {
            statusElement.textContent = connected ? '🟢 Connected' : '🔴 Disconnected';
            statusElement.className = `status-indicator ${connected ? 'connected' : 'disconnected'}`;
        }
    }
    
    async makeRequest(url, options = {}) {
        const controller = new AbortController();
        const timeoutId = setTimeout(() => controller.abort(), options.timeout || 5000);
        
        try {
            const response = await fetch(url, {
                ...options,
                signal: controller.signal
            });
            clearTimeout(timeoutId);
            return response;
        } catch (error) {
            clearTimeout(timeoutId);
            if (error.name === 'AbortError') {
                throw new Error('Request timeout');
            }
            throw error;
        }
    }
    
    showToast(message, type = 'info') {
        const container = document.getElementById('toast-container');
        if (!container) return;
        
        const toast = document.createElement('div');
        toast.className = `toast ${type}`;
        toast.textContent = message;
        
        container.appendChild(toast);
        
        // Trigger animation
        setTimeout(() => toast.classList.add('show'), 100);
        
        // Remove after 3 seconds
        setTimeout(() => {
            toast.classList.remove('show');
            setTimeout(() => container.removeChild(toast), 300);
        }, 3000);
    }
    
    hideLoadingOverlay() {
        const overlay = document.getElementById('loading-overlay');
        if (overlay) {
            overlay.classList.add('hidden');
        }
    }
    
    handleKeyboardShortcuts(e) {
        // Ctrl + 1-4 to toggle relays
        if (e.ctrlKey && e.key >= '1' && e.key <= '4') {
            e.preventDefault();
            const relayNumber = parseInt(e.key);
            const checkbox = document.getElementById(`relay${relayNumber}`);
            if (checkbox) {
                checkbox.checked = !checkbox.checked;
                this.toggleRelay(relayNumber, checkbox.checked);
            }
        }
        
        // Ctrl + A to toggle all on/off
        if (e.ctrlKey && e.key.toLowerCase() === 'a') {
            e.preventDefault();
            const allOn = Object.values(this.relays).every(state => !state);
            this.allRelays(allOn);
        }
        
        // F5 to refresh
        if (e.key === 'F5') {
            e.preventDefault();
            this.refreshStatus();
        }
    }
    
    setupServiceWorker() {
        if ('serviceWorker' in navigator) {
            navigator.serviceWorker.register('/sw.js')
                .then(registration => {
                    console.log('Service Worker registered:', registration);
                })
                .catch(error => {
                    console.log('Service Worker registration failed:', error);
                });
        }
    }
}

// Initialize the dashboard when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    window.dashboard = new HomeAutomationDashboard();
});

// Handle page visibility changes
document.addEventListener('visibilitychange', () => {
    if (document.visibilityState === 'visible' && window.dashboard) {
        window.dashboard.refreshStatus();
    }
});

// Handle beforeunload to clean up
window.addEventListener('beforeunload', () => {
    if (window.dashboard && window.dashboard.updateInterval) {
        clearInterval(window.dashboard.updateInterval);
    }
});
