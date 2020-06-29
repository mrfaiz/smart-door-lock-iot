#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>
#include "sys/node-id.h"
#include "net/netstack.h"        // for sending data
#include "net/nullnet/nullnet.h" // for nullnet
#include "../utilities/utilities.h"

// static linkaddr_t dest_addr =  {{ 0x01, 0x01, 0x01, 0x00, 0x01, 0x74, 0x12, 0x00 }};
//ID:1	[INFO: Main      ] Link-layer address: 0101.0100.0174.1200
//ID:2	[INFO: Main      ] Link-layer address: 0202.0200.0274.1200

static linkaddr_t gateway_addr = {{0x02, 0x02, 0x02, 0x00, 0x02, 0x74, 0x12, 0x00}};
static unsigned short generated_packet_id;
/*-----------------------------------------------------------------------------*/

void input_callback(const void *data, uint16_t len, const linkaddr_t *src, const linkaddr_t *dest)
{
  if (len == sizeof(pkt))
  {
    struct packet tmp;
    memcpy(&tmp, data, sizeof(tmp));
    if (tmp.packet_type == 2)
    {
      printf("Door unlocked.\n");
    }
  }
}

/*---------------------------------------------------------------------------*/
PROCESS(door_switch, "door switch");
AUTOSTART_PROCESSES(&door_switch);
/*---------------------------------------------------------------------------*/
static uint8_t active;
PROCESS_THREAD(door_switch, ev, data)
{
  PROCESS_BEGIN();

  /* Initialize NullNet */
  nullnet_buf = (uint8_t *)&pkt;
  nullnet_len = sizeof(pkt);
  nullnet_set_input_callback(input_callback);
  printf("Main Door\n");
  active = 0;
  SENSORS_ACTIVATE(button_sensor);

  while (1)
  {
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
    generated_packet_id = generated_packet_id + 1;
    if (generated_packet_id > 1000)
      generated_packet_id = 1;

    pkt.status = active;
    pkt.node_id = node_id;
    pkt.packet_type = 1;
    pkt.packet_id = generated_packet_id;
    NETSTACK_NETWORK.output(&gateway_addr);
    printf("Door: Unlock signal sent\n");
    active ^= 1;
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/