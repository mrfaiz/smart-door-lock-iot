#include <stdio.h>
#include "contiki.h"
#include <stdio.h>
#include <string.h>
#include "net/netstack.h"        // for sending data
#include "net/nullnet/nullnet.h" // for nullnet
#include "../utilities/utilities.h"

/*----------------------------------------------------------------------------*/

static linkaddr_t door_switch_addr = {{ 0x01, 0x01, 0x01, 0x00, 0x01, 0x74, 0x12, 0x00 }};

//ID:3	[INFO: Main      ] Link-layer address: 0303.0300.0374.1200
// static linkaddr_t device1_addr = {{ 0x03, 0x03, 0x03, 0x00, 0x03, 0x74, 0x12, 0x00 }};

//ID:4	[INFO: Main      ] Link-layer address: 0404.0400.0474.1200
// static linkaddr_t device2_addr = {{ 0x04, 0x04, 0x04, 0x00, 0x04, 0x74, 0x12, 0x00 }};

//ID:5	[INFO: Main      ] Link-layer address: 0505.0500.0574.1200
// static linkaddr_t device3_addr = {{ 0x05, 0x05, 0x05, 0x00, 0x05, 0x74, 0x12, 0x00 }};

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len, const linkaddr_t *src, const linkaddr_t *dest)
{
  if (len == sizeof(pkt))
  {
    struct packet tmp;
    memcpy(&tmp, data, sizeof(tmp));
    printf("\ngateway received=>%u", tmp.status);
    pkt = tmp;
    if (tmp.packet_type == 1)
    {
      // NETSTACK_NETWORK.output(&device3_addr);
      // NETSTACK_NETWORK.output(&device1_addr);
      // NETSTACK_NETWORK.output(&device2_addr);
      NETSTACK_NETWORK.output(NULL);
    }
    else
    {
      NETSTACK_NETWORK.output(&door_switch_addr);
    }
  }
}

/*---------------------------------------------------------------------------*/
PROCESS(gateway, "gateway");
AUTOSTART_PROCESSES(&gateway);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(gateway, ev, data)
{
  PROCESS_BEGIN();

  /* Initialize NullNet */
  nullnet_buf = (uint8_t *)&pkt;
  nullnet_len = sizeof(pkt);
  nullnet_set_input_callback(input_callback);
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/