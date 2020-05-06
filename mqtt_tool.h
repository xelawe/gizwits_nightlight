#include "cy_mqtt.h"

char *gv_power;

const char* mqtt_pubtopic_pwr_i_suff = "tele/POWER";
String mqtt_pubtopic_pwr_i_s;
const char* mqtt_pubtopic_pwr_i;


void init_mqtt_local() {
  mqtt_pubtopic_pwr_i_s += gv_clientname;
  mqtt_pubtopic_pwr_i_s += '/';
  mqtt_pubtopic_pwr_i_s += mqtt_pubtopic_pwr_i_suff;
  mqtt_pubtopic_pwr_i = (char*) mqtt_pubtopic_pwr_i_s.c_str();
  DebugPrintln(mqtt_pubtopic_pwr_i);

  init_mqtt(gv_clientname);

}

void pub_power() {

//  char buffer[10];
//  dtostrf(gv_power, 0, 1, buffer);
//  client.publish(mqtt_pubtopic_pwr_i, buffer, true);
    client.publish(mqtt_pubtopic_pwr_i, gv_power, true);

}
