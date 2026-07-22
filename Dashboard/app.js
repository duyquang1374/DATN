/* ============================================================
   NHÀ MÀNG THÔNG MINH - DASHBOARD APP
   ============================================================ */

// ======================== STATE ========================

const state = {
    mqttClient: null,
    connected: false,
    dataCount: 0,
    sessionStart: Date.now(),
    sensorData: [],        // Lưu tối đa 1080 mẫu (9 giờ @ 30s)
    maxDataPoints: 1080,
    chartRange: 30,        // Phút hiển thị trên chart
    prevTemp: null,
    prevHumid: null,

    // MQTT default config
    config: {
        mode: 'local',
        host: 'ws://192.168.1.227:9001',
        topic: 'greenhouse/sensor',
        user: '',
        pass: ''
    }
};

// ======================== DOM ELEMENTS ========================

const DOM = {
    tempValue: document.getElementById('tempValue'),
    humidValue: document.getElementById('humidValue'),
    tempBar: document.getElementById('tempBar'),
    humidBar: document.getElementById('humidBar'),
    tempTrend: document.getElementById('tempTrend'),
    humidTrend: document.getElementById('humidTrend'),
    statusBadge: document.getElementById('statusBadge'),
    statusText: document.getElementById('statusText'),
    lastUpdate: document.getElementById('lastUpdate'),
    dataCount: document.getElementById('dataCount'),
    sessionTime: document.getElementById('sessionTime'),
    clock: document.getElementById('clock'),
    tableBody: document.getElementById('tableBody'),

    // Settings
    settingsModal: document.getElementById('settingsModal'),
    btnSettings: document.getElementById('btnSettings'),
    btnCloseModal: document.getElementById('btnCloseModal'),
    btnConnect: document.getElementById('btnConnect'),
    btnExport: document.getElementById('btnExport'),
    mqttMode: document.getElementById('mqttMode'),
    mqttHost: document.getElementById('mqttHost'),
    mqttUser: document.getElementById('mqttUser'),
    mqttPass: document.getElementById('mqttPass'),
    mqttTopic: document.getElementById('mqttTopic'),
    authGroup: document.getElementById('authGroup')
};

// ======================== CHART ========================

let mainChart = null;

function initChart() {
    const ctx = document.getElementById('mainChart').getContext('2d');

    mainChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [
                {
                    label: 'Nhiệt độ (°C)',
                    data: [],
                    borderColor: '#f59e0b',
                    backgroundColor: 'rgba(245, 158, 11, 0.08)',
                    borderWidth: 2,
                    fill: true,
                    tension: 0.4,
                    pointRadius: 0,
                    pointHoverRadius: 5,
                    pointHoverBackgroundColor: '#f59e0b',
                    yAxisID: 'y'
                },
                {
                    label: 'Độ ẩm (%)',
                    data: [],
                    borderColor: '#3b82f6',
                    backgroundColor: 'rgba(59, 130, 246, 0.08)',
                    borderWidth: 2,
                    fill: true,
                    tension: 0.4,
                    pointRadius: 0,
                    pointHoverRadius: 5,
                    pointHoverBackgroundColor: '#3b82f6',
                    yAxisID: 'y1'
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            interaction: {
                mode: 'index',
                intersect: false
            },
            plugins: {
                legend: {
                    labels: {
                        color: '#8b8fa3',
                        font: { family: "'Be Vietnam Pro'", size: 12 },
                        usePointStyle: true,
                        pointStyle: 'circle'
                    }
                },
                tooltip: {
                    backgroundColor: 'rgba(15, 17, 23, 0.95)',
                    titleFont: { family: "'Be Vietnam Pro'" },
                    bodyFont: { family: "'Be Vietnam Pro'" },
                    borderColor: 'rgba(255,255,255,0.1)',
                    borderWidth: 1,
                    padding: 12,
                    cornerRadius: 8
                }
            },
            scales: {
                x: {
                    grid: { color: 'rgba(255,255,255,0.03)', drawBorder: false },
                    ticks: {
                        color: '#5a5e72',
                        font: { family: "'Be Vietnam Pro'", size: 11 },
                        maxTicksLimit: 8
                    }
                },
                y: {
                    position: 'left',
                    grid: { color: 'rgba(255,255,255,0.03)', drawBorder: false },
                    ticks: {
                        color: '#f59e0b',
                        font: { family: "'Be Vietnam Pro'", size: 11 },
                        callback: v => v + '°C'
                    },
                    title: {
                        display: true,
                        text: 'Nhiệt độ',
                        color: '#f59e0b',
                        font: { family: "'Be Vietnam Pro'", size: 12 }
                    }
                },
                y1: {
                    position: 'right',
                    grid: { drawOnChartArea: false },
                    ticks: {
                        color: '#3b82f6',
                        font: { family: "'Be Vietnam Pro'", size: 11 },
                        callback: v => v + '%'
                    },
                    title: {
                        display: true,
                        text: 'Độ ẩm',
                        color: '#3b82f6',
                        font: { family: "'Be Vietnam Pro'", size: 12 }
                    },
                    min: 0,
                    max: 100
                }
            }
        }
    });
}

