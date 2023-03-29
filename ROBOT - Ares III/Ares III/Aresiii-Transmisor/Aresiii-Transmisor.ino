#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,20,2);

RF24 radio(9,10);  // CE, CSN
const byte adress[11] = "aresiii3331";

int x,y,laser,i;
bool ok;
unsigned long pm;

void setup()
{
lcd.init();  
lcd.backlight();  
lcd.setCursor(0,0);
lcd.print("Conectado       ");

Serial.begin(115200);  


Serial.print("\n ARES III\n");

pinMode(7, INPUT);    ///boton analogico
digitalWrite(7,HIGH);

radio.begin();
radio.setPALevel(RF24_PA_HIGH);
radio.setDataRate(RF24_250KBPS);
radio.openWritingPipe(adress);
//radio.setChannel(5);
//radio.setPayloadSize(4);

radio.stopListening();
}
   
void loop()
{

x=analogRead(A0);
x=analogRead(A0);
x=map(x, 0, 1023, 0, 210);

y=analogRead(A1);
y=analogRead(A1);
y=map(y, 0, 1023, 300, 510);

///movimiento horizontal
if(x >=0 && x <=33) //izq vel5
   {comando(10);}
if(x >=34 && x <=66) //izq vel4
   {comando(11);}
if(x >=67 && x <=100) //izq vel3
   {comando(12);}


if(x >=110 && x <=143) //der1 vel1
  {comando(20);}
if(x >=144 && x <=176) //der2 vel2
  {comando(21);}
if(x >=177 && x <=210) //der3 vel3
  {comando(22);} 
       

///movimiento vertical
if(y >=300 && y <=333) //izq vel4
  {comando(30);}
if(y >=334 && y <=366) //izq vel4
  {comando(31);}
if(y >=367 && y <=400) //izq vel3
   {comando(32);}
 

if(y >=410 && y <=443) //izq vel4
  {comando(40);}
if(y >=444 && y <=476) //izq vel4
  {comando(41);}
if(y >=477 && y <=510) //izq vel3
   {comando(42);}
 
                     

if (digitalRead(7)== LOW)
   {comando(101+i);
   i++;
   lcd.setCursor(0,0);
   if(i==8)
   {lcd.print("Reiniciado       ");
   i=0;}
   else
   {lcd.print("Rele: ");
   lcd.print(i);
   lcd.print(" detonado");}
   }

if ((millis() - pm)>=10)
        {pm=millis();
        comando(0);}  
}


/////////////////////////////////////////////////////////////////
void comando(int orden)
{  

    ok = radio.write( &orden, sizeof(int) );

    lcd.setCursor(0,1);
    lcd.print(orden);
    Serial.println(orden);

   
 /*if((millis() - pm) >= 5000)
    {pm=millis();
     if(ok)
      {lcd.setCursor(0,1);
       lcd.print("Conectado       ");}
     else
      {lcd.setCursor(0,1);
       lcd.print("Desconectado    ");}
    }*/
    
    if (ok)                         //descomentar
      Serial.println("ok...\n");
    else
      Serial.println("failed.\n\r");
      
  delay(2);
}
