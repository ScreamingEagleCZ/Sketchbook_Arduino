// AWS IOT Device SDK - Version: Latest 


/*
Mk1 - Flowmeter + solenoid
*/

//Libraries----------------------
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Setup of display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Define Pins--------------------
#define FlowPin 2
#define InterruptPin 0 // digital pin 2 = interrupt pin 0
#define RelayPin 12

//Variables----------------------
//Flowmeter
const float CalibValue = 4.5; // 4.5 pulses/s = 1l/s for FlowMeter
float CalibPulse = 504; // pulses per 1 liter
unsigned long InputValue = 500; //shows this value upon booting
unsigned long PulsesCount = 0;
unsigned long DesiredVolume = InputValue * (CalibPulse / 1000); //used for counting volume
float MlPerPulse = 2.2222;  //1 pulse = 2.2222ml
unsigned int Counter = 0;
float flow = 0.0;
unsigned int flowML = 0;
unsigned long soucetML = 0;
unsigned long oldTime = 0;
//Keyboard
const byte rows = 4;
const byte columns = 4;
//field
char keys[rows][columns] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
//set of pin numbers for connection with keyboard
byte RowPin[rows] = {4,5,6,7};
byte ColumnPin[columns] = {8,9,10,11};
//keyboard instance from keypad library
Keypad keyboard = Keypad( makeKeymap(keys), RowPin, ColumnPin, rows, columns);

//Setup ---------------------------------------
void setup() {
  //initialization of LCD
  lcd.init();
  lcd.backlight(); //switch on of display illumination
  lcd.print("Pripraveno...");
  //Comunication over serial 9600 baud
  Serial.begin(9600);
  //setup of pins
  pinMode(FlowPin,INPUT);
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW);
  //input pin for interrupt, calling subFunction addPulse upon FALLING edge
  attachInterrupt(InterruptPin, addPulse, FALLING);
  lcd.setCursor(0,1);
  lcd.print(InputValue);
  lcd.print("ml/");
  lcd.print(int(CalibPulse));
  lcd.print("p/l          ");
  delay(1000);
}
//Loop -------------------------------------------
void loop() {
  //load pressed key to variable
  
  // Desired volume reached
  if (PulsesCount > DesiredVolume){
    digitalWrite(RelayPin, LOW);
    Counter++;
    PulsesCount = 0;
    //Serial.println("Hotovo");
    //Serial.println("Pocet: ");
    //Serial.print(Counter);
    lcd.setCursor (0,0);
    lcd.print("Plneni: ");
    lcd.setCursor (8,0);
    lcd.print(Counter);
    lcd.print("          ");
  }
  // "*" - pressed (Setting up of Volume)
  char key = keyboard.getKey();
  if (key == '*'){
    InputValue = GetNumber(); //in mililitres
    DesiredVolume = InputValue * (CalibPulse / 1000);  
    //Serial.println(InputValue);
    //Serial.println(DesiredVolume);
    lcd.setCursor(0,1);
    lcd.print(InputValue);
    lcd.print("ml/");
    lcd.print(int(CalibPulse));
    lcd.print("p/l        ");
  }
  // "A" - pressed (Start of dose)
  if (key == 'A'){
    //Serial.println("Start");
    digitalWrite(RelayPin, HIGH);
    PulsesCount = 0;
    lcd.setCursor (0,0);
    lcd.print("Start...        ");
  }
  // "B" - pressed (Stop of dosing)
  if (key == 'B'){
    //Serial.println("Force Stop");
    digitalWrite(RelayPin, LOW);
    PulsesCount = 0;
  }
  // "C" - pressed (Calibration pulse setting)
  if (key == 'C'){
    CalibPulse = GetNumber();
    DesiredVolume = InputValue * (CalibPulse / 1000);
    //Serial.println(CalibPulse);
    lcd.setCursor(0,1);
    lcd.print(InputValue);
    lcd.print("ml/");
    lcd.print(int(CalibPulse));
    lcd.print("p/l        ");
  }
  // "D" - pressed (Reset to default)
  if (key == 'D'){
    CalibPulse = 450;
    DesiredVolume = 225;  //225 = 500ml with 450pulses/liter
    InputValue = 500; //for showing on LCD
    Counter = 0;
    //Serial.println("Reset hodnot");
    //Serial.println(CalibPulse);
    //Serial.println(DesiredVolume);
    lcd.setCursor (0,0);
    lcd.print("Plneni: ");
    lcd.setCursor (8,0);
    lcd.print(Counter);
    lcd.setCursor(0,1);
    lcd.print(InputValue);
    lcd.print("ml/");
    lcd.print(int(CalibPulse));
    lcd.print("p/l        ");
  }
  
}


//subFunctions
void addPulse() {
  PulsesCount++;
}

unsigned long GetNumber()
{
   unsigned long num = 0;
   char key = keyboard.getKey();
   while(key != '#')
   {
      switch (key)
      {
         case NO_KEY:
            break;

         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
            //lcd.print(key);
            //Serial.println(key);
            num = num * 10 + (key - '0');
            if (num > 1000000000){
              num = 1000000000;
            }
            //Serial.println(num);
            break;

         case '*':
            num = 0;
            //lcd.clear();
            break;
      }

      key = keyboard.getKey();
   }

   return num;
}