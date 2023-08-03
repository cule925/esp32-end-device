#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "include_config.h"

static const char* TAG = "main";

/* Ovdje se sprema IP adresa pronadjena mDNS-om*/
static char ipv4_addr[16] = {0}; 

/* Funkcije koje uredjaj treba obavljati */
static void (*event_function)(esp_mqtt_event_handle_t) = device_event_function;

/* Mrezne postavke */
void network_setup() {

    #ifdef WIFI_COMPONENT
    
        init_and_connect_wifi();
    
        #ifdef MDNS_COMPONENT
        
        start_mdns(ipv4_addr, sizeof(ipv4_addr));
        
        #endif

        #ifdef MQTT_COMPONENT
    
            #ifdef MDNS_COMPONENT
                
                init_mqtt(ipv4_addr, sizeof(ipv4_addr), event_function);
    
            #else

                init_mqtt(NULL, 0, event_function);
    
            #endif

        #endif

    #endif

}


/* Postavke */
void setup()
{

    network_setup();
    device_setup();

}

/* Ulazna tocka */
void app_main(void)
{

    ESP_LOGI(TAG, "started main");

    setup();

    #ifdef MQTT_COMPONENT

        device_function();

    #else

        while(1) vTaskDelay(10000 / portTICK_PERIOD_MS);

    #endif

}
