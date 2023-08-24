# ESP32 KRAJNJI UREĐAJ

Jednostavni IoT krajnji uređaj pomoću ESP32 mikroupravljača. Izvorni kodovi nalaze se u `main` direktoriju.

### RAZVOJNO OKRUŽENJE I POKRETANJE KODA NA PLOČICI

Razvojno okruženje koje se ovdje koristilo je *Visual Studio Code* te proširenje *Espressif IDE*. Prije  
nego li se kod prenese u flash memoriju pločice, potrebno je odabrati ciljni uređaj te port. Ciljni uređaj  
je ESP32, a port je oblika */dev/ttyUSB[BROJ]*. Međutim, potrebno je omogućiti dopuštenja čitanja i pisanja  
na taj port naredbom:  

```
sudo chmod a+rw /dev/ttyUSB[BROJ]
```

### POSTAVLJANJE BAUD RATE-A ZA KOMUNIKACIJU

U datoteci `settings.json` potrebno je postaviti Baud rate dodajući liniju `"idf.monitorBaudRate": "115200",`  
kako serijski monitor ne bi ispisivao smeće.

### ISKLJUČIVANJE PODRŠKE ZA MULTIPROCESORE

Kako je projekt izgrađen na najjednostavniji način, podrška za više procesora je dopuštena. Međutim, trenutna  
konfiguracija projekta to ne dopušta. Potrebno je urediti datoteku `sdkconfig` omogućujući jednojezgreni  
način rada postavljajući zastavicu `CONFIG_FREERTOS_UNICORE=y`.