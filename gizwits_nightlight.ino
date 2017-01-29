#define serdebug
#ifdef serdebug
#define DebugPrint(...) {  Serial.print(__VA_ARGS__); }
#define DebugPrintln(...) {  Serial.println(__VA_ARGS__); }
#else
#define DebugPrint(...) { }
#define DebugPrintln(...) { }
#endif

#include "tools_wifi.h"
#include "ota_tool.h"
#include <Ticker.h>

#define btnpin 4
#define ledpinbl 13
#define ledpinrt 15
#define ledpingn 12
#define LDRPin (A0)

#define pirpin 14

#define lightpin1 16
#define lightpin2 5

int LDRValue;

// Parameter for Timing, ...
#define LDRThres 5
// Light On Time in s
#define OnTimeLight 25

boolean gv_PIR_on = false;
boolean gv_light_on = false;

const int CMD_WAIT = 0;
const int CMD_BUTTON_CHANGE = 1;
int cmd = CMD_WAIT;
int buttonState = HIGH;
static long startPress = 0;

Ticker ticker_piroff;

void set_rgb(int iv_red, int iv_green, int iv_blue, int iv_LDRvalue) {

  int lv_green = iv_green * iv_LDRvalue / 1023;
  int lv_red = iv_red * iv_LDRvalue / 1023;
  int lv_blue = iv_blue * iv_LDRvalue / 1023;

  set_rgb(lv_red, lv_green, lv_blue);

}

void set_rgb(int iv_red, int iv_green, int iv_blue) {

  int lv_green = iv_green * 0.8;
  int lv_red = iv_red;
  int lv_blue = iv_blue;

  analogWrite(ledpinrt, lv_red);
  analogWrite(ledpingn, lv_green);
  analogWrite(ledpinbl, lv_blue);
}

void toggle() {
  //  led_stat++;
  //  if (led_stat > 5) {
  //    led_stat = 0;
  //  }

}


void piroff()
{
  gv_PIR_on = false;
  ticker_piroff.detach();
}

void restart() {
  ESP.reset();
  delay(1000);
}

void reset() {
  //reset wifi credentials
  WiFi.disconnect();
  delay(1000);
  ESP.reset();
  delay(1000);
}


void IntBtn() {
  cmd = CMD_BUTTON_CHANGE;
}

void IntPIR() {
  if ( LDRValue < LDRThres ) {
    ticker_piroff.detach();
    gv_PIR_on = true;
    ticker_piroff.attach(OnTimeLight, piroff);
  }
}

void setup() {
  // put your setup code here, to run once:

#ifdef serdebug
  Serial.begin(115200);
#endif


  pinMode(ledpinrt, OUTPUT);
  pinMode(ledpingn, OUTPUT);
  pinMode(ledpinbl, OUTPUT);
  pinMode(lightpin1, OUTPUT);
  pinMode(lightpin2, OUTPUT);

  set_rgb(255, 255, 255);

  wifi_init("NightLight");

  init_ota("NightLight");

  set_rgb(0, 0, 0);

  //setup button
  pinMode(btnpin, INPUT);
  attachInterrupt(btnpin, IntBtn, CHANGE);

  //
  pinMode(pirpin, INPUT);
  attachInterrupt(pirpin, IntPIR, RISING);

  delay(500);

}

void loop() {
  // put your main code here, to run repeatedly:

  check_ota();


  int pirState = digitalRead(pirpin);

  if (gv_PIR_on) {
    analogWrite(ledpinbl, 255);
    digitalWrite(lightpin1, HIGH);
    digitalWrite(lightpin2, HIGH);
    gv_light_on = true;
  } else {
    analogWrite(ledpinbl, 0);
    digitalWrite(lightpin1, LOW);
    digitalWrite(lightpin2, LOW);
    gv_light_on = false;
  }

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
      analogWrite(ledpingn, 50);
    }
  }


  if (pirState == 1) {
    analogWrite(ledpinrt, 50);
  } else {
    analogWrite(ledpinrt, 0);
  }

  switch (cmd) {
    case CMD_WAIT:
      break;
    case CMD_BUTTON_CHANGE:
      int currentState = digitalRead(btnpin);
      if (currentState != buttonState) {
        if (buttonState == LOW && currentState == HIGH) {
          long duration = millis() - startPress;
          if (duration < 1000) {
            DebugPrintln("short press - toggle LED");
            toggle();
          } else if (duration < 5000) {
            DebugPrintln("medium press - reset");
            restart();
          } else if (duration < 60000) {
            DebugPrintln("long press - reset settings");
            reset();
          }
        } else if (buttonState == HIGH && currentState == LOW) {
          startPress = millis();
        }
        buttonState = currentState;
      }
      break;
  }








  delay(100);

}
