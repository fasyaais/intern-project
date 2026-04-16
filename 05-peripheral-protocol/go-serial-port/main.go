package main

import (
	"fmt"
	"log"
	"os"
	"time"

	"go.bug.st/serial"
)

func main() {
	args :=os.Args
	data := []byte(args[1])
	payload := []byte{}
	payload = append(payload, 0x02)

	lengthData := len(args[1])
	highByte := byte(lengthData >> 8)
	lowByte := byte(lengthData & 0xFF)
	payload = append(payload, highByte)
	payload = append(payload, lowByte)

	payload = append(payload, data...)
	var chc byte
	for _, v := range []byte(data) {
	  chc ^= v
	}
	payload = append(payload, chc)
	payload = append(payload, 0x03)
	fmt.Printf("data = %#v\n",payload)

	mode := &serial.Mode{
		BaudRate: 115200,
	}
	port, err := serial.Open("/dev/ttyUSB0", mode)
	if err != nil {
		log.Fatal(err)
	}
	defer port.Close()

	go readBuffer(port)

	_, err = port.Write(payload)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Data terkirim!")
	time.Sleep(10 * time.Second)

	// fmt.Printf("raw = %s, hex = %s, length = %d, hex length = %s\n",args[1],hexData,lengthData,hexLength)
	// fmt.Printf("raw = %s, hex = %s, length = %d, hex length = %s\n",args[1],hexData,lengthData,hexLength)
}

func readBuffer(port serial.Port){
	for {
		readPayload := make([]byte, 255)
		n, err := port.Read(readPayload)
		time.Sleep(1 * time.Second)
		if err != nil {
			fmt.Println("Gagal membaca balasan:", err)
			return 
		}

		if n > 0 {
			// fmt.Printf("Data terbaca (%d byte): %#v\n", n, readPayload[:n])
			fmt.Printf("Data = %s\n",readPayload)
		}
	}
}