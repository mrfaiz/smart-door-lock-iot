#include <stdio.h>
#include "contiki.h"
#include <stdio.h>
#include <string.h>
#include "sys/node-id.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"

#include "net/netstack.h"        // for sending data
#include "net/nullnet/nullnet.h" // for nullnet
#include "../utilities/utilities.h"

static linkaddr_t gateway_addr = {{0x02, 0x02, 0x02, 0x00, 0x02, 0x74, 0x12, 0x00}};

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len, const linkaddr_t *src, const linkaddr_t *dest)
{
    if (len == sizeof(pkt))
    {
        struct packet tmp;
        memcpy(&tmp, data, sizeof(tmp));
        printf("\ndevice received=>%u", tmp.status);
        if (tmp.status)
            leds_on(LEDS_RED);
        else
        {
            leds_off(LEDS_RED);
        }
    }
}

/*---------------------------------------------------------------------------*/
PROCESS(devices, "devices");
AUTOSTART_PROCESSES(&devices);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(devices, ev, data)
{
    PROCESS_BEGIN();

    /* Initialize NullNet */
    nullnet_buf = (uint8_t *)&pkt;
    nullnet_len = sizeof(pkt);
    nullnet_set_input_callback(input_callback);

    SENSORS_ACTIVATE(button_sensor);

    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
        leds_off(LEDS_RED);
        
        pkt.packet_type = 2;
        pkt.node_id = node_id;
        NETSTACK_NETWORK.output(&gateway_addr);
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/