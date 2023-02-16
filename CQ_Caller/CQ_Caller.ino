//CQ Caller



//NASTAVENÍ !!!

int   timeoutset  = 200;                         //prodleva přepnutí prog/play

//procesní proměnné
byte   switched              ;                       //stav přepínače
unsigned long casovacstart = 0;                      // kdy naposledy zapnuto [ms]
int                    cas = 0;                      // naměřený čas
unsigned long  mezerastart = 0;                      // kdy naposledy vypnuto [ms]
int                 mezera = 0;                      // mezera mezi CQ
boolean   oversetPlay           =0;                       //stav přepínače
boolean   oversetProg           =0;
boolean   oversetRec            =0;
byte      exchange              =1;
byte      exchangePlay          =1;
byte      exchangeRec           =1;
boolean   playstate             = LOW;
boolean   mezerastate;

   


#define REC     12
#define Play    10
#define VOX     11
#define PTT     8
#define Buzzer  7
#define Switch A1


void setup() {

  Serial.begin(9600);
  pinMode(Switch, INPUT);
  pinMode(REC, OUTPUT);
  pinMode(Play, OUTPUT);
  pinMode(VOX, OUTPUT);
  pinMode(PTT, OUTPUT);
  pinMode(Buzzer, OUTPUT);
 
 pinMode(13, OUTPUT);

  
  digitalWrite(13,HIGH);
}

void loop(){
  switched = digitalRead (Switch);                                             //poloha přepínače
              Serial.println(switched," "); 
           if (switched != exchange && switched == 0){ 
             casovacstart = millis();   Serial.println(casovacstart );                //start časovače pro mezeru mezi CQ 
           }
           if (switched == 0){
               cas=(millis()-casovacstart);                                      //výpočet časovače 
             if(cas>timeoutset){prog();
           } } 
           if (switched != exchange && switched == 1){
             if (cas < timeoutset){
               oversetPlay = !oversetPlay; 
           } } 
           if (oversetPlay == true){
               //exchangePlay =oversetPlay
               play();
           }
           exchange = switched;
           if (playstate = HIGH) digitalWrite (13,LOW);
           else digitalWrite (13,HIGH);
          
}
void prog(){  Serial.println("prog");                                           // proces programování času mezery
              //digitalWrite (Buzzer,HIGH);delay (500);digitalWrite (Buzzer,LOW);
              oversetPlay = false;
              mezera = cas;                                                     //zápis mezery
              //digitalWrite (Buzzer,HIGH);delay (500);digitalWrite (Buzzer,LOW); // pípnutí jako potvrzení
              Serial.print("Mezera "); 
              Serial.print(mezera); 
           
}
void play(){ Serial.println("play");
           
            unsigned long currentMillis = millis();
           if(currentMillis - mezerastart > mezera) {       // uložte, kdy jste naposledy blikli LED
              mezerastart = currentMillis;   Serial.println(playstate);             // jestliže je LED vypnutá, zapněte ji a naopak:
              if (playstate == HIGH) playstate=LOW;
              else                  playstate=HIGH;
           
           }
}
           
