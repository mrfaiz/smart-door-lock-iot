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

/*---------------------------------------------------------------------------*/

static linkaddr_t gateway_addr = {{0x02, 0x02, 0x02, 0x00, 0x02, 0x74, 0x12, 0x00}};
static unsigned short received_packet_id = -1;

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len, const linkaddr_t *src, const linkaddr_t *dest)
{
    if (len == sizeof(pkt))
    {
        struct packet tmp;
        memcpy(&tmp, data, sizeof(tmp));
        pkt = tmp;
        if (tmp.packet_type == 1)
        {
            printf("Device: Unlock signal found\n");
            if (tmp.status)
                leds_on(LEDS_RED);
            else
            {
                leds_off(LEDS_RED);
            }
            received_packet_id = tmp.packet_id;
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
    printf("IoT device\n");
    SENSORS_ACTIVATE(button_sensor);

    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
        if (received_packet_id > 0)
        {
            leds_off(LEDS_RED);
            pkt.packet_type = 2;
            pkt.node_id = node_id;
            pkt.packet_id = received_packet_id;
            NETSTACK_NETWORK.output(&gateway_addr);
            printf("Device: Unlock sent\n");
            received_packet_id = -1;
        }
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/