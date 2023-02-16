// Arduino CNC Shield + Driver A4988
// + krokový motor 28BYJ-48
// navody.arduino-shop.cz

// nastavení čísel projovacích pinů
#define ENABLE 8
#define X_STEP 2
#define Y_STEP 3
#define Z_STEP 4
#define X_DIR  5
#define Y_DIR  6
#define Z_DIR  7

void setup() {
  // nastavení směrů pro všechny piny
  pinMode(X_STEP, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(Z_STEP, OUTPUT);
  pinMode(X_DIR,  OUTPUT);
  pinMode(Y_DIR,  OUTPUT);
  pinMode(Z_DIR,  OUTPUT);
  pinMode(ENABLE, OUTPUT);
  // povolení řízení pro všechny drivery
  digitalWrite(ENABLE, LOW);
}

void loop() {
  // posun osy X v jednom směru o půl otáčky
  // (2048 kroků na celou otáčku)
  pohybOsy(false, X_DIR, X_STEP, 1024);
  delay(1000);
  // posun osy X ve druhém směru o půl otáčky
  pohybOsy(true,  X_DIR, X_STEP, 1024);
  delay(1000);
}

void pohybOsy(boolean smer, byte dirPin, byte stepPin, int kroky) {
  // zápis směru na příslušný pin DIR
  digitalWrite (dirPin, smer);
  delay(50);
  // smyčka pro provedení předaného množství kroků
  for (int i = 0; i < kroky; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds (800);
    digitalWrite(stepPin, LOW);
    delayMicroseconds (8000);
  }
}