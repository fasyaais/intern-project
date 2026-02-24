# TASK MINGGU 4
- [ ] Demonstrate a stack overflow condition.
- [ ] Capture and analyze panic (Guru Meditation) logs.
- [ ] Measure heap usage during runtime.
- [ ] Implement a fix and explain root cause.

## MEMORY AND CRASH HANDLING
Pada minggu ini akan melakukan mempelajari memori dan jenis-jenis crash pada esp idf.

### Stack Overflow
Stack overflow merupakan kondisi ketika suatu task melebihi stack yang telah di tentukan.

![Stack Overflow](./img/stack_overflow.png)
Pada task LED Blink terjadi stack overflow dikarenakan size untuk task kurang/habis, untuk mengatasi hal ini dengan cara menaikkan kapasitas dari task tersebut.
   

### Load Access Fault (LoadProhibited)
CPU exception ini terjadi karena suatu aplikasi mencoba membaca dari atau menulis ke lokasi memori yang tidak valid.

![Kode Load fault](./img/load_fault_code.png)

Error terminal
![Error Load fault](./img/load_fault.png)

Hal ini terjadi karena variable load mencoba mengambil nilai dari variable buff yang bernilai sebuah nullptr bukan array.

### Instruction Access Fault (InstrFetchProhibited)
