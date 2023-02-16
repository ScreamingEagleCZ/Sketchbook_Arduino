const int LED = LED_BUILTIN;
int shortP = 250;
int longP = 500;
int waitP = 200;
int delayBetweenChar = 500;
String x = "";
String y = "";


void setup() {
  // put your setup code here, to run once:

pinMode(LED, OUTPUT);


Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void sb(int x){
  digitalWrite(x, HIGH);
  delay(shortP);
  digitalWrite(x, LOW);
  delay(waitP);
  Serial.println(".");
}

void lb(int x){
  digitalWrite(x, HIGH);
  delay(longP);
  digitalWrite(x, LOW);
  delay(waitP);
  Serial.println("-");
}

void del(String x, String y){
  delay(delayBetweenChar);
  Serial.println(" " + x + " " + y);
}

void A(){
  sb(LED);
  lb(LED);
  del("A", "Alpha");
}

void B(){
  lb(LED);
  sb(LED);
  sb(LED);
  sb(LED);
  del("B", "Bravo");
}
