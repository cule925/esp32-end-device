#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "network_config.h"
#include "wifi_component.h"

/* Signalizacija dogadjaja grupe */
static EventGroupHandle_t wifi_event_group;

/* Dva bitna dogadjaja, spojen si na WiFi i neuspjeh spajanja na WiFi mrežu nakon zadanog broja pokušaja */
#define WIFI_CONNECTED_BIT              BIT0
#define WIFI_CONNECTION_FAILED_BIT      BIT1

/* Oznaka za debug */
static const char *TAG = "wifi station";

/* Broj ponovnih pokusaja */
static int retry_num = 0;

/* Handler dogadjaja */
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    
    /* Pokusaj se spojiti na WiFi pristupnu tocku u slucaju nekog WiFi dogadjaja ili pocetka rada stanice */
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    
        esp_wifi_connect();
    
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {

        /* Ponovi spajanje ako nisi presao broj ponavljanja */
        if (retry_num < WIFI_MAX_RETRY) {

            esp_wifi_connect();
            retry_num++;
            ESP_LOGI(TAG, "retrying to connect to the AP");
        
        } else {
            
            /* U prelaska broja ponavljanja, postavi bit greske */
            xEventGroupSetBits(wifi_event_group, WIFI_CONNECTION_FAILED_BIT);
        
        }
        
        ESP_LOGI(TAG,"connection to the AP failed");

    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        
        /* U slucaju IP dogadjaja, */
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));

        /* Resetiraj broj pokusaja */
        retry_num = 0;

        /* Postavi bit da smo spojeni na WiFi */
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    
    }

}

/* Inicijalizira nacin rada WiFi stanice (uobicajen nacin povezivanja) */
void init_wifi_station_mode()
{

    /* Inicijalizacije grupe dogadjaja */
    wifi_event_group = xEventGroupCreate();

    /* Inicijaliziraj TCP/IP stog */
    ESP_ERROR_CHECK(esp_netif_init());

    /* Stvori petlju dogadjaja */
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* Inicijaliziraj uobičajeni način WiFi stanice */
    esp_netif_create_default_wifi_sta();

    /* Vrati uobicajenu konfiguraciju */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    /* Alociraj resurse za WiFi upravljacki program */
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    /* Handler-i */
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    /* U slucaju WiFi dogadjaja, pozovi handler (registracija funkcija za pojedine dogadjaje) */
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,       /* Autentifikacija WPA2 PSK */
            .pmf_cfg.capable = true,                        /* Koristi PMF (Protected Management Frames) kako bi povecao sigurnost */
            .pmf_cfg.required = false,                      /* Koristi PMF kad mozes */
        },
    };

    /* Postavi nacin rada WiFi stanice */
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );

    /* Postavi konfiguraciju */
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    
    /* Zapocni rad WiFi-ja pomocu trenutno postavljene konfiguracije */
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "init_wifi_station_mode finished.");

    /* Cekanje hoce li se postaviti bilo koji bit (blokirajuca funkcija) */
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_CONNECTION_FAILED_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    /* Ispitivanje stanje bitova */
    if (bits & WIFI_CONNECTED_BIT) ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", WIFI_SSID, WIFI_PASSWORD);
    else if (bits & WIFI_CONNECTION_FAILED_BIT) ESP_LOGI(TAG, "failed to connect to SSID:%s, password:%s", WIFI_SSID, WIFI_PASSWORD);
    else ESP_LOGE(TAG, "UNEXPECTED EVENT");

}

/* Inicijalizira flash */
void init_nonvolatile_storage()
{

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

}

/* Spoji se na WiFi mrezu */
void init_and_connect_wifi(void)
{
    
    init_nonvolatile_storage();
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    init_wifi_station_mode();

}
