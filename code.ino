// attiny85

#include <SoftwareSerial.h>

SoftwareSerial SIMSerial =  SoftwareSerial( 4,3 ); //rxPin, txPin

String TEXT = "";
long TIME = 0;
long OLDTIME = 0;
byte ZVON = 0;
long DOBA = 10000;

//************************************************************************************************

void setup(){
  SIMSerial.begin(9600);

  pinMode(0, OUTPUT);//KONTROLNI LED
  pinMode(1, OUTPUT);//CERVENA LED
  pinMode(2, OUTPUT);//ZELENA LED
  pinMode(4, INPUT_PULLUP);

  delay(6666);  
  
  SIMSerial.println("AT"); //otevreni komunikace
  delay(300);  
  SIMSerial.println("AT+CLIP=1");//nastaveni zobrazeni volajiciho cisla
  delay(300);  
  SIMSerial.println("AT+DDET=1,1000,0,0");//nastaveni dhmf
  delay(300);
  SIMSerial.println("AT+CLVL=99");//nastaveni dhmf
  delay(300);
 }
 
//************************************************************************************************ 
 
void loop(){
  if (ZVON) { TIME = millis();
	if (TIME - OLDTIME > DOBA)  { ZVON = 0; SIMSerial.println("ATH"); digitalWrite(0, LOW);  DOBA = 10000;       
   }
}
  if (SIMSerial.available() > 0) {
        TEXT = SIMSerial.readString();

 
 	if (TEXT.substring(22,31)=="737xxxxxx") {  //prijima pouze toto cislo
        	ZVON = 1;
        	OLDTIME = millis();
 		delay(100); 
		SIMSerial.println("ATA"); //prijem hovoru
		delay(100);
        	digitalWrite(0, HIGH); // svitit kontrolni LED
       } 

   if (TEXT.substring(3,7)=="DTMF") { 
          OLDTIME = millis();         
        if (TEXT.substring(9,10)=="3") { digitalWrite(1, LOW);    }  //zhasnout CERVENA
        if (TEXT.substring(9,10)=="1") { digitalWrite(1, HIGH);   }  //svitit
        if (TEXT.substring(9,10)=="9") { digitalWrite(2, LOW);    }  //zhasnout ZELENA
        if (TEXT.substring(9,10)=="7") { digitalWrite(2, HIGH);   }  //svitit
        if (TEXT.substring(9,10)=="5") { DOBA = 30000;  }           //prodlouzeni doby na 30s 
        if (TEXT.substring(9,10)=="0") { ZVON = 0; SIMSerial.println("ATH");  digitalWrite(0, LOW); DOBA = 10000; }// zhasnout kontrolni LED
        
       }   		
    }  
 }
