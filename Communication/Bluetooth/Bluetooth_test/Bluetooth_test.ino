// Bluetooth Tutorial By Robo India
// Bluetooth module used - HC-06

#include <SoftwareSerial.h>
SoftwareSerial BlueTooth(19, 18); // (TXD, RXD) of HC-06

char BT_input; // to store input character received via BT.

void setup()  
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);     // Arduino Board LED Pin
  BlueTooth.begin(9600);  
}

void loop() 
{
  if (BlueTooth.available())
 
  {
    BT_input=(BlueTooth.read());
    BlueTooth.println("Return");
    BlueTooth.println(BT_input);
    Serial.println(BT_input);
    if (BT_input=='a')
    {
      digitalWrite(13, HIGH);
      BlueTooth.println("Now LED is ON");
    }
    else if (BT_input=='b')
    {
      digitalWrite(13, LOW);
      BlueTooth.println("Now LED is OFF");
    }
   else if (BT_input=='?')
    {
      BlueTooth.println("Send 'a' to turn LED ON");
      BlueTooth.println("Send 'b' to turn LED OFF");
    }   
   // You may add other if else condition here. 
  }
}
