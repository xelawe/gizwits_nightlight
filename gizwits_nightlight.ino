#include <cy_serdebug.h>
#include <cy_serial.h>

const char *gc_hostname = "Nightlight";
#include "cy_wifi.h"
#include "cy_ota.h"
#include <Ticker.h>

#define ledpinbl 13
#define ledpinrt 15
#define ledpingn 12
#define LDRPin (A0)

#define pirpin 14

#define lightpin1 16
#define lightpin2 5

// Parameter for Timing, ...
#define LDRThres 30
// Light On Time in s
#define OnTimeLight 25

int LDRValue;
volatile boolean  gv_ldr_ok = false;

volatile boolean gv_PIR_on = false;

volatile boolean gv_light_on = false;

volatile boolean gv_tick_send_mqtt = false;

Ticker ticker_piroff;
Ticker send_mqtt;
