<script setup>
import { ref, onMounted } from 'vue'

const sheetData = ref([])
const loading = ref(true)

const fetchData = async () => {
  try {
    // Replace with your actual Cloud Run URL after deployment
    const response = await fetch('https://toes-backend-202431845326.us-central1.run.app/data')
    const data = await response.json()
    sheetData.value = data
  } catch (error) {
    console.error('Error loading data:', error)
  } finally {
    loading.value = false
  }
}

onMounted(fetchData)
</script>

<template>
  <div class="container">
    <h1>TOES - Turbidity Station Information (test with Jeff)</h1>

    <div v-if="loading">Loading data from Google Cloud...</div>

    <table v-else border="1">
      <tr v-for="(row, index) in sheetData" :key="index">
        <td v-for="(cell, cellIndex) in row" :key="cellIndex">
          {{ cell }}
        </td>
      </tr>
    </table>
  </div>
</template>

<style scoped>
.container {
  font-family: sans-serif;
  padding: 20px;
}
table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 10px;
}
td {
  padding: 8px;
  text-align: left;
}
</style>
