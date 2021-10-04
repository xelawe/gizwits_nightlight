
void setup() {
  // put your setup code here, to run once:

  cy_serial::start(__FILE__);

  pinMode(ledpinrt, OUTPUT);
  pinMode(ledpingn, OUTPUT);
  pinMode(ledpinbl, OUTPUT);

  pinMode(lightpin1, OUTPUT);
  pinMode(lightpin2, OUTPUT);

  set_rgb(255, 255, 255);

  wifi_init(gc_hostname);

  init_ota(gv_clientname);

  init_mqtt_local();

  set_rgb(0, 0, 0);

  pinMode(pirpin, INPUT);
  attachInterrupt(pirpin, IntPIR, RISING);
  send_mqtt.attach(60, tick_send_mqtt);

  delay(500);

  DebugPrintln("Setup init done!");
}

void loop() {

  check_ota();

  check_mqtt_reset();

  if ( !gv_light_on ) {

    // Lights off: measure LDR
    // so turn off all LEDs
    analogWrite(ledpingn, 0);
    analogWrite(ledpinrt, 0);
    analogWrite(ledpinbl, 0);
    delay(100);

    // now measure LDR
    LDRValue = analogRead(LDRPin);

    // show result of measurement
    if ( LDRValue < LDRThres ) {
      gv_ldr_ok = true;
      analogWrite(ledpingn, 10);
    } else {
      gv_ldr_ok = false;
    }
  }

  int pirState = digitalRead(pirpin);

  if (pirState == 1) {
    analogWrite(ledpinrt, 50);
  } else {
    analogWrite(ledpinrt, 0);
  }

  if (gv_PIR_on) {
    // PIR trigged, is LDR OK?
    if (  gv_ldr_ok == true  or gv_light_on ) {
      // --> Turn On Light
      analogWrite(ledpinbl, 255);
      digitalWrite(lightpin1, HIGH);
      digitalWrite(lightpin2, HIGH);
      if ( !gv_light_on ) {
        gv_light_on = true;
      }
      gv_power = 1;
    }

  } else {
    analogWrite(ledpinbl, 0);
    digitalWrite(lightpin1, LOW);
    digitalWrite(lightpin2, LOW);
    if ( gv_light_on ) {
      gv_light_on = false;
    }
    gv_power = 0;
  }

  pub_power();
  
  if (gv_tick_send_mqtt) {
    pub_sens();
    gv_tick_send_mqtt = false;
  }
}
