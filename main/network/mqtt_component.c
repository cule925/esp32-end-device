#include <stddef.h>
#include <string.h>
#include "mqtt_client.h"
#include "esp_log.h"
#include "network_config.h"
#include "mqtt_component.h"

static const char *TAG = "mqtt";

/* Handler klijenta */
esp_mqtt_client_handle_t mqtt_client_handle;

/* Funkcija koju ce uredjaj izvrsavati */
void (*device_actuator_function)(esp_mqtt_event_handle_t) = NULL;

/* Sucelje za objavljivanje */
void mqtt_publish(char topic[], char data[]) {
    esp_mqtt_client_publish(mqtt_client_handle, topic, data, 0, 0, 0);
}

/* Sucelje za pretplatu */
void mqtt_subscribe(char topic[]) {
    esp_mqtt_client_subscribe(mqtt_client_handle, topic, 0);
}

/* Handler dogadjaja */
static void event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{

    /* Hanlde-ovi */
    esp_mqtt_event_handle_t event = event_data;
    
    switch ((esp_mqtt_event_id_t)event_id) {

        case MQTT_EVENT_CONNECTED: /* U slucaju uspostave veze */
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            mqtt_subscribe(MQTT_SUB_TOPIC);
            break;

        case MQTT_EVENT_DISCONNECTED: /* U slucaju prekida veze */
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED: /* U slucaju pretplate */
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_UNSUBSCRIBED: /* U slucaju prekida pretplate */
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED: /* U slucaju objave */
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA: /* U slucaju primanja podataka */
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            if(device_actuator_function != NULL) device_actuator_function(event);
            break;

        case MQTT_EVENT_ERROR: /* U slucaju greske */
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;

        default: /* U slucaju ostalih dogadjaja */
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    
    }

}

/* Konfiguriraj i spoji se na MQTT posrednik */
static void mqtt_config_and_connect(char* url, size_t url_size)
{

    /* Konfiguriraj */
    esp_mqtt_client_config_t mqtt_config = {
        .broker.address.uri = url,
        .credentials.username = MQTT_USERNAME,
        .credentials.authentication.password = MQTT_PASSWORD,
    };

    /* Inicijaliziraj */
    mqtt_client_handle = esp_mqtt_client_init(&mqtt_config);

    /* Registriraj handler dogadjaja */
    esp_mqtt_client_register_event(mqtt_client_handle, ESP_EVENT_ANY_ID, event_handler, NULL);

    /* Zapocni rad */
    esp_mqtt_client_start(mqtt_client_handle);

}

/* Inicijaliziraj MQTT */
void init_mqtt(char* ipv4_addr, size_t ipv4_addr_size, void (*device_actuator_function_param)(esp_mqtt_event_handle_t))
{

    ESP_LOGI(TAG, "starting mqtt init.");

    /* Slozi url */
    char *url = NULL;
    size_t url_size;

    /* Dodijeli funkciju koju ce izvrsavati ovaj uredjaj */
    if(device_actuator_function_param != NULL) device_actuator_function = device_actuator_function_param;

    /* Ako nismo dali IP adresu dinamicki u programu */
    if (ipv4_addr == NULL) {
        ipv4_addr = MQTT_BROKER_HARDCODED_IP_ADDRESS;
        ipv4_addr_size = sizeof(MQTT_BROKER_HARDCODED_IP_ADDRESS); 
    }

    /* Konacna velicina URL-a + terminator */
    url_size = sizeof(MQTT_URL_SCHEME) + ipv4_addr_size + sizeof(MQTT_BROKER_PORT) + 1;

    /* Alociraj prostor za URL */
    url = (char *)malloc(sizeof(char) * url_size);

    /* Spoji sve u jedan URL*/
    sprintf(url, MQTT_URL_SCHEME "%s" MQTT_BROKER_PORT, ipv4_addr);

    /* Postavi konfiguraciju za spajanje i vrati handle */
    mqtt_config_and_connect(url, url_size);

    ESP_LOGI(TAG, "mqtt init finished.");

}
