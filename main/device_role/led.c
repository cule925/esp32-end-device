#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "string.h"

/* Obavezna zaglavlja za sve uloge*/
#include "network_config.h"
#include "mqtt_component.h"

/* Obavezno zaglavlje za svaku ulogu zasebno */
#include "led.h"

/* Pamtitelj stanja */
int state = 0;

/* Tri obavezne funkcije: device_setup(), device_function(), device_event_function() */

/* Pocetne postavke uredjaja, primjerice pretplata na neku temu */
void device_setup() {

    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    state = 0;
    gpio_set_level(LED_PIN, 0);

}

/* Normalna funkcija uredjaja, ono sto ce se pokretati nakon postavljanja svih postavki */
void device_function() {

    /* Objavi svoje stanje */
    char message[20];
    while(1) {
        sprintf(message, ESP32_ID "%d", state);
        mqtt_publish(MQTT_PUB_TOPIC, message);
        vTaskDelay(STATE_PUBLISH_DELAY / portTICK_PERIOD_MS);
    }

}

/* Izvuci naredbu i LED diodu */
void parse_value(char* message, size_t id_size, size_t size) {

    /* Pomakni pokazivac na mjesto vrijednosti */
    char* value = message + id_size;
    char* end_ptr;

    /* Parsiraj vrijednost u cijeli broj */
    int level = strtol(value, &end_ptr, 10); // Dekadska baza

    /* Provjeri jeli bilo gresaka */
    if((end_ptr == value) || ((level != 0) && (level != 1)))
        return;

    /* Postavi izlaz LED-ice */
    gpio_set_level(LED_PIN, level);
    state = level;

}

/* Funkcija uredjaja pri nekom MQTT dogadjaju */
void device_event_function(esp_mqtt_event_handle_t event) {

    /* Poruka koja je dosla na temu pretplate */
    char* data = event->data;
    size_t size = event->data_len;

    char* message = malloc(sizeof(char) * (size + 1));
    strncpy(message, data, size);
    message[size] = '\0';

    /* Ako je poruka manja nego sami ID */
    size_t id_size = sizeof(ESP32_ID) - 1; // Bez '\0'

    /* Provjeri jeli poruka namjenjena ovom ESP-u */
    if(size > id_size)
        if(strncmp(ESP32_ID, message, id_size) == 0)
            parse_value(message, id_size, size);

    free(message);

}