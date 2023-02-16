struct TDATA{
  byte Hedding;
  byte CarSpeed;
  byte Servo1;
  byte Servo2;
  byte Servo3;
  byte Servo4;
  byte Servo5;
  byte Servo6;
  byte ServoSpeed;
  byte CameraX;
  byte CameraY;
  };

  int index;
  TDATA data;
  byte Buffer[16];
  unsigned long actual;
  unsigned long last;
  unsigned long timeout;
  int DataSize;
  
#include <SoftwareSerial.h>

SoftwareSerial hc06(10, 11); // RX, TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Booting up...");
  hc06.begin(115200);
  while (!hc06) {}
  index=0;
  timeout=50;
  DataSize=sizeof(data);
  data.Hedding=0;
  data.CarSpeed=0;
  data.Servo1=0;
  data.Servo2=0;
  data.Servo3=0;
  data.Servo4=0;
  data.Servo5=0;
  data.Servo6=0;
  data.ServoSpeed=0;
  data.CameraX=0;
  data.CameraY=0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (hc06.available() > 0)
  {
    last=millis()+timeout;
    Buffer[index] = hc06.read();
    index++;
  }

  if (millis()>last)
  {
    index=0;
  }
  if (index>DataSize-1)
  {
  memcpy(&data, Buffer, DataSize);
  hc06.println(sizeof(data));
  hc06.println(data.Hedding);
  hc06.println(data.CarSpeed);
  hc06.println(data.Servo1);
  hc06.println(data.Servo2);
  hc06.println(data.Servo3);
  hc06.println(data.Servo4);
  hc06.println(data.Servo5);
  hc06.println(data.Servo6);
  hc06.println(data.ServoSpeed);
  hc06.println(data.CameraX);
  hc06.println(data.CameraY);
  index=0;
  Serial.println(data.Hedding);
  }
  
}
