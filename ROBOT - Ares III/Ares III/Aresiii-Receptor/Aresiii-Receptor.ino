#include <Servo.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

int valor_recibido = 0;
RF24 radio(9,10);  // CE, CSN
const byte adress[11] = "aresiii3332";

unsigned long pmt1=0; 
unsigned long pmt2=0;
unsigned long pm1=0; 
unsigned long pm2=0;
unsigned long pm3=0; 
unsigned long pm4=0; 
unsigned long pm5=0; 
unsigned long pm6=0; 
unsigned long pm7=0; 
unsigned long pm8=0; 
////////////relays//////////
boolean r1 = false;
boolean r2 = false;
boolean r3 = false;
boolean r4 = false;
boolean r5 = false;
boolean r6 = false;
boolean r7 = false;
boolean r8 = false;
/////////nro rele ya detonado///////
boolean d1=false;
boolean d2=false;
boolean d3=false;
boolean d4=false;
boolean d5=false;
boolean d6=false;
boolean d7=false;
boolean d8=false;

int tdet=2500; ////tiempo detonacion
float valor_x;
float valor_y;

Servo X;
Servo Y;

void setup() {
Serial.begin(115200);
X.attach(5);
Y.attach(6);

pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
pinMode(A0, OUTPUT);
pinMode(A1, OUTPUT);
pinMode(A2, OUTPUT);
pinMode(A3, OUTPUT);
pinMode(A4, OUTPUT);

digitalWrite(2, HIGH);
digitalWrite(3, HIGH);
digitalWrite(4, HIGH);
digitalWrite(A0, HIGH);
digitalWrite(A1, HIGH);
digitalWrite(A2, HIGH);
digitalWrite(A3, HIGH);
digitalWrite(A4, HIGH);

Serial.print("\n ARES III\n");
radio.begin();
radio.setPALevel(RF24_PA_HIGH);
radio.setDataRate(RF24_250KBPS);
radio.openReadingPipe(0, adress);
//radio.setChannel(5);  
//radio.setPayloadSize(4);


radio.startListening();  

X.write(90);
Y.write(90);
valor_x=90;
valor_y=90;
}


void loop() {
  
    //while (!radio.available());
    radio.read(&valor_recibido, sizeof(valor_recibido));
    Serial.println(valor_recibido);
        
 /////////////////////////////////////       
 //////////////disparar/////////////// 
 /////////////////////////////////////   

      if (valor_recibido ==101 && d1==false)
        {digitalWrite(2, LOW);
         r1=true;
         pm1=millis();}
      if (valor_recibido ==102 && d2==false)
        {digitalWrite(3, LOW);
         r2=true;
         pm2=millis();}
      if (valor_recibido ==103 && d3==false)
        {digitalWrite(4, LOW);
         r3=true;
         pm3=millis();}
      if (valor_recibido ==104 && d4==false)
        {digitalWrite(A0, LOW);
         r4=true;
         pm4=millis();}  
      if (valor_recibido ==105 && d5==false)
        {digitalWrite(A1, LOW);
         r5=true;
         pm5=millis();}  
      if (valor_recibido ==106 && d6==false)
        {digitalWrite(A2, LOW);
         r6=true;
         pm6=millis();}  
      if (valor_recibido ==107 && d7==false)
        {digitalWrite(A3, LOW);
         r7=true;
         pm7=millis();}    
      if (valor_recibido ==108 && d8==false)
        {digitalWrite(A4, LOW);
         r8=true;
         pm8=millis();}
      
         
      if (valor_recibido ==109 ) ////recarga
        {d1=false;
         d2=false;
         d3=false;
         d4=false;
         d5=false;
         d6=false;
         d7=false;
         d8=false;}            
/************fin disparar*************/         

////////////////Movimiento Horizontal/////////////////
       if(valor_x <= 177.75)
         {if (valor_recibido == 10)         
              {valor_x+=2;}
          if (valor_recibido == 11)         
              {valor_x+=0.5;}     
          if (valor_recibido == 12)         
              {valor_x+=0.25;}
          X.write(valor_x);      
          }
          
       if(valor_x >= 0.25) 
         {if (valor_recibido == 20)         
              {valor_x-=0.25;}
          if (valor_recibido == 21)         
              {valor_x-=0.5;}     
          if (valor_recibido == 22)         
              {valor_x-=2;}
           X.write(valor_x);
          }        
      /////////////////////////////////////////////////////////
      
      /////////////////Movimiento vertical////////////////////
       if(valor_y <= 177.75)
         {if (valor_recibido == 30)
              {valor_y+=2;}
          if (valor_recibido == 31)
              {valor_y+=0.5;}
          if (valor_recibido == 32)
              {valor_y+=0.25;}
          Y.write(valor_y);
         }
         
       if(valor_y >= 0.25)
         {if (valor_recibido == 40)
              {valor_y-=0.25;}
          if (valor_recibido == 41)
              {valor_y-=0.5;}
          if (valor_recibido == 42)
              {valor_y-=2;}
          Y.write(valor_y);    
         }
         //Serial.print(valor_y);
         
/////////////////////////////////////////////////////////                        

 /////////////////////////////////////////////////////    
  ///////////timers que apagan los cortocircuitos//////
  ///////////////////////////////////////////////////// 
      if (r1==true && ((millis() - pm1))>=tdet)
        {digitalWrite(2,HIGH);
         r1=false;}  
      if (r2==true && ((millis() - pm2)>=tdet))
        {digitalWrite(3,HIGH);
         r2=false;} 
      if (r3==true && ((millis() - pm3)>=tdet))
        {digitalWrite(4,HIGH);
         r3=false;} 
      if (r4==true && ((millis() - pm4)>=tdet))
        {digitalWrite(A0,HIGH);
         r4=false;}  
      if (r5==true && ((millis() - pm5)>=tdet))
        {digitalWrite(A1,HIGH);
         r5=false;} 
      if (r6==true && ((millis() - pm6)>=tdet))
        {digitalWrite(A2,HIGH);
         r6=false;} 
      if (r7==true && ((millis() - pm7)>=tdet+500))
        {digitalWrite(A3,HIGH);
         r7=false;} 
      if (r8==true && ((millis() - pm8)>=tdet+500))
        {digitalWrite(A4,HIGH);
         r8=false;} 
 /*****************FIN timer cortocircuitos***************/      

      valor_recibido = 0;
}
