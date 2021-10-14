/*
  TinyPico RGB Color Cycling demo

  https://wokwi.com/arduino/projects/308012505806930496
*/

#include <TinyPICO.h>

TinyPICO tp = TinyPICO();

void setup() {
  Serial.begin(115200);
  Serial.println("Hi, TinyPico!");
  Serial.println("--");
  Serial.print("Total PSRAM:");
  Serial.println(ESP.getPsramSize());
  Serial.print("Free PSRAM: ");
  Serial.println(ESP.getFreePsram());

  tp.DotStar_Clear();
  tp.DotStar_SetBrightness(255);
}

void loop() {
  tp.DotStar_CycleColor(25);
  delay(10); // speeds up simulation
}
