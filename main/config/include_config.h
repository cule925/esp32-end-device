#ifndef __CONF__
#define __CONF__

/* ODKOMENTIRAJ|ZAKOMENTIRAJ */

/* WiFi komponenta */
#define WIFI_COMPONENT

/* MDNS komponenta */
// #define MDNS_COMPONENT

/* MQTT komponenta */
#define MQTT_COMPONENT

/* ULOGA 
    0 -> paljenje i gasenje LED-ice preko MQTT-a
    ...
*/

#define ROLE        0

/* ------------------------------NE UREDJIVATI--------------------------------------- */

#if ROLE == 0
    #include "led.h"
#elif ROLE == 1                 /* Neka druga fja */
    // TODO
#elif ROLE == 2
    // TODO
#elif ROLE == 3
    // TODO
    // ...
#endif

#ifdef WIFI_COMPONENT
    #include "wifi_component.h"
#endif

#ifdef MDNS_COMPONENT
    #include "mdns_component.h"
#endif

#ifdef MQTT_COMPONENT
    #include "mqtt_component.h"
    
#endif

#endif