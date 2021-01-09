
void pub_power() {
  if ( gv_power == gv_power_old) {
    return;
  }

  char *lv_power_c = "OFF";
  if (gv_power == 0) {
    lv_power_c = "OFF";
  } else {
    lv_power_c = "ON";
  }

  if (!client.publish(mqtt_GetTopic_P(gv_ptopic, mqtt_pre_stat, gv_clientname, (PGM_P)F("POWER")), lv_power_c, true)) {
    DebugPrintln("pub failed!");
  } else {
    gv_power_old = gv_power;
    DebugPrintln("pub ok!");
  }
}

void init_mqtt_local() {

  init_mqtt(gv_clientname);

  delay(500);

  gv_power = 0;
  check_mqtt_reset();

  DebugPrintln("MQTT init done!");
}
