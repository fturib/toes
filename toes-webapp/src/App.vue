<script setup>
import { ref, onMounted, watch } from 'vue'
import Chart from 'chart.js/auto'

const sheetData = ref([])
const firestoreData = ref([])
const stationCounts = ref({})
const loading = ref(true)
const activeTab = ref('firestore')
const selectedStation = ref(null)
const stationData = ref([])
const chartInstance = ref(null)

const makeDate = (dbTimestamp) => {
  if (dbTimestamp._seconds && dbTimestamp._nanoseconds) {
    return new Date(dbTimestamp._seconds*1000 + dbTimestamp._nanoseconds/1000000)
  }
  return new Date()
}

const asSimpleString = (dbTimestamp) => {
  const date = makeDate(dbTimestamp)
  return date.toLocaleDateString() + ' ' + date.toLocaleTimeString()
}

const fetchSheetData = async () => {
  try {
    const response = await fetch('https://toes-backend-202431845326.us-central1.run.app/data')
    const data = await response.json()
    sheetData.value = data
  } catch (error) {
    console.error('Error loading sheet data:', error)
  }
}

const fetchFirestoreData = async () => {
  try {
    const response = await fetch('https://toes-backend-202431845326.us-central1.run.app/data-points')
    const data = await response.json()
    firestoreData.value = data
  } catch (error) {
    console.error('Error loading firestore data:', error)
  }
}

const fetchStationCounts = async () => {
  try {
    const response = await fetch('https://toes-backend-202431845326.us-central1.run.app/stations')
    const data = await response.json()
    stationCounts.value = data
  } catch (error) {
    console.error('Error loading station counts:', error)
  }
}

const fetchStationData = async (station) => {
  try {
    const response = await fetch(`https://toes-backend-202431845326.us-central1.run.app/data-points?station=${encodeURIComponent(station)}`)
    const data = await response.json()
    stationData.value = data
  } catch (error) {
    console.error('Error loading station data:', error)
  }
}

const selectStation = async (station) => {
  selectedStation.value = station
  await fetchStationData(station)
}

const renderChart = () => {
  if (!selectedStation.value || stationData.value.length === 0) return

  // Destroy previous chart if it exists
  if (chartInstance.value) {
    chartInstance.value.destroy()
  }

  const ctx = document.getElementById('turbidityChart')
  if (!ctx) return

  // Sort data by collected-at ascending
  const sortedData = [...stationData.value].sort((a, b) => {
    const timeA = makeDate(a['collected-at'])
    const timeB = makeDate(b['collected-at'])
    return timeA - timeB
  })

  const labels = sortedData.map(d => {
    const date = makeDate(d['collected-at'])
    return date.toLocaleString()
  })

  const values = sortedData.map(d => d.turbidity?.value || 0)

  chartInstance.value = new Chart(ctx, {
    type: 'line',
    data: {
      labels: labels,
      datasets: [
        {
          label: `Turbidity for ${selectedStation.value}`,
          data: values,
          borderColor: '#007bff',
          backgroundColor: 'rgba(0, 123, 255, 0.1)',
          tension: 0.3,
          fill: true,
          pointRadius: 4,
          pointBackgroundColor: '#007bff'
        }
      ]
    },
    options: {
      responsive: true,
      maintainAspectRatio: true,
      plugins: {
        legend: {
          display: true,
          position: 'top'
        },
        title: {
          display: true,
          text: `Turbidity Data - ${selectedStation.value}`
        }
      },
      scales: {
        y: {
          beginAtZero: true,
          title: {
            display: true,
            text: 'Turbidity Value'
          }
        },
        x: {
          title: {
            display: true,
            text: 'Time'
          }
        }
      }
    }
  })
}

const loadData = async () => {
  loading.value = true
  await Promise.all([fetchSheetData(), fetchFirestoreData(), fetchStationCounts()])
  loading.value = false
}

watch(() => stationData.value, () => {
  renderChart()
})

onMounted(loadData)
</script>

