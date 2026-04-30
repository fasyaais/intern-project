<script lang="ts" setup>
import { onMounted, onUnmounted, ref } from 'vue';
import type { GPIO } from './types/gpio';
import { Switch } from './components/ui/switch';
import Separator from './components/ui/separator/Separator.vue';
import { Dialog, DialogClose, DialogContent, DialogFooter, DialogHeader, DialogTitle, DialogTrigger } from './components/ui/dialog';
import { Button } from './components/ui/button';
import { Label } from './components/ui/label';
import { Input } from './components/ui/input';

const gpio = ref<GPIO[]>([])

const pinGPIO = ref<number>();

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
    const res = await fetch("http://192.168.1.22/api/gpio")
    const json = (await res.json() as GPIO[])

    if (gpio.value.length === 0) {
      gpio.value = json
    } else {
      json.forEach(newItem => {
        const existing = gpio.value.find(g => g.pin === newItem.pin)
        if (existing) {
          existing.state = newItem.state
          existing.last_click_at = newItem.last_click_at
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

  await fetch("http://192.168.1.22/api/gpio/state", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ pin, state: newState, last_click_at: new Date().getTime() })
  })
}

const dateToString = (time: number): string => {
  const pars = new Date(time)
  return `${pars.getHours()}:${pars.getMinutes()}:${pars.getSeconds()}`
}

const addGPIO = async () => {
  await fetch("http://192.168.1.22/api/gpio", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ pin: pinGPIO.value })
  })
}

const deleteGPIO = async (pin:number) => {
  await fetch("http://192.168.1.22/api/gpio",{
    method: "DELETE",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ pin })
  })
  const res = await fetch("http://192.168.1.22/api/gpio")
  const json = (await res.json() as GPIO[])

  if (gpio.value.length === 0) {
    gpio.value = json
  } else {
    const idx =gpio.value.findIndex(g => g.pin === pin)
    gpio.value.splice(idx,1);
  }
}
</script>

<template>
  <div class="container md:w-xl mx-auto bg-gray-200 p-5 mt-14 rounded-3xl">
    <div class="flex justify-between items-center">
      <h1 class="text-3xl font-bold ">GPIO RAVELEDGE</h1>
      <Dialog>
        <DialogTrigger as-child>
          <Button variant="ghost" class="hover:bg-white/90 bg-white/60 rounded-sm p-1.5">
            <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke-width="2.5"
              stroke="currentColor" class="size-3.5">
              <path stroke-linecap="round" stroke-linejoin="round" d="M12 4.5v15m7.5-7.5h-15" />
            </svg>
          </Button>
        </DialogTrigger>
        <DialogContent class="sm:max-w-md">
          <DialogHeader>
            <DialogTitle>Tambah LED</DialogTitle>
          </DialogHeader>
          <div class="flex items-center gap-2">
            <div class="grid flex-1 gap-2">
              <Label for="link" class="sr-only">
                Link
              </Label>
              <Input type="number" id="link" placeholder="Masukkan pin led..." v-model="pinGPIO" />
            </div>
          </div>
          <DialogFooter class="sm:justify-end">
            <DialogClose as-child>
              <Button type="button" variant="secondary">
                Close
              </Button>
            </DialogClose>
            <DialogClose as-child>
              <Button type="submit" @click="addGPIO" variant="default" class="bg-green-600 hover:bg-green-900">
                Submit
              </Button>
            </DialogClose>
          </DialogFooter>
        </DialogContent>
      </Dialog>
    </div>

    <Separator class="my-4 bg-gray-50 rounded-2xl" />
    <ul class="flex flex-wrap gap-3 justify-between">
      <li class="flex flex-col justify-between w-40 bg-white p-3 rounded-xl gap-2" v-for="row in gpio" :key="row.pin">
        <div class="flex justify-between items-center">
          <h3 class="text-lg font-semibold">LED {{ row.pin }}</h3>
          <div class="flex items-center gap-1">
            <div class=" rounded-4xl h-3 w-3" :class="[row.state ? 'bg-green-600' : 'bg-red-600']"></div>
            <button class=" hover:bg-red-600 bg-red-500 p-1.5 rounded-md hover:cursor-pointer" @click="deleteGPIO(row.pin)">
              <svg xmlns="http://www.w3.org/2000/svg" x="0px" y="0px" width="10" height="10"
                viewBox="0,0,255.98958,255.98958">
                <g fill="#ffffff" fill-rule="nonzero" stroke="none" stroke-width="1" stroke-linecap="butt"
                  stroke-linejoin="miter" stroke-miterlimit="10" stroke-dasharray="" stroke-dashoffset="0"
                  font-family="none" font-weight="none" font-size="none" text-anchor="none"
                  style="mix-blend-mode: normal">
                  <g transform="scale(10.66667,10.66667)">
                    <path
                      d="M10,2l-1,1h-5v2h1v15c0,0.52222 0.19133,1.05461 0.56836,1.43164c0.37703,0.37703 0.90942,0.56836 1.43164,0.56836h10c0.52222,0 1.05461,-0.19133 1.43164,-0.56836c0.37703,-0.37703 0.56836,-0.90942 0.56836,-1.43164v-15h1v-2h-5l-1,-1zM7,5h10v15h-10zM9,7v11h2v-11zM13,7v11h2v-11z">
                    </path>
                  </g>
                </g>
              </svg>
            </button>
          </div>
        </div>
        <div class="flex justify-between">
          <Switch :id="'id-' + row.pin" :model-value="row.state"
            @update:model-value="(newState) => changedStatus(row.pin, newState)" />
          <!-- <p class="text-xs font-extralight">{{ (row.state ? "ON" : "OFF") }}</p> -->
          <p class="text-xs font-extralight">{{ dateToString(row.last_click_at) }}</p>
        </div>
      </li>
    </ul>
  </div>
</template>