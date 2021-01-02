#include "cy_mqtt_v1.h"

char *gv_power = "OFF";

const char* mqtt_pubtopic_pwr_suff = "tele/POWER";
String mqtt_pubtopic_pwr_s;
const char* mqtt_pubtopic_pwr;

void pub_power() {

  //  char buffer[10];
  //  dtostrf(gv_power, 0, 1, buffer);
  //  client.publish(mqtt_pubtopic_pwr_i, buffer, true);
  if (!client.publish(mqtt_pubtopic_pwr, gv_power, true)) {
    DebugPrintln("pub failed!");
  } else {
    DebugPrintln("pub ok!");
  }

}

void init_mqtt_local() {
  mqtt_pubtopic_pwr_s += gv_clientname;
  mqtt_pubtopic_pwr_s += '/';
  mqtt_pubtopic_pwr_s += mqtt_pubtopic_pwr_suff;
  mqtt_pubtopic_pwr = (char*) mqtt_pubtopic_pwr_s.c_str();
  DebugPrintln(mqtt_pubtopic_pwr);

  init_mqtt(gv_clientname);

  delay(500);

  gv_power = "OFF";
  pub_power();

  delay(500);
  check_mqtt_reset();

  delay(500);
  pub_power();
  check_mqtt_reset();



  DebugPrintln("MQTT init done!");
}