<template>
  <div class="container">
    <h1>TOES - Turbidity Station Information</h1>

    <div v-if="loading">Loading data from Google Cloud...</div>

    <div v-else>
      <div class="tabs">
        <button 
          class="tab-button" 
          :class="{ active: activeTab === 'firestore' }"
          @click="activeTab = 'firestore'"
        >
          Firestore Data
        </button>
        <button 
          class="tab-button" 
          :class="{ active: activeTab === 'stations' }"
          @click="activeTab = 'stations'"
        >
          Stations & Analysis
        </button>
        <button 
          class="tab-button" 
          :class="{ active: activeTab === 'sheet' }"
          @click="activeTab = 'sheet'"
        >
          Sheet Data
        </button>
      </div>

      <div v-if="activeTab === 'firestore'" class="tab-content">
        <h2>Firestore Data</h2>
        <table border="1" v-if="firestoreData.length > 0">
          <thead>
            <tr>
              <th>ID</th>
              <th>Station</th>
              <th>Collected At</th>
              <th>Turbidity Value</th>
              <th>ADC</th>
              <th>Raw</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="row in firestoreData" :key="row.id">
              <td>{{ row.id }}</td>
              <td>{{ row.station }}</td>
              <td>{{ makeDate(row['collected-at']) }}</td>
              <td>{{ row.turbidity?.value }}</td>
              <td>{{ row.turbidity?.adc }}</td>
              <td>{{ row.turbidity?.raw }}</td>
            </tr>
          </tbody>
        </table>
        <div v-else>No firestore data available</div>
      </div>

      <div v-if="activeTab === 'stations'" class="tab-content">
        <h2>Stations & Analysis</h2>
        <div class="stations-layout">
          <div class="stations-table-section">
            <h3>Stations</h3>
            <table border="1" v-if="Object.keys(stationCounts).length > 0" class="stations-table">
              <thead>
                <tr>
                  <th>Station</th>
                  <th>Data Points</th>
                </tr>
              </thead>
              <tbody>
                <tr 
                  v-for="(count, station) in stationCounts" 
                  :key="station"
                  class="station-row"
                  :class="{ selected: selectedStation === station }"
                  @click="selectStation(station)"
                >
                  <td>{{ station }}</td>
                  <td>{{ count }}</td>
                </tr>
              </tbody>
            </table>
            <div v-else>No stations available</div>

            <div v-if="selectedStation" class="selected-station-data">
              <h4>Data for {{ selectedStation }}</h4>
              <table border="1" v-if="stationData.length > 0" class="data-table">
                <thead>
                  <tr>
                    <th>Collected At</th>
                    <th>Turbidity Value</th>
                  </tr>
                </thead>
                <tbody>
                  <tr v-for="row in stationData" :key="row.id">
                    <td style="white-space: nowrap;">{{ asSimpleString(row['collected-at'])}}</td>
                    <td>{{ row.turbidity?.value }}</td>
                  </tr>
                </tbody>
              </table>
              <div v-else>No data available for this station</div>
            </div>
          </div>

          <div class="chart-section" v-if="selectedStation">
            <h3>Turbidity Graph - {{ selectedStation }}</h3>
            <div class="chart-container">
              <canvas id="turbidityChart"></canvas>
            </div>
          </div>
        </div>
      </div>

      <div v-if="activeTab === 'sheet'" class="tab-content">
        <h2>Sheet Data</h2>
        <table border="1" v-if="sheetData.length > 0">
          <tr v-for="(row, index) in sheetData" :key="index">
            <td v-for="(cell, cellIndex) in row" :key="cellIndex">
              {{ cell }}
            </td>
          </tr>
        </table>
        <div v-else>No sheet data available</div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.container {
  font-family: sans-serif;
  padding: 20px;
}

.tabs {
  display: flex;
  gap: 10px;
  margin: 20px 0;
  border-bottom: 2px solid #ddd;
}

.tab-button {
  padding: 10px 20px;
  border: none;
  background: transparent;
  cursor: pointer;
  font-size: 16px;
  border-bottom: 3px solid transparent;
  transition: all 0.3s ease;
}

.tab-button:hover {
  background: #f0f0f0;
}

.tab-button.active {
  color: #007bff;
  border-bottom-color: #007bff;
}

.tab-content {
  margin-top: 20px;
}

table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 10px;
}

td, th {
  padding: 8px;
  text-align: left;
  border: 1px solid #ddd;
}

th {
  background-color: #f5f5f5;
  font-weight: bold;
}

.stations-layout {
  display: grid;
  grid-template-columns: 300px 1fr;
  gap: 20px;
  margin-top: 20px;
}

.stations-table-section {
  border: 1px solid #ddd;
  padding: 15px;
  border-radius: 8px;
}

.stations-table-section h3 {
  margin-top: 0;
  margin-bottom: 15px;
}

.stations-table {
  width: 100% !important;
  margin: 0 !important;
}

.station-row {
  cursor: pointer;
  transition: background-color 0.2s;
}

.station-row:hover {
  background-color: #f0f0f0;
}

.station-row.selected {
  background-color: #e3f2fd;
  font-weight: bold;
}

.selected-station-data {
  margin-top: 20px;
  padding-top: 20px;
  border-top: 1px solid #ddd;
}

.selected-station-data h4 {
  margin-top: 0;
  margin-bottom: 10px;
  color: #333;
}

.data-table {
  width: 100% !important;
  margin: 10px 0 !important;
  font-size: 14px;
}
.data-table td {
  white-space: nowrap;
}

.chart-section {
  border: 1px solid #ddd;
  padding: 15px;
  border-radius: 8px;
}

.chart-section h3 {
  margin-top: 0;
  margin-bottom: 15px;
}

.chart-container {
  position: relative;
  height: 400px;
  width: 100%;
}

#turbidityChart {
  max-height: 400px;
}

@media (max-width: 768px) {
  .stations-layout {
    grid-template-columns: 1fr;
  }
}
</style>
