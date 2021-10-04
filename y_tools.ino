
void set_rgb(int iv_red, int iv_green, int iv_blue) {

  int lv_green = map(iv_green, 0, 255, 0, PWMRANGE) * 0.8;
  int lv_red = map(iv_red, 0, 255, 0, PWMRANGE);
  int lv_blue = map(iv_blue, 0, 255, 0, PWMRANGE);

  analogWrite(ledpinrt, lv_red);
  analogWrite(ledpingn, lv_green);
  analogWrite(ledpinbl, lv_blue);
}

void piroff() {
  gv_PIR_on = false;
  ticker_piroff.detach();
}

void ICACHE_RAM_ATTR tick_send_mqtt() {
  gv_tick_send_mqtt = true;
}

void ICACHE_RAM_ATTR IntPIR() {
  ticker_piroff.detach();

  if (  gv_ldr_ok == true ) {
    // --> Turn On Light
    digitalWrite(lightpin1, HIGH);
    digitalWrite(lightpin2, HIGH);
    gv_light_on = true;
  }

  gv_PIR_on = true;
  ticker_piroff.attach(OnTimeLight, piroff);
}
