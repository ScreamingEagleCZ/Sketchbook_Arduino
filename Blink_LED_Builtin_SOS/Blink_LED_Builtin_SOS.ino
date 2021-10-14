void setup() {
  // put your setup code here, to run once:
pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  s_morse();
  o_morse();
  s_morse();
}
void s_morse(){
  for (int i = 0; i < 3; i++){
    digitalWrite(LED_BUILTIN, LOW);
    delay(600);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
  }
  delay(500);
}

void o_morse(){
  for (int i = 0; i < 3; i++){
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
  }
  delay(500);
}
