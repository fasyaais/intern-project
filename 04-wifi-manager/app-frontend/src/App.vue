<script lang="ts" setup>
import { onMounted, onUnmounted, ref } from 'vue';
import type { Response } from './types/type';
import type { GPIO } from './types/gpio';
import { Switch } from './components/ui/switch';
import Separator from './components/ui/separator/Separator.vue';

const gpio = ref<GPIO[]>([])
let pollingGPIO: ReturnType<typeof setInterval> | null = null

onMounted(() => {
  fetchGPIO()
  pollingGPIO = setInterval(fetchGPIO, 1000)
})

onUnmounted(() => {
  if (pollingGPIO) clearInterval(pollingGPIO)
})

async function fetchGPIO() {
  try {
    const res = await fetch("http://192.168.1.51:3000/api/gpio")
    const json = (await res.json() as Response<GPIO[]>)

    if (gpio.value.length === 0) {
      gpio.value = json.data
    } else {
      json.data.forEach(newItem => {
        const existing = gpio.value.find(g => g.pin === newItem.pin)
        if (existing) {
          existing.state = newItem.state
        } else {
          gpio.value.push(newItem)
        }
      })
    }
  } catch (err) {
    console.error("Gagal fetch GPIO:", err)
  }
}

const changedStatus = async (pin: number, newState: boolean) => {
  const item = gpio.value.find(g => g.pin === pin)
  if (item) item.state = newState

  await fetch("http://192.168.1.51:3000/api/gpio/state", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ pin, state: newState })
  })
}
</script>

<template>
  <div class="container md:w-xl mx-auto bg-gray-200 p-5 mt-14 rounded-3xl">
    <h1 class="text-3xl font-bold ">GPIO RAVELEDGE</h1>
    <Separator class="my-4 bg-gray-50 rounded-2xl" />
    <ul  class="flex flex-wrap gap-3 justify-between">
      <li class="flex flex-col justify-between w-40 bg-white p-3 rounded-xl" v-for="row in gpio" :key="row.pin">
        <div class="flex justify-between">
          <h3 class="text-lg font-semibold">LED {{ row.pin }}</h3>
          <div class=" rounded-4xl h-3 w-3" :class="[row.state?'bg-green-600':'bg-red-600']"></div>
        </div>
        <div class="flex justify-between">
          <Switch
            :id="'id-' + row.pin"
            :model-value="row.state"
            @update:model-value="(newState) => changedStatus(row.pin, newState)"
          />
          <p class="text-xs font-extralight">{{ (row.state ? "ON":"OFF") }}</p>
        </div>
      </li>
    </ul>
  </div>
</template>