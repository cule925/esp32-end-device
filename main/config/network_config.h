/* GENERALNE POSTAVKE */

#define ESP32_ID            "ESP32-ID0:"

/* WIFI POSTAVKE */

#define WIFI_SSID           "MILAN"
#define WIFI_PASSWORD       "milan1234"
#define WIFI_MAX_RETRY      5

/* MDNS POSTAVKE */

// TODO

/* MQTT POSTAVKE */

// Poruke su oblika: "ESP32_ID{stanje}"
#define MQTT_BROKER_HOSTNAME                    "raspberry_pi"
#define MQTT_URL_SCHEME                         "mqtt://"
#define MQTT_BROKER_HARDCODED_IP_ADDRESS        "192.168.1.26"
#define MQTT_BROKER_PORT                        ":1883"
#define MQTT_USERNAME                           "username"
#define MQTT_PASSWORD                           "password"
#define MQTT_SUB_TOPIC                          "receive/led"
#define MQTT_PUB_TOPIC                          "send/led"