function updateChart() {
    if (!mainChart || state.sensorData.length === 0) return;

    const samplesPerMinute = 2; // 30s interval
    const maxSamples = state.chartRange * samplesPerMinute;
    const data = state.sensorData.slice(-maxSamples);

    mainChart.data.labels = data.map(d => d.time);
    mainChart.data.datasets[0].data = data.map(d => d.temperature);
    mainChart.data.datasets[1].data = data.map(d => d.humidity);
    mainChart.update('none');
}

// ======================== MQTT ========================

function connectMQTT() {
    // Disconnect existing
    if (state.mqttClient) {
        try { state.mqttClient.end(true); } catch(e) {}
    }

    const config = state.config;
    updateStatus('connecting');

    const options = {
        clientId: 'Dashboard_' + Math.random().toString(16).substring(2, 8),
        clean: true,
        reconnectPeriod: 5000
    };

    if (config.user) {
        options.username = config.user;
        options.password = config.pass;
    }

    try {
        state.mqttClient = mqtt.connect(config.host, options);

        state.mqttClient.on('connect', () => {
            console.log('[MQTT] Connected!');
            state.connected = true;
            updateStatus('connected');
            state.mqttClient.subscribe(config.topic, (err) => {
                if (err) console.error('[MQTT] Subscribe error:', err);
                else console.log('[MQTT] Subscribed:', config.topic);
            });
        });

        state.mqttClient.on('message', (topic, message) => {
            try {
                const payload = JSON.parse(message.toString());
                handleSensorData(payload);
            } catch (e) {
                console.error('[MQTT] Parse error:', e);
            }
        });

        state.mqttClient.on('error', (err) => {
            console.error('[MQTT] Error:', err);
            updateStatus('error');
        });

        state.mqttClient.on('close', () => {
            state.connected = false;
            updateStatus('disconnected');
        });

        state.mqttClient.on('reconnect', () => {
            updateStatus('connecting');
        });

    } catch (e) {
        console.error('[MQTT] Connection failed:', e);
        updateStatus('error');
    }
}

// ======================== DATA HANDLING ========================

function handleSensorData(payload) {
    const temp = parseFloat(payload.temperature);
    const humid = parseFloat(payload.humidity);

    if (isNaN(temp) || isNaN(humid)) return;

    const now = new Date();
    const timeStr = now.toLocaleTimeString('vi-VN', { hour: '2-digit', minute: '2-digit', second: '2-digit' });
    const fullTime = now.toLocaleString('vi-VN');

    // Save data
    state.sensorData.push({
        time: timeStr,
        fullTime: fullTime,
        temperature: temp,
        humidity: humid,
        timestamp: now
    });

    // Giới hạn dữ liệu
    if (state.sensorData.length > state.maxDataPoints) {
        state.sensorData.shift();
    }

    state.dataCount++;

    // Update UI
    updateCards(temp, humid);
    updateChart();
    updateTable(fullTime, temp, humid);

    // Save previous values for trend
    state.prevTemp = temp;
    state.prevHumid = humid;
}

