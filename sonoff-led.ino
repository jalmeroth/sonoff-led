#include <Homie.h>

#define PIN_BUTTON 0
#define PIN_LED 13
#define PIN_COLD 12
#define PIN_WARM 14

#define FW_NAME "sonoff-led"
#define FW_VERSION "1.0.0"

int sliderPoints[] = {0, 25, 38, 40, 61, 85, 103, 117, 130, 141, 150, 159, 167, 174, 180, 186, 192, 197, 202, 207, 211, 255};
int counter = 0;
int incrAmount = 1;
int warmValue = 0;
int coldValue = 0;
int delayValue = 0;

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

HomieNode warmNode("warm", "light");
HomieNode coldNode("cold", "light");

bool warmHandler(String value) {
  int val = value.toInt();
  Serial.print("warmValue: "); Serial.println(sliderPoints[val]);
  warmValue = sliderPoints[val];
  return true;
}

bool coldHandler(String value) {
  int val = value.toInt();
  Serial.print("coldValue: "); Serial.println(sliderPoints[val]);
  coldValue = sliderPoints[val];
  return true;
}

void setupHandler() {
  //
  Homie.setNodeProperty(warmNode, "val", String(warmValue), true);
  Homie.setNodeProperty(coldNode, "val", String(coldValue), true);
}

void loopHandler() {
  // set the brightness of pin 9:
  analogWrite(PIN_COLD, coldValue);
  analogWrite(PIN_WARM, warmValue);
}

void setup() {
  pinMode(PIN_WARM, OUTPUT);
  pinMode(PIN_COLD, OUTPUT);
  Homie.setLedPin(PIN_LED, HIGH);
  Homie.setFirmware(FW_NAME, FW_VERSION);
  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);
  Homie.setResetTrigger(PIN_BUTTON, LOW, 5000);
  warmNode.subscribe("val", warmHandler);
  coldNode.subscribe("val", coldHandler);
  Homie.registerNode(warmNode);
  Homie.registerNode(coldNode);
  Homie.setup();
}

void loop() {
  Homie.loop();
}

