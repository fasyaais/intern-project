<script setup lang="ts">
import {  onMounted, ref } from 'vue'
import networks from '@/networks.json'
import {
  AlertDialog,
  AlertDialogAction,
  AlertDialogCancel,
  AlertDialogContent,
  AlertDialogDescription,
  AlertDialogFooter,
  AlertDialogHeader,
  AlertDialogTitle,
  AlertDialogTrigger,
} from '@/components/ui/alert-dialog'

import {Input} from '@/components/ui/input'
import { Button } from './components/ui/button'

interface Networks {
  ssid:string,
  rssi:number,
  authmode:number,
}

const data = ref<Networks[]>()
const password = ref('')
const isSubmited = ref(false)

onMounted(async () => {
  const n = await fetch('http://192.168.4.1/api/scan')
  data.value = await n.json()
  // data.value = networks
  // console.log(data.value)
})

const submit = async (ssid:string)=>{
  await fetch("http://192.168.4.1/api/connect", {
    method: "POST",
    // headers: {
    //   "content-type" : "application/json",
    // },
    body: JSON.stringify({ssid,password:password.value})
  })
  isSubmited.value = true;
}

const refresh = async () => {
  const n = await fetch('http://192.168.4.1/api/scan')
  data.value = await n.json() 
  // console.log("refresh")
  console.info(data.value)
}


</script>

<template>
  <div class="mx-auto container md:w-lg p-4">
    <div v-if="!isSubmited">
      <div class="mb-3 flex justify-between content-center">
        <h1 class="text-3xl font-bold">All WiFi</h1>
        <button type="button" class="hover:cursor-pointer" @click="refresh()">
          <svg width="25px" height="25px" viewBox="0 0 50 50" xmlns="http://www.w3.org/2000/svg"><path d="M25 38c-7.2 0-13-5.8-13-13 0-3.2 1.2-6.2 3.3-8.6l1.5 1.3C15 19.7 14 22.3 14 25c0 6.1 4.9 11 11 11 1.6 0 3.1-.3 4.6-1l.8 1.8c-1.7.8-3.5 1.2-5.4 1.2z"/><path d="M34.7 33.7l-1.5-1.3c1.8-2 2.8-4.6 2.8-7.3 0-6.1-4.9-11-11-11-1.6 0-3.1.3-4.6 1l-.8-1.8c1.7-.8 3.5-1.2 5.4-1.2 7.2 0 13 5.8 13 13 0 3.1-1.2 6.2-3.3 8.6z"/><path d="M18 24h-2v-6h-6v-2h8z"/><path d="M40 34h-8v-8h2v6h6z"/></svg>
        </button>
      </div>
      <ul class="flex flex-col gap-4 bg-white p-4 rounded-2xl h-128 overflow-y-auto">
        <li v-for="network in data" class="flex justify-between">
          
          <AlertDialog>
            <AlertDialogTrigger class="w-full">
              <Button variant="ghost" class="flex justify-between w-full hover:bg-slate-300">
                <p>{{ network.ssid }}</p>
                <p>{{ network.rssi }}</p>
              </Button>
            </AlertDialogTrigger>
            <AlertDialogContent>
              <AlertDialogHeader>
                <AlertDialogTitle>Connect to {{ network.ssid }} ?</AlertDialogTitle>
                <AlertDialogDescription>
                  Apakah anda akan menghubungkan ke WiFi <span class="font-bold">{{ network.ssid }}</span> dengan nilai rssi {{ network.rssi }}?
                </AlertDialogDescription>
              </AlertDialogHeader>
              <div v-if="network.authmode != 0">
                <Input type="password" v-model="password" placeholder="Masukkan password wifi" />
              </div>
                <Input type="hidden" name="ssid" :value="network.ssid" />
                <Input type="hidden" name="authmode" :value="network.authmode" />
              <AlertDialogFooter>
                <AlertDialogCancel>Cancel</AlertDialogCancel>
                <AlertDialogAction @click="submit(network.ssid)" >Connect</AlertDialogAction>
              </AlertDialogFooter>
            </AlertDialogContent>
          </AlertDialog>

        </li>
      </ul>
    </div>
    <div v-else>
      <p>ESP akan di reboot</p>
    </div>
  </div>
</template>

<style scoped></style>