function updateCards(temp, humid) {
    // Temperature
    DOM.tempValue.textContent = temp.toFixed(1);
    DOM.tempBar.style.width = Math.min((temp / 50) * 100, 100) + '%';

    // Humidity
    DOM.humidValue.textContent = humid.toFixed(1);
    DOM.humidBar.style.width = humid + '%';

    // Trends
    if (state.prevTemp !== null) {
        const tempDiff = temp - state.prevTemp;
        updateTrend(DOM.tempTrend, tempDiff, '°C');
    }

    if (state.prevHumid !== null) {
        const humidDiff = humid - state.prevHumid;
        updateTrend(DOM.humidTrend, humidDiff, '%');
    }

    // Data count
    DOM.dataCount.textContent = state.dataCount;

    // Last update
    DOM.lastUpdate.innerHTML = `<span class="trend-text">Cập nhật: ${new Date().toLocaleTimeString('vi-VN')}</span>`;

    // Animate value change
    DOM.tempValue.classList.add('value-flash');
    DOM.humidValue.classList.add('value-flash');
    setTimeout(() => {
        DOM.tempValue.classList.remove('value-flash');
        DOM.humidValue.classList.remove('value-flash');
    }, 500);
}

function updateTrend(element, diff, unit) {
    let arrow, cls, text;
    if (Math.abs(diff) < 0.1) {
        arrow = '→'; cls = 'trend-stable'; text = 'Ổn định';
    } else if (diff > 0) {
        arrow = '↑'; cls = 'trend-up'; text = `+${diff.toFixed(1)}${unit}`;
    } else {
        arrow = '↓'; cls = 'trend-down'; text = `${diff.toFixed(1)}${unit}`;
    }
    element.innerHTML = `<span class="trend-arrow ${cls}">${arrow}</span><span class="trend-text">${text}</span>`;
}

function updateTable(time, temp, humid) {
    // Remove empty row
    const emptyRow = DOM.tableBody.querySelector('.empty-row');
    if (emptyRow) emptyRow.parentElement.remove();

    const row = document.createElement('tr');
    row.innerHTML = `
        <td>${time}</td>
        <td style="color: ${temp > 35 ? '#ef4444' : '#f59e0b'}">${temp.toFixed(1)}</td>
        <td style="color: #3b82f6">${humid.toFixed(1)}</td>
    `;
    row.style.animation = 'fadeIn 0.3s ease';

    // Thêm vào đầu bảng
    DOM.tableBody.insertBefore(row, DOM.tableBody.firstChild);

    // Giới hạn 50 hàng
    while (DOM.tableBody.children.length > 50) {
        DOM.tableBody.removeChild(DOM.tableBody.lastChild);
    }
}

function updateStatus(status) {
    const badge = DOM.statusBadge;
    const text = DOM.statusText;

    switch (status) {
        case 'connected':
            badge.textContent = '🟢 Đã kết nối';
            badge.className = 'badge connected';
            text.textContent = 'Online';
            text.className = 'status-text status-online';
            break;
        case 'connecting':
            badge.textContent = '🟡 Đang kết nối...';
            badge.className = 'badge';
            text.textContent = 'Connecting';
            text.className = 'status-text';
            break;
        case 'disconnected':
            badge.textContent = '🔴 Mất kết nối';
            badge.className = 'badge error';
            text.textContent = 'Offline';
            text.className = 'status-text status-offline';
            break;
        case 'error':
            badge.textContent = '❌ Lỗi kết nối';
            badge.className = 'badge error';
            text.textContent = 'Error';
            text.className = 'status-text status-offline';
            break;
    }
}

// ======================== EXPORT CSV ========================

function exportCSV() {
    if (state.sensorData.length === 0) {
        alert('Chưa có dữ liệu để export!');
        return;
    }

    let csv = 'Thời gian,Nhiệt độ (°C),Độ ẩm (%)\n';
    state.sensorData.forEach(d => {
        csv += `${d.fullTime},${d.temperature},${d.humidity}\n`;
    });

    const blob = new Blob(['\ufeff' + csv], { type: 'text/csv;charset=utf-8;' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `greenhouse_${new Date().toISOString().slice(0, 10)}.csv`;
    a.click();
    URL.revokeObjectURL(url);
}

// ======================== CLOCK & SESSION ========================

function updateClock() {
    DOM.clock.textContent = new Date().toLocaleTimeString('vi-VN');

    const elapsed = Math.floor((Date.now() - state.sessionStart) / 60000);
    DOM.sessionTime.textContent = `Phiên: ${elapsed} phút`;
}

// ======================== SETTINGS ========================

function loadConfig() {
    const saved = localStorage.getItem('greenhouse_mqtt_config');
    if (saved) {
        try {
            state.config = JSON.parse(saved);
            DOM.mqttMode.value = state.config.mode;
            DOM.mqttHost.value = state.config.host;
            DOM.mqttUser.value = state.config.user || '';
            DOM.mqttPass.value = state.config.pass || '';
            DOM.mqttTopic.value = state.config.topic;
            if (state.config.mode === 'cloud') {
                DOM.authGroup.style.display = 'flex';
            }
        } catch(e) {}
    }
}

function saveConfig() {
    state.config = {
        mode: DOM.mqttMode.value,
        host: DOM.mqttHost.value,
        topic: DOM.mqttTopic.value,
        user: DOM.mqttUser.value,
        pass: DOM.mqttPass.value
    };
    localStorage.setItem('greenhouse_mqtt_config', JSON.stringify(state.config));
}

// ======================== EVENT LISTENERS ========================

DOM.btnSettings.addEventListener('click', () => {
    DOM.settingsModal.classList.add('active');
});

DOM.btnCloseModal.addEventListener('click', () => {
    DOM.settingsModal.classList.remove('active');
});

DOM.settingsModal.addEventListener('click', (e) => {
    if (e.target === DOM.settingsModal) {
        DOM.settingsModal.classList.remove('active');
    }
});

DOM.mqttMode.addEventListener('change', () => {
    const mode = DOM.mqttMode.value;
    if (mode === 'cloud') {
        DOM.mqttHost.value = 'wss://YOUR_CLUSTER.s1.eu.hivemq.cloud:8884/mqtt';
        DOM.authGroup.style.display = 'flex';
    } else {
        DOM.mqttHost.value = 'ws://192.168.1.227:9001';
        DOM.authGroup.style.display = 'none';
    }
});

DOM.btnConnect.addEventListener('click', () => {
    saveConfig();
    DOM.settingsModal.classList.remove('active');
    connectMQTT();
});

DOM.btnExport.addEventListener('click', exportCSV);

// Chart range buttons
document.querySelectorAll('.btn-chart').forEach(btn => {
    btn.addEventListener('click', () => {
        document.querySelectorAll('.btn-chart').forEach(b => b.classList.remove('active'));
        btn.classList.add('active');
        state.chartRange = parseInt(btn.dataset.range);
        updateChart();
    });
});

// ======================== DEMO MODE ========================
// Tạo dữ liệu giả để preview khi chưa kết nối MQTT

function startDemoMode() {
    if (state.connected) return;

    console.log('[DEMO] Chế độ demo - dữ liệu giả');
    DOM.statusBadge.textContent = '🟠 Demo Mode';

    let i = 0;
    const demoInterval = setInterval(() => {
        if (state.connected) {
            clearInterval(demoInterval);
            return;
        }

        const baseTemp = 28 + 5 * Math.sin(i * 0.05);
        const temp = baseTemp + (Math.random() - 0.5) * 2;
        const humid = 70 + 10 * Math.cos(i * 0.03) + (Math.random() - 0.5) * 5;

        handleSensorData({
            temperature: parseFloat(temp.toFixed(1)),
            humidity: parseFloat(Math.max(40, Math.min(95, humid)).toFixed(1))
        });

        i++;
    }, 2000);

    // Auto stop demo after 5 min
    setTimeout(() => clearInterval(demoInterval), 300000);
}

// ======================== INIT ========================

function init() {
    console.log('🌿 Nhà Màng Thông Minh - Dashboard v1.0');
    initChart();
    loadConfig();
    updateClock();
    setInterval(updateClock, 1000);

    // Thử kết nối MQTT ngay
    connectMQTT();

    // Nếu sau 5s chưa kết nối, bật demo mode
    setTimeout(() => {
        if (!state.connected) {
            startDemoMode();
        }
    }, 5000);
}

init();
