
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



void pub_sens() {

  char buffer[256];
  //dtostrf(fire.NUMFirePIXELS, 0, 0, buffer);

  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<200> jsondoc;

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument  doc(200);

  // Add values in the document
  //

  // Add the "DS1820"
  //  jsondoc["DS18B20"] = (int)(tempC * 100 + 0.5) / 100.0;
  JsonObject lv_ldr = jsondoc.createNestedObject("LDR");
  lv_ldr["Brightness"] = LDRValue;

  int n  = serializeJson(jsondoc, buffer);

  //client.publish(mqtt_pubtopic_sensor, buffer, true);
  client.publish(mqtt_GetTopic_P(gv_ptopic, mqtt_pre_tele, gv_clientname, (PGM_P)F("SENSOR")), buffer, true);
}

void init_mqtt_local() {

  init_mqtt(gv_clientname);

  delay(500);

  gv_power = 0;
  check_mqtt_reset();

  DebugPrintln("MQTT init done!");
}
