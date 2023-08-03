# ESP32 KRAJNJI UREĐAJ

Jednostavni IoT krajnji uređaj pomoću ESP32 mikroupravljača. Izvorni kod se nalazi u `main` direktoriju,  
ulazna točka je funkcija `main()` u `main.c` datoteci. Konfiguracijska zaglavlja u direktoriju `config`  
i dijele se na konfiguracije za uključivanje `include_config.h` i mrežne konfiguracije `network_config.h`.  
Direktorij `include` sadržava sva potrebna zaglavlja za sve datoteke. Direktorij `network` sadržava  
izvorni kod za povezivanje na WiFi mrežu te uspostava veze na MQTT posrednik. Direktorij `device_role`  
označava ulogu uređaja i uređaj će moći poprimiti samo jednu ulogu (osigurano `include_config.h`  
zaglavljem).

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

### POKRETANJE SERIJSKOG MONITORA PREKO TERMINALA

S Baud rate-om od 115200 Bd:
```
/home/tether/.espressif/python_env/idf[VERZIJA]_py3.10_env/bin/python /home/tether/esp/esp-idf/tools/idf_monitor.py -p /dev/ttyUSB[BROJ] -b 115200
```
