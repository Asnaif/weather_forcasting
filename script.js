const socket = io('http://localhost:5000');
let historicalData = [];

const charts = {
    temperature: new Chart(document.getElementById('temperatureChart'), createChartConfig('Temperature (°C)', '#e74c3c')),
    humidity: new Chart(document.getElementById('humidityChart'), createChartConfig('Humidity (%)', '#3498db')),
    airQuality: new Chart(document.getElementById('airQualityChart'), createChartConfig('Air Quality (ppm)', '#2ecc71'))
};

function createChartConfig(label, color) {
    return {
        type: 'line',
        data: { labels: [], datasets: [{ label, data: [], borderColor: color, fill: false, tension: 0.3 }] },
        options: { responsive: true, scales: { x: { display: false }, y: { beginAtZero: false } } }
    };
}

function updateAllCharts(data) {
    Object.keys(charts).forEach(metric => {
        const chart = charts[metric];
        chart.data.labels.push(new Date(data.timestamp).toLocaleTimeString());
        chart.data.datasets[0].data.push(data[metric]);
        if (chart.data.labels.length > 20) chart.data.labels.shift();
        chart.update();
    });
}

socket.on('new_data', (newData) => {
    historicalData.push(newData);
    document.querySelector('.temperature-gauge').innerHTML = `${newData.temperature.toFixed(1)}°C`;
    document.querySelector('.humidity-gauge').innerHTML = `${newData.humidity.toFixed(1)}%`;
    document.querySelector('.airquality-gauge').innerHTML = `${newData.air_quality}ppm`;

    const tbody = document.getElementById('tableBody');
    const newRow = document.createElement('tr');
    newRow.innerHTML = `
        <td>${new Date(newData.timestamp).toLocaleString()}</td>
        <td>${newData.temperature.toFixed(1)}°C</td>
        <td>${newData.humidity.toFixed(1)}%</td>
        <td>${newData.air_quality}ppm</td>
        <td>${newData.air_quality > 1000 ? '<i class="fas fa-exclamation-triangle" style="color: #e74c3c"></i>' : '<i class="fas fa-check" style="color: #2ecc71"></i>'}</td>
    `;
    tbody.prepend(newRow);
    if (tbody.children.length > 15) tbody.lastElementChild.remove();

    updateAllCharts(newData);
});
