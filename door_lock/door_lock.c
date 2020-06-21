#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(test_button_process, "Test button");
AUTOSTART_PROCESSES(&test_button_process);
/*---------------------------------------------------------------------------*/
static uint8_t active;
PROCESS_THREAD(test_button_process, ev, data)
{
  PROCESS_BEGIN();
  active = 0;
  SENSORS_ACTIVATE(button_sensor);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event &&data == &button_sensor);
    // leds_toggle(LEDS_ALL);
    if(!active) {
      /* activate light sensor */

      printf("Light: %u\n", active);
    } else {
      /* deactivate light sensor */
      printf("Light: %u\n", active);

    }
    active ^= 1;
    // leds_toggle(LEDS_ALL);
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/