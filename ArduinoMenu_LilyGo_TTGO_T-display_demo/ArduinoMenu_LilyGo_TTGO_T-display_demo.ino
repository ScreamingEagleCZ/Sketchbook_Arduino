

#include <TFT_eSPI.h>

#include <Arduino.h>

//#include "Platform.h"
//#include "Settimino.h"

#include <SPI.h>
#include <SD.h>
const int chipSelect = 33;

// Uncomment next line to perform small and fast data access
#define DO_IT_SMALL

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0F, 0x08, 0xE1 };

IPAddress Local(192,168,250,90); // Local Address
IPAddress PLC(192,168,250,10);   // PLC Address

// Following constants are needed if you are connecting via WIFI
// The ssid is the name of my WIFI network (the password obviously is wrong)
char ssid[] = "dlink-81C8";    // Your network SSID (name)
char pass[] = "cttlr11298";  // Your network password (if any)
IPAddress Gateway(192, 168, 178, 254);
IPAddress Subnet(255, 255, 255, 0);

int DBNum = 9999; // This DB must be present in your PLC
byte Buffer[1024];

//S7Client Client;

unsigned long Elapsed; // To calc the execution time

/***************************************************************************************************************************************
LILYGO TTGO T-DISPLAY DEMO
Created at August 29th, 2019 by Jeroen Maathuis (j [dot] maathuis [at] gmail [dot] com) to support the LilyGo TTGO T-display

Based on:
- ArduinoMenu TFT_eSPI example on https://github.com/neu-rah/ArduinoMenu/blob/master/examples/adafruitGfx/eTFT/TFT_eSPI/TFT_eSPI.ino.
- TTGO T-display example on https://github.com/Xinyuan-LilyGO/TTGO-T-Display/blob/master/TTGO-T-Display.ino.
- Several Button2 examples on https://github.com/LennartHennigs/Button2/tree/master/examples

External needed library's:
- TFT_eSPI
- Buttons2

Additional needed files:
- bmp.h (should be found at the same place as this sketch file)

Preparations:
- Create TFT_eSPI User Setup file as described on https://github.com/Xinyuan-LilyGO/TTGO-T-Display/blob/master/README.MD and
  https://sites.google.com/site/jmaathuis/arduino/lilygo-ttgo-t-display-esp32/arduinomenu-demo-ttgo-t-display
- Attach a led (and a resistor if needed). This is optional.

This demo can also work on other setups with displays supported by the TFT_eSPI library. This demo is explicitly for the
TTGO T-display but I had a slightly modifies version working on a Wemos D1 mini with attached ST7735 display and some image
and text modifications as well another User Setup file for TFT_eSPI.

For a more detailed description about this demo please visit:
https://sites.google.com/site/jmaathuis/arduino/lilygo-ttgo-t-display-esp32/arduinomenu-demo-ttgo-t-display

This sketch may freely redistributed as long as you mention the sources / authors mentioned above. I would be very happy to here from
you when you have improved the sketch. You can mail me at j [dot] maathuis [at] gmail [dot] com.

****************************************************************************************************************************************/
#define PROG_VERSION "0.2" // Don't change this!

//#include <SPI.h>
#include <TFT_eSPI.h>
//#include <Button2.h>
#include "HahnLogo.h"

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

#define TFT_BL          4  // Display backlight control pin

#define LED_CTRL        0 // Pinnumber of external led to control. Set to 0 if there isn't a external led attached.

int ledBacklight = 80; // Initial TFT backlight intensity on a scale of 0 to 255. Initial value is 80.

#define BTN_UP 35 // Pinnumber for button for up/previous and select / enter actions (don't change this if you want to use the onboard buttons)
#define BTN_DWN 0 // Pinnumber for button for down/next and back / exit actions (don't change this if you want to use the onboard buttons)

//#include <menu.h>
//#include <menuIO/serialIO.h>
//#include <menuIO/TFT_eSPIOut.h>
// #include <menuIO/chainStream.h>
//#include <menuIO/esp8266Out.h>//must include this even if not doing web output...

//using namespace Menu;

#define ARRAYSIZE 11
String PLCModes[ARRAYSIZE] = { "  Machine off    ", "emergency  stop not ok", "safety door circuit not ","machine not ready for operation","    manual mode    ", "      setup mode     ","automatic selected","automatic activ","emptyrun activ","automatic stop request","move to homeposition"};
int PLCModesColor[ARRAYSIZE] = {0,0,1,2,2,2};
int IndexMode;







TFT_eSPI gfx = TFT_eSPI(); // Define TFT_eSPI object with the size of the screen: 135 pixels width and 240 pixels height. We will rotate it later a quarter clockwise.

//Button2 btnUp(BTN_UP); // Initialize the up button
//Button2 btnDwn(BTN_DWN); // Initialize the down button

result doAlert(eventMask e, prompt &item);

int test=55;

int ledCtrl=LOW; // Initial value for external connected led

// Setting PWM properties, do not change this!
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

result myLedOn() {
  ledCtrl=HIGH;
  return proceed;
}
result myLedOff() {
  ledCtrl=LOW;
  return proceed;
}


char* constMEM hexDigit MEMMODE="0123456789ABCDEF";
char* constMEM hexNr[] MEMMODE={"0","x",hexDigit,hexDigit};
char buf1[]="0x11";


// define menu colors --------------------------------------------------------
//  {{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
//monochromatic color table


#define Black RGB565(0,0,0)
#define Red	RGB565(255,0,0)
#define Green RGB565(0,255,0)
#define Blue RGB565(0,0,255)
#define Gray RGB565(128,128,128)
#define LighterRed RGB565(255,150,150)
#define LighterGreen RGB565(150,255,150)
#define LighterBlue RGB565(150,150,255)
#define DarkerRed RGB565(150,0,0)
#define DarkerGreen RGB565(0,150,0)
#define DarkerBlue RGB565(0,0,150)
#define Cyan RGB565(0,255,255)
#define Magenta RGB565(255,0,255)
#define Yellow RGB565(255,255,0)
#define White RGB565(255,255,255)

const colorDef<uint16_t> colors[] MEMMODE={
  {
    {
      (uint16_t)Black,
      (uint16_t)Black
    },
    {
      (uint16_t)Black,
      (uint16_t)DarkerBlue,
      (uint16_t)DarkerBlue
    }
  },//bgColor
  {
    {
      (uint16_t)Gray,
      (uint16_t)Gray
    },
    {
      (uint16_t)White,
      (uint16_t)White,
      (uint16_t)White
    }
  },//fgColor
  {
    {
      (uint16_t)White,
      (uint16_t)Black
    },
    {
      (uint16_t)Yellow,
      (uint16_t)Yellow,
      (uint16_t)Red
    }
  },//valColor
  {
    {
      (uint16_t)White,
      (uint16_t)Black
    },
    {
      (uint16_t)White,
      (uint16_t)Yellow,
      (uint16_t)Yellow
    }
  },//unitColor
  {
    {
      (uint16_t)White,
      (uint16_t)Gray
    },
    {
      (uint16_t)Black,
      (uint16_t)Blue,
      (uint16_t)White
    }
  },//cursorColor
  {
    {
      (uint16_t)White,
      (uint16_t)Yellow
    },
    {
      (uint16_t)DarkerRed,
      (uint16_t)White,
      (uint16_t)White
    }
  },//titleColor
};

#define MAX_DEPTH 5

serialIn serial(Serial);

//MENU_INPUTS(in,&serial);its single, no need to `chainStream`

//define serial output device
idx_t serialTops[MAX_DEPTH]={0};
serialOut outSerial(Serial,serialTops);

#define GFX_WIDTH 240
#define GFX_HEIGHT 135
#define fontW 12
#define fontH 18

constMEM panel panels[] MEMMODE = {{0, 0, GFX_WIDTH / fontW, GFX_HEIGHT / fontH}};
navNode* nodes[sizeof(panels) / sizeof(panel)]; //navNodes to store navigation status
panelsList pList(panels, nodes, 1); //a list of panels and nodes
idx_t eSpiTops[MAX_DEPTH]={0};
TFT_eSPIOut eSpiOut(gfx,colors,eSpiTops,pList,fontW,fontH+1);
//menuOut* constMEM outputs[] MEMMODE={&outSerial,&eSpiOut};//list of output devices
//outputsList out(outputs,sizeof(outputs)/sizeof(menuOut*));//outputs list controller

//NAVROOT(nav,mainMenu,MAX_DEPTH,serial,out);

unsigned long idleTimestamp = millis();



//config myOptions('*','-',defaultNavCodes,false);

void button_init()
{

    int xpos = gfx.width() / 2; // Half the screen width
    btnUp.setLongClickHandler([](Button2 & b) {
        // Select
        unsigned int time = b.wasPressedFor();
        if (time >= 1000) {
        //  nav.doNav(enterCmd);
        }
    });
    
    btnUp.setClickHandler([](Button2 & b) {
       // Up
      //nav.doNav(downCmd); // It's called downCmd because it decreases the index of an array. Visually that would mean the selector goes upwards.
    });

    btnDwn.setLongClickHandler([](Button2 & b) {
        // Exit
        unsigned int time = b.wasPressedFor();
        if (time >= 1000) {
        //  nav.doNav(escCmd);
        }
    });
    
    btnDwn.setClickHandler([](Button2 & b) {
        // Down
     //   nav.doNav(upCmd); // It's called upCmd because it increases the index of an array. Visually that would mean the selector goes downwards.
    });
}

void button_loop()
{
    // Check for button presses
    btnUp.loop();
    btnDwn.loop();
}

void setup() {

Serial.begin(115200);
pinMode(25, INPUT_PULLUP);   
pinMode(33, INPUT);  
pinMode(32, INPUT); 
 //Serial.print("Initializing SD card...");
/*
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
*/
  
  //options=&myOptions;//can customize options
  //Serial.begin(115200); // Set Serial baudrate at 115200
  while(!Serial);
  Serial.flush();
  Serial.println();
  Serial.print("ARDUINOMENU DEMO V");
  Serial.print(PROG_VERSION);
  Serial.println(" FOR LILYGO TTGO T-DISPLAY");
  
  //nav.idleTask=idle;//point a function to be used when menu is suspended
  //mainMenu[1].disable();
  //outGfx.usePreview=true;//reserve one panel for preview?
  //nav.showTitle=false;//show menu title?

  //SPI.begin(); // Leave this commented or else there will be nothing shown on the screen.
  gfx.init(); // Initialize the screen.

  Serial.print("Configuring PWM for TFT backlight... ");
  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  Serial.println("DONE");

  Serial.print("Setting PWM for TFT backlight to default intensity... ");
  ledcWrite(pwmLedChannelTFT, ledBacklight);
  Serial.println("DONE");

  gfx.setRotation(1); // Rotate display a quarter clockwise
  
  gfx.setTextSize(2);
  gfx.setTextWrap(false);
  gfx.fillScreen(Black);
  
  Serial.print("Showing bootlogo... ");
  gfx.setSwapBytes(true);
  gfx.pushImage(0, 0,  240, 135, HahnLogo);
  delay(5000);
  Serial.println("DONE");
  
  Serial.print("Initialize buttons... ");
  button_init();
  delay(1000);
  Serial.println("DONE");

  Serial.print("Initialize external led... ");
  if(LED_CTRL > 0){
    pinMode(LED_CTRL,OUTPUT);
    digitalWrite(LED_CTRL,ledCtrl);
    delay(500);
    Serial.println("DONE");
  }
  else{
    delay(500);
    Serial.println("NOT SET UP");
  }

  delay(2000); // A little bit more delay so that you will be able to see the bootlogo.
  
  Serial.println("- READY -");
  
  gfx.fillScreen(Gray); // Clear the screen to be ready to draw the menu
  IndexMode=0;

  //--------------------------------------------- ESP8266 Initialization    
  /*  Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    WiFi.config(Local, Gateway, Subnet);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.print("Local IP address : ");
    Serial.println(WiFi.localIP());
    */
}

//----------------------------------------------------------------------
// Connects to the PLC
//----------------------------------------------------------------------
bool Connect()
{
    int Result=Client.ConnectTo(PLC, 
                                  0,  // Rack (see the doc.)
                                  0); // Slot (see the doc.)
    Serial.print("Connecting to ");Serial.println(PLC);  
    if (Result==0) 
    {
      Serial.print("Connected ! PDU Length = ");Serial.println(Client.GetPDULength());
    }
    else
      Serial.println("Connection error");
    return Result==0;
}
//----------------------------------------------------------------------
// Dumps a buffer (a very rough routine)
//----------------------------------------------------------------------
void Dump(void *Buffer, int Length)
{
  int i, cnt=0;
  pbyte buf;
  
  if (Buffer!=NULL)
    buf = pbyte(Buffer);
  else  
    buf = pbyte(&PDU.DATA[0]);

  Serial.print("[ Dumping ");Serial.print(Length);
  Serial.println(" bytes ]===========================");
  for (i=0; i<Length; i++)
  {
    cnt++;
    if (buf[i]<0x10)
      Serial.print("0");
    Serial.print(buf[i], HEX);
    Serial.print(" ");
    if (cnt==16)
    {
      cnt=0;
      Serial.println();
    }
  }  
  Serial.println("===============================================");
}
//----------------------------------------------------------------------
// Prints the Error number
//----------------------------------------------------------------------
void CheckError(int ErrNo)
{
  Serial.print("Error No. 0x");
  Serial.println(ErrNo, HEX);
  
  // Checks if it's a Severe Error => we need to disconnect
  if (ErrNo & 0x00FF)
  {
    Serial.println("SEVERE ERROR, disconnecting.");
    Client.Disconnect(); 
  }
}
//----------------------------------------------------------------------
// Profiling routines
//----------------------------------------------------------------------
void MarkTime()
{
  Elapsed=millis();
}
//----------------------------------------------------------------------
void ShowTime()
{
  // Calcs the time
  Elapsed=millis()-Elapsed;
  Serial.print("Job time (ms) : ");
  Serial.println(Elapsed);   
}


void loop() {
  //int xpos = gfx.width() / 2; // Half the screen width
  int Size, Result;
  void *Target;
   
  #ifdef DO_IT_SMALL
  Size=64;
  Target = NULL; // Uses the internal Buffer (PDU.DATA[])
#else
  Size=1024;
  Target = &Buffer; // Uses a larger buffer
#endif

  button_loop();
 // nav.poll();//this device only draws when needed
/*
 while (!Client.Connected)
  {
    if (!Connect())
      delay(500);
  }
  
  Serial.print("Reading ");Serial.print(Size);Serial.print(" bytes from DB");Serial.println(DBNum);
  // Get the current tick
  MarkTime();
  Result=Client.ReadArea(S7AreaDB, // We are requesting DB access
                         DBNum,    // DB Number
                         0,        // Start from byte N.0
                         Size,     // We need "Size" bytes
                         Target);  // Put them into our target (Buffer or PDU)
   */
 int sensorValueY = analogRead(33);
 int sensorValueX = analogRead(32);
 int buttonState = digitalRead(25);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  //float voltage = sensorValueY;
  // print out the value you read:

  String Message;
  Message=String(sensorValueX)+":"+String(sensorValueY)+":"+String(buttonState);
  
  Serial.println(Message);
       gfx.setTextColor(Black,Red);
       gfx.drawString(String(sensorValueX), 0,16 );
       gfx.drawString(String(sensorValueY), 0,32 );
       gfx.drawString(String(buttonState), 0,48 );
  /*                       
  if (Result==0)
  {
    ShowTime();
    Dump(Target, Size);
  }
  else
    CheckError(Result);
  IndexMode=PDU.DATA[1];
  
  gfx.setTextSize(2);

  switch (IndexMode){
    case 0:
       gfx.setTextColor(Black,Red);
       gfx.drawString(PLCModes[0], 0, 0);
    break;
    case 1:
       gfx.setTextColor(Black,Red);
       gfx.drawString(PLCModes[1], 0, 0);
    break;

       case 2:
       gfx.setTextColor(Black,Red);
       gfx.drawString(PLCModes[2], 0, 0);
    break;

    case 3:
       gfx.setTextColor(Black,Red);
       gfx.drawString(PLCModes[3], 0, 0);
    break;

    case 10:
       gfx.setTextColor(Black,Yellow);
       gfx.drawString(PLCModes[4], 0, 0);
    break;
    case 11:
       gfx.setTextColor(Black,Yellow);
       gfx.drawString(PLCModes[5], 0, 0);
    break;

    case 20:
       gfx.setTextColor(Black,Green);
       gfx.drawString(PLCModes[6], 0, 0);
    break;
    case 21:
       gfx.setTextColor(Black,Green);
       gfx.drawString(PLCModes[7], 0, 0);
    break;
     case 22:
       gfx.setTextColor(Black,Green);
       gfx.drawString(PLCModes[8], 0, 0);
    break;
    case 25:
       gfx.setTextColor(Black,Green);
       gfx.drawString(PLCModes[9], 0, 0);
    break;
    case 30:
       gfx.setTextColor(Black,Green);
       gfx.drawString(PLCModes[10], 0, 0);
    break;
  }
 */
   delay(500);
   
  // External connected led
  if(LED_CTRL > 0){ // Only set led state when a pinnumber for the external led has been provided.
    digitalWrite(LED_CTRL,ledCtrl);
  }
  
  // Set blacklight intenisty of screen
  ledcWrite(pwmLedChannelTFT, ledBacklight);
}
