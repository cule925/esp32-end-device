#ifndef __MQTT__
#define __MQTT__

#include <stddef.h>
#include "mqtt_client.h"

void init_mqtt(char*, size_t, void (*)(esp_mqtt_event_handle_t));
void mqtt_publish(char topic[], char data[]);
void mqtt_subscribe(char topic[]);

#endif
