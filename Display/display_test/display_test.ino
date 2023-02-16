// LCD displej pres I2C
// navody.arduino-shop.cz

// knihovny pro LCD přes I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// nastavení adresy I2C (0x27 v mém případě),
// a dále počtu znaků a řádků LCD, zde 20x4
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  // inicializace LCD
  lcd.init();
  // zapnutí podsvícení
  lcd.backlight();
  // vytisknutí hlášky na první řádek
  lcd.print("->arduino-shop.cz<-");
  // nastavení kurzoru na první znak, druhý řádek
  // veškeré číslování je od nuly, poslední znak je tedy 19, 3
  lcd.setCursor ( 0, 0 );
  lcd.print("--------------------");
  lcd.setCursor ( 0, 1);
  lcd.print(" Test LCD pres I2C");
  lcd.setCursor ( 15, 1);
  lcd.print("!");
  delay(2000);
}

void loop()
{
  // nastavení kurzoru na devátý znak, druhý řádek
  lcd.setCursor(8, 1);
  // vytisknutí počtu sekund od začátku programu
  lcd.print(millis() / 1000);
}