// Arduino průtokoměr YF-S201

// nastavení čísel propojovacích pinů
#define pinPrutokomer 2
#define pinPreruseni 0  // 0 = digitální pin 2
// kalibrační proměnná, u senzoru YF-S201
// je to 4,5 pulzu za vteřinu pro 1 litr za minutu
const float kalibrFaktor = 4.5;
// pomocné proměnné
volatile byte pocetPulzu = 0;
float prutok = 0.0;
unsigned int prutokML = 0;
unsigned long soucetML = 0;
unsigned long staryCas = 0;

void setup() {
  // komunikace po sériové lince rychlostí 9600 baud
  Serial.begin(9600);
  // nastavení směru vstupního pinu
  pinMode(pinPrutokomer, INPUT);
  // nastavení vstupního pinu pro využití přerušení,
  // při detekci přerušení pomocí sestupné hrany (FALLING)
  // bude zavolán podprogram prictiPulz
  attachInterrupt(pinPreruseni, prictiPulz, FALLING);
}
void loop() {
  // místo pro další příkazy
  
  // pokud je rozdíl posledního uloženého času a aktuálního
  // 1 vteřina nebo více, provedeme měření
  if ((millis() - staryCas) > 1000) {
    // vypnutí detekce přerušení po dobu výpočtu a tisku výsledku
    detachInterrupt(pinPreruseni);
    // výpočet průtoku podle počtu pulzů za daný čas
    // se započtením kalibrační konstanty
    prutok = ((1000.0 / (millis() - staryCas)) * pocetPulzu) / kalibrFaktor;
    // výpočet průtoku kapaliny v mililitrech
    prutokML = (prutok / 60) * 1000;
    // přičtení objemu do celkového součtu
    soucetML += prutokML;
    // vytištění všech dostupných informací po sériové lince
    Serial.print("Prutok: ");
    Serial.print(prutok);
    Serial.print(" l/min");
    Serial.print("  Aktualni prutok: ");
    Serial.print(prutokML);
    Serial.print(" ml/sek");
    Serial.print("  Soucet prutoku: ");
    Serial.print(soucetML);
    Serial.println(" ml");
    // nulování počítadla pulzů
    pocetPulzu = 0;
    // uložení aktuálního času pro zahájení dalšího měření
    staryCas = millis();
    // povolení detekce přerušení pro nové měření
    attachInterrupt(pinPreruseni, prictiPulz, FALLING);
  }
}
// podprogram pro obsluhu přerušení
void prictiPulz() {
  // inkrementace čítače pulzů
  pocetPulzu++;
}