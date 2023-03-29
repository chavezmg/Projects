/*  ERIS II - ROBOT CODE
 *  written by Matias Chavez Buschiazzo
 *  colab with: 
 */


//motores derechos  MDS = Motor derecho superior, MDI = Motor derecho inferior, F = adelante, B = atras
#define MDI_PWM  2  //mover a A0
#define MDI_F    4
#define MDI_B    6
#define MDS_B    8
#define MDS_F    10
#define MDS_PWM  12  //mover a A1
//motores izquierdos  MIS = Motor izquierdo superior, MII = Motor izquierdo inferior, F = adelante, B = atras
#define MIS_PWM  3   //mover a A2
#define MIS_F    5
#define MIS_B    7
#define MII_B    9
#define MII_F    11
#define MII_PWM  13  //mover a A3
//sensores de ultrasonido frontales  SFD = sensor frontal derecho, SFI = sensor frontal izquierdo, E = ECHO, T = TRIGGER
#define SFI_T  32  
#define SFI_E  33
#define SFD_T  34
#define SFD_E  35
//sensores de ultrasonido traseros  STD = sensor trasero derecho, STI = sensor trasero izquierdo, E = ECHO, T = TRIGGER
#define STI_T  16
#define STI_E  17
#define STD_T  18
#define STD_E  19

int outputs[16]= {2,3,4,5,6,7,8,9,10,11,12,13,16,18,32,34};
int inputs[4] = {17,19,33,35};
int i, j, temp; //variables aux
unsigned long tiempo_SFD, tiempo_SFI, tiempo_STD, tiempo_STI;
int distancia_frontal_derecho;
int distancia_frontal_izquierdo;
int distancia_trasero_derecho;
int distancia_trasero_izquierdo;
bool forward = true;   //si es true, el vehiculo se esta moviendo hacia adelante, false hacia atras
bool sm = false;      //manejo con control remoto single mode
bool dm = false;      //manejo con control remoto double mode
bool rc = false;      //indica si está activo el modo control remoto
bool deb = false;
//los sig. 4 arreglos nos serviran para captar las señales de los sensores y evitar errores de medida
int listaSFD[10]={}; //Declaracion e inicializacion de un arreglo para el sensor frontal derecho
int listaSFI[10]={}; //Declaracion e inicializacion de un arreglo para el sensor frontal izquierdo
int listaSTD[10]={}; //Declaracion e inicializacion de un arreglo para el sensor trasero derecho
int listaSTI[10]={}; //Declaracion e inicializacion de un arreglo para el sensor trasero izquierdo
//velocidad
const int pwm = 255;
int contador_choque;
char control[3] ;
float velocidad;
 

void leer_sensores_front(){
  //medimos el tiempo que tarda en cambiar de estado el pin echo
  //utilizamos un metodo para descartar los valores erroneos, estos son valores que se alejan mucho
  //del verdadero valor, se carga una lista para cada sensor con 10 muestras, luego se la ordena de mayor a menor
  //finalmente se toma como verdadero valor, el valor que se encuentra en el centro de la lista.
  delay(20);
  contador_choque = 0;
  for (i=0;i<10;i++){   //cargar arreglo
    delayMicroseconds(10);
    digitalWrite(SFD_T, HIGH);
    delayMicroseconds(10);
    digitalWrite(SFD_T, LOW);
    tiempo_SFD = pulseIn(SFD_E, HIGH);   
    listaSFD[i] = tiempo_SFD * 0.017;
    //el siguiente codigo es para acelerar el frenado cuando algo se atraviesa
    if (listaSFD[i] <= 30 || listaSFI[i] <= 30 && forward){
      contador_choque += 1;}  
    if (contador_choque >= 3){
      freno();
      contador_choque = 0;}  
    } 
  delay(20);
  contador_choque = 0;
  for (i=0;i<10;i++){   //cargar arreglo 
    delayMicroseconds(10);
    digitalWrite(SFI_T, HIGH);
    delayMicroseconds(10);
    digitalWrite(SFI_T, LOW);
    tiempo_SFI = pulseIn(SFI_E, HIGH);
    listaSFI[i] = tiempo_SFI * 0.017;

    //el siguiente codigo es para acelerar el frenado cuando algo se atraviesa
    if (listaSFI[i] <= 30  || listaSFD[i] <= 30 && forward){
      contador_choque += 1;}  
    if (contador_choque >= 3){
      freno();
      contador_choque = 0;}  
    } 

for (i=1;i<10;i++){
       for (j=0; j < 10-i ;j++){     // for(j=0; j < Nelementos-i; j++) es menor y no menor igual
          if (listaSFD[j] > listaSFD[j+1]){   //Condicion mayor-menor
            temp=listaSFD[j];
            listaSFD[j]=listaSFD[j+1];
            listaSFD[j+1]=temp;}
          if (listaSFI[j] > listaSFI[j+1]){   //Condicion mayor-menor
            temp=listaSFI[j];
            listaSFI[j]=listaSFI[j+1];
            listaSFI[j+1]=temp;}
        }
}
  //calculamos las distancias que mide cada sensor
  distancia_frontal_derecho = (listaSFD[5]+listaSFD[6])/2;
  distancia_frontal_izquierdo = (listaSFI[5]+listaSFI[6])/2; 
}
void leer_sensores_back(){
  //medimos el tiempo que tarda en cambiar de estado el pin echo
  //utilizamos un metodo para descartar los valores erroneos, estos son valores que se alejan mucho
  //del verdadero valor, se carga una lista para cada sensor con 10 muestras, luego se la ordena de mayor a menor
  //finalmente se toma como verdadero valor, el valor que se encuentra en el centro de la lista.
  delay(20);
  contador_choque = 0;
  for (i=0;i<10;i++){   //cargar arreglo
    delayMicroseconds(10);
    digitalWrite(STD_T, HIGH);
    delayMicroseconds(10);
    digitalWrite(STD_T, LOW);
    tiempo_STD = pulseIn(STD_E, HIGH); 
    listaSTD[i] = tiempo_STD * 0.017;
    //el siguiente codigo es para acelerar el frenado cuando algo se atraviesa
    if (listaSTD[i] <= 30 || listaSTI[i] <= 30 && !forward){
      contador_choque += 1;}  
    if (contador_choque >= 3){
      freno();
      contador_choque = 0;}
    }
  delay(20);
  contador_choque = 0;
  for (i=0;i<10;i++){   //cargar arreglo
    delayMicroseconds(10);
    digitalWrite(STI_T, HIGH);
    delayMicroseconds(10);
    digitalWrite(STI_T, LOW);
    tiempo_STI = pulseIn(STI_E, HIGH);
    listaSTI[i] = tiempo_STI * 0.017;
    //el siguiente codigo es para acelerar el frenado cuando algo se atraviesa
    if (listaSTI[i] <= 30 || listaSTD[i] <= 30 && !forward){
      contador_choque += 1;}  
    if (contador_choque >= 3){
      freno();
      contador_choque = 0;}
    }
for (i=1;i<10;i++){
       for (j=0; j < 10-i ;j++){     // for(j=0; j < Nelementos-i; j++) es menor y no menor igual
          if (listaSTD[j] > listaSTD[j+1]){   //Condicion mayor-menor
            temp=listaSTD[j];
            listaSTD[j]=listaSTD[j+1];
            listaSTD[j+1]=temp;}
          if (listaSTI[j] > listaSTI[j+1]){   //Condicion mayor-menor
            temp=listaSTI[j];
            listaSTI[j]=listaSTI[j+1];
            listaSTI[j+1]=temp;}
        }
}
  //calculamos las distancias que mide cada sensor
  distancia_trasero_derecho = (listaSTD[5]+listaSTD[6])/2;
  distancia_trasero_izquierdo = (listaSTI[5]+listaSTI[6])/2;
}


///funciones de movimiento
void adelante(int v=pwm){
  if(rc){
    velocidad = v;}
  else if ((((distancia_frontal_derecho + distancia_frontal_izquierdo))*0.005)>=1){
    velocidad = pwm;}
  else{
    velocidad = pwm*((distancia_frontal_derecho + distancia_frontal_izquierdo)*0.005);}
  if (deb){
    Serial.println("Adelante");}
    
  digitalWrite(MDS_F, HIGH);
  digitalWrite(MDS_B, LOW);
  analogWrite(MDS_PWM, velocidad);
  digitalWrite(MDI_F, HIGH);
  digitalWrite(MDI_B, LOW);
  analogWrite(MDI_PWM, velocidad);
  digitalWrite(MIS_F, HIGH);
  digitalWrite(MIS_B, LOW);
  analogWrite(MIS_PWM, velocidad);
  digitalWrite(MII_F, HIGH);
  digitalWrite(MII_B, LOW);
  analogWrite(MII_PWM, velocidad);
}

void freno(){
  if (deb){
    Serial.println("Freno");}
    
  digitalWrite(MDS_F, LOW);
  digitalWrite(MDS_B, LOW);
  digitalWrite(MDS_PWM, HIGH);
  digitalWrite(MDI_F, LOW);
  digitalWrite(MDI_B, LOW);
  digitalWrite(MDI_PWM, HIGH);
  digitalWrite(MIS_F, LOW);
  digitalWrite(MIS_B, LOW);
  digitalWrite(MIS_PWM, HIGH);
  digitalWrite(MII_F, LOW);
  digitalWrite(MII_B, LOW);
  digitalWrite(MII_PWM, HIGH);
}

void atras(int v=pwm){
  if(rc){
    velocidad = v;}
  else if ((((distancia_trasero_derecho + distancia_trasero_izquierdo))*0.005)>=1){
    velocidad = pwm;}
  else{
    velocidad = pwm*((distancia_trasero_derecho + distancia_trasero_izquierdo)*0.005);}
  if (deb){
    Serial.println("Atras");}
    
  digitalWrite(MDS_F, LOW);
  digitalWrite(MDS_B, HIGH);
  analogWrite(MDS_PWM, velocidad);
  digitalWrite(MDI_F, LOW);
  digitalWrite(MDI_B, HIGH);
  analogWrite(MDI_PWM, velocidad);
  digitalWrite(MIS_F, LOW);
  digitalWrite(MIS_B, HIGH);
  analogWrite(MIS_PWM, velocidad);
  digitalWrite(MII_F, LOW);
  digitalWrite(MII_B, HIGH);
  analogWrite(MII_PWM, velocidad);
}

void adelante_derecha_90(int v=pwm){
  if (deb){
    Serial.println("Adelante derecha 90");}
  if(rc){
    velocidad = v;}
  else{
    velocidad = pwm*((2.5/distancia_frontal_izquierdo)*10);
  }
  
  digitalWrite(MDS_F, LOW);
  digitalWrite(MDS_B, HIGH);
  analogWrite(MDS_PWM, velocidad);
  digitalWrite(MDI_F, LOW);
  digitalWrite(MDI_B, HIGH);
  analogWrite(MDI_PWM, velocidad);
  digitalWrite(MIS_F, HIGH);
  digitalWrite(MIS_B, LOW);
  analogWrite(MIS_PWM, velocidad);
  digitalWrite(MII_F, HIGH);
  digitalWrite(MII_B, LOW);
  analogWrite(MII_PWM, velocidad);
}

void adelante_izquierda_90(int v=pwm){
  if (deb){
    Serial.println("Adelante izquierda 90");}
  if(rc){
    velocidad = v;}
  else{
    velocidad = pwm*((2.5/distancia_frontal_derecho)*10);
  }   
  
  digitalWrite(MDS_F, HIGH);
  digitalWrite(MDS_B, LOW);
  analogWrite(MDS_PWM, velocidad);
  digitalWrite(MDI_F, HIGH);
  digitalWrite(MDI_B, LOW);
  analogWrite(MDI_PWM, velocidad);
  digitalWrite(MIS_F, LOW);
  digitalWrite(MIS_B, HIGH);
  analogWrite(MIS_PWM, velocidad);
  digitalWrite(MII_F, LOW);
  digitalWrite(MII_B, HIGH);
  analogWrite(MII_PWM, velocidad);
}

void adelante_derecha_45(int v=pwm){
  if (deb){
    Serial.println("Adelante derecha 45");}
  if(rc){
    velocidad = v;}
  else{
      velocidad = pwm*((5/distancia_frontal_izquierdo)*10);
  }      
    
  digitalWrite(MDS_F, LOW);
  digitalWrite(MDS_B, LOW);
  analogWrite(MDS_PWM, velocidad);
  digitalWrite(MDI_F, LOW);
  digitalWrite(MDI_B, LOW);
  analogWrite(MDI_PWM, velocidad);
  digitalWrite(MIS_F, HIGH);
  digitalWrite(MIS_B, LOW);
  analogWrite(MIS_PWM, velocidad);
  digitalWrite(MII_F, HIGH);
  digitalWrite(MII_B, LOW);
  analogWrite(MII_PWM, velocidad);
}

void adelante_izquierda_45(int v=pwm){
  if (deb){
    Serial.println("Adelante izquierda 45");}

  if(rc){
    velocidad = v;}
  else{
    velocidad = pwm*((5/distancia_frontal_derecho)*10);
  }       
  
  digitalWrite(MDS_F, HIGH);
  digitalWrite(MDS_B, LOW);
  analogWrite(MDS_PWM, velocidad);
  digitalWrite(MDI_F, HIGH);
  digitalWrite(MDI_B, LOW);
  analogWrite(MDI_PWM, velocidad);
  digitalWrite(MIS_F, LOW);
  digitalWrite(MIS_B, LOW);
  analogWrite(MIS_PWM, velocidad);
  digitalWrite(MII_F, LOW);
  digitalWrite(MII_B, LOW);
  analogWrite(MII_PWM, velocidad);
}

void atras_derecha_90(int v=pwm){
  if (deb){
    Serial.println("Atras derecha 90");}
  if(rc){
    velocidad = v;}
  else{
    velocidad = pwm*((2.5/distancia_trasero_izquierdo)*10);
  }       
  
  digitalWrite(MDS_F, LOW);
  digitalWrite(MDS_B, HIGH);
  analogWrite(MDS_PWM, velocidad);
  digitalWrite(MDI_F, LOW);
  digitalWrite(MDI_B, HIGH);
  analogWrite(MDI_PWM, velocidad);
  digitalWrite(MIS_F, HIGH);
  digitalWrite(MIS_B, LOW);
  analogWrite(MIS_PWM, velocidad);
  digitalWrite(MII_F, HIGH);
  digitalWrite(MII_B, LOW);
  analogWrite(MII_PWM, velocidad);
}
void atras_izquierda_90(int v=pwm){
  if (deb){
    Serial.println("Atras izquierda 90");}
  if(rc){
    velocidad = v;}
  else{
    velocidad = pwm*((2.5/distancia_trasero_derecho)*10);
  }       

  digitalWrite(MDS_F, HIGH);
  digitalWrite(MDS_B, LOW);
  analogWrite(MDS_PWM, velocidad);
  digitalWrite(MDI_F, HIGH);
  digitalWrite(MDI_B, LOW);
  analogWrite(MDI_PWM, velocidad);
  digitalWrite(MIS_F, LOW);
  digitalWrite(MIS_B, HIGH);
  analogWrite(MIS_PWM, velocidad);
  digitalWrite(MII_F, LOW);
  digitalWrite(MII_B, HIGH);
  analogWrite(MII_PWM, velocidad);
}
void atras_derecha_45(int v=pwm){
  if (deb){
    Serial.println("Atras derecha 45");}
  if(rc){
    velocidad = v;}
  else{
    velocidad = pwm*((5/distancia_trasero_izquierdo)*10);
  }       
  
  digitalWrite(MDS_F, LOW);
  digitalWrite(MDS_B, LOW);
  analogWrite(MDS_PWM, velocidad);
  digitalWrite(MDI_F, LOW);
  digitalWrite(MDI_B, LOW);
  analogWrite(MDI_PWM, velocidad);
  digitalWrite(MIS_F, HIGH);
  digitalWrite(MIS_B, LOW);
  analogWrite(MIS_PWM, velocidad);
  digitalWrite(MII_F, HIGH);
  digitalWrite(MII_B, LOW);
  analogWrite(MII_PWM, velocidad);
}
void atras_izquierda_45(int v=pwm){
  if (deb){
    Serial.println("Atras izquierda 45");}
  if(rc){
    velocidad = v;}
  else{
    velocidad = pwm*((5/distancia_trasero_derecho)*10);
  }       
  
  digitalWrite(MDS_F, HIGH);
  digitalWrite(MDS_B, LOW);
  analogWrite(MDS_PWM, velocidad);
  digitalWrite(MDI_F, HIGH);
  digitalWrite(MDI_B, LOW);
  analogWrite(MDI_PWM, velocidad);
  digitalWrite(MIS_F, LOW);
  digitalWrite(MIS_B, LOW);
  analogWrite(MIS_PWM, velocidad);
  digitalWrite(MII_F, LOW);
  digitalWrite(MII_B, LOW);
  analogWrite(MII_PWM, velocidad);
}
//////////////////////////////

void trap(){
  if (deb){
    Serial.println("Trap");}
  
  while(true){
    if (deb){
      Serial.println("Inside Trap");}      
    delay(100);
    freno();
    leer_sensores_front();
    leer_sensores_back();
    if (distancia_frontal_derecho >= 40 && distancia_frontal_izquierdo >=40){
      forward = true;
      break;}
    else if (distancia_trasero_derecho >= 40 && distancia_trasero_izquierdo >=40){
      forward = false;
      break;}
    else{
          if(distancia_frontal_derecho >= distancia_trasero_derecho && distancia_frontal_izquierdo >= distancia_trasero_izquierdo){   //giro para buscar salida
             adelante_derecha_90();}   
          else if (distancia_trasero_derecho > distancia_frontal_derecho && distancia_trasero_izquierdo > distancia_frontal_izquierdo){ //giro para buscar salida
             atras_derecha_90();}
          else if (distancia_frontal_derecho > 15 && distancia_frontal_izquierdo > 15){ //giro para buscar salida
             adelante_derecha_90();}
          else if (distancia_trasero_derecho > 15 && distancia_trasero_izquierdo > 15){ //giro para buscar salida
             atras_derecha_90();}
          else{
             freno();}
        } 
    }
}

void decision_front(){
  //////movimiento hacia adelante//////
      
  //CASOS GENERALES
  if (distancia_frontal_derecho >= 80 && distancia_frontal_izquierdo >= 80 && forward){
    adelante();
  }   
  else if (distancia_frontal_derecho >= 80 && distancia_frontal_izquierdo >= 50 && distancia_frontal_izquierdo < 80 && forward){
      adelante_derecha_45();
   }  
  else if (distancia_frontal_izquierdo >= 80 && distancia_frontal_derecho >= 50 && distancia_frontal_derecho < 80 && forward){
      adelante_izquierda_45();
   }  
  else if (distancia_frontal_derecho >= 35 && distancia_frontal_izquierdo >= 35 && distancia_frontal_derecho < 65 && distancia_frontal_izquierdo < 65 && forward){
    if (distancia_frontal_derecho >= distancia_frontal_izquierdo){
      freno();
      adelante_derecha_90();}
    else{
      freno();
      adelante_izquierda_90();}
   }

  //FRENO
  if ((distancia_frontal_derecho < 35|| distancia_frontal_izquierdo < 35) && forward){
    freno();
    leer_sensores_back();
    leer_sensores_front();
    if(distancia_frontal_derecho >= 60 && distancia_frontal_izquierdo >= 60){   //cambio de direccion
      forward = true;}
    else if(distancia_trasero_derecho >= 60 && distancia_trasero_izquierdo >= 60){   //cambio de direccion
      forward = false;}  
    else if ((distancia_frontal_derecho < 60 && distancia_frontal_izquierdo < 60) && (distancia_trasero_derecho < 60 && distancia_trasero_izquierdo < 60)){
      trap();}
    else{
      freno();}  
  }
} 

void decision_back(){
  //////movimiento hacia atras//////
  
  //CASOS GENERALES
  if (distancia_trasero_derecho >= 80 && distancia_trasero_izquierdo >= 80 && !forward){
    atras();
  }   
  else if (distancia_trasero_derecho >= 80 && distancia_trasero_izquierdo >= 50 && distancia_trasero_izquierdo < 80 && !forward){
      atras_derecha_45();
   }  
  else if (distancia_trasero_izquierdo >= 80 && distancia_trasero_derecho >= 50 && distancia_trasero_derecho < 80 && !forward){
      atras_izquierda_45();
   }  
  else if (distancia_trasero_derecho >= 35 && distancia_trasero_izquierdo >= 35 && distancia_trasero_derecho < 65 && distancia_trasero_izquierdo < 65 && !forward){
    if (distancia_trasero_derecho >= distancia_trasero_izquierdo){
      freno();
      atras_derecha_90();}
    else{
      freno();
      atras_izquierda_90();}
   }

  //FRENO
  if ((distancia_trasero_derecho < 35 || distancia_trasero_izquierdo < 35) && !forward){
    freno();
    leer_sensores_back();
    leer_sensores_front();
    if(distancia_frontal_derecho >= 60 && distancia_frontal_izquierdo >= 60){   //cambio de direccion
      forward = true;}
    else if(distancia_trasero_derecho >= 60 && distancia_trasero_izquierdo >= 60){   //cambio de direccion
      forward = false;}  
    else if ((distancia_frontal_derecho < 60 && distancia_frontal_izquierdo < 60) && (distancia_trasero_derecho < 60 && distancia_trasero_izquierdo < 60)){
      trap();}
    else{
      freno();}  
  }
}

void debug(){
  deb = true;
  if (forward){
    leer_sensores_front();
    Serial.print("FD:");
    Serial.print(distancia_frontal_derecho);
    Serial.print(" FI:");
    Serial.print(distancia_frontal_izquierdo);
  }
  else{
    leer_sensores_back();
    Serial.print(" TD:");
    Serial.print(distancia_trasero_derecho);
    Serial.print(" TI:");
    Serial.print(distancia_trasero_izquierdo);
  }
  Serial.print("  Dir:");
  Serial.print(forward);
  Serial.print("  Vel: ");
  Serial.println(velocidad);
}

void debug2(){
  deb = true;

    leer_sensores_front();
    Serial.print("FD:");
    Serial.print(distancia_frontal_derecho);
    Serial.print(" FI:");
    Serial.println(distancia_frontal_izquierdo);

    
    leer_sensores_back();
    Serial.print(" TD:");
    Serial.print(distancia_trasero_derecho);
    Serial.print(" TI:");
    Serial.println(distancia_trasero_izquierdo);

}


void rcf(){
  if (control[2] == 's'){
    sm = true;
    dm = false;}
  if (control[2] == 'd'){
    dm = true;
    sm = false;}
  if (sm){                //single mode
    if (control[0]== 'f'){
      adelante(control[1]);
      }
    if (control[0]== 'b'){
      atras(control[1]);
      }
    if (control[0]== 's'){
      freno();
      }
    if (control[0]== 'd'){
      adelante_derecha_45(control[1]);
      }
    if (control[0]== 'i'){
      adelante_izquierda_45(control[1]);
      }
    if (control[0]== 'e'){
      adelante_derecha_90(control[1]);
      }
    if (control[0]== 'o'){
      adelante_izquierda_90(control[1]);
      }
  }
  if (dm){              //double mode
    if (control[0]== 'f'){
      adelante(control[1]);
      }
    if (control[0]== 'b'){
      atras(control[1]);
      }
    if (control[0]== 's'){
      freno();
      }
  }
}

String inputString = "";
bool stringComplete = false;
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void setup() {
  Serial.begin(9600);
  for (i=0; i<16; i++){
    pinMode(outputs[i], OUTPUT);
    digitalWrite(outputs[i], LOW);}
  for (i=0; i<4; i++){
    pinMode(inputs[i], INPUT);}
}

void loop() {
  //debug();
  //debug2();
  
 /* if (forward){
    //debug();
    leer_sensores_front();
    decision_front();}
  if (!forward){
    //debug();
    leer_sensores_back();
    decision_back();}*/
  rc = true;
  if (rc){
    rcf();
  }

  if (stringComplete) {
    Serial.println("input: ");
    Serial.println(inputString);
    control[0] = inputString[0];
    control[1] = (inputString[1]-48)*25;
    control[2] = inputString[2];
    Serial.println(control[0]);
    Serial.println(control[1]);
    Serial.println(control[2]);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

}
