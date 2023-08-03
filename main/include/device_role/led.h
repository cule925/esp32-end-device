#ifndef __DUMMY__
#define __DUMMY__

#include "mqtt_client.h"
#include "driver/gpio.h"

void device_setup();
void device_function();
void device_event_function(esp_mqtt_event_handle_t);

#define LED_PIN                     GPIO_NUM_4
#define STATE_PUBLISH_DELAY         1000

#endif