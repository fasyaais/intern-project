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
  rssi:number
}

const data = ref<{data:Networks[]}>()
const password = ref('')
const isSubmited = ref(false)

onMounted(async () => {
  const n = await fetch('http://192.168.4.1:8080/api/scan')
  data.value = await n.json()
  // data.value = networks
  // console.log(data.value.data)
})

const submit = async (ssid:string)=>{
  await fetch("http://192.168.4.1:8080/api/connect", {
    method: "POST",
    // headers: {
    //   "content-type" : "application/json",
    // },
    body: JSON.stringify({ssid,password:password.value})
  })
  isSubmited.value = true;
}


</script>

<template>
  <div class="mx-auto container md:w-lg p-4">
    <div v-if="!isSubmited">
      <div class="mb-3">
        <h1 class="text-3xl font-bold">All WiFi</h1>
      </div>
      <ul class="flex flex-col gap-4 bg-white p-4 rounded-2xl h-128 overflow-y-auto">
        <li v-for="network in data?.data" class="flex justify-between">
          
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
              <div>
                <Input type="password" v-model="password" placeholder="Masukkan password wifi" />
                <Input type="hidden" name="ssid" :value="network.ssid" />
              </div>
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
