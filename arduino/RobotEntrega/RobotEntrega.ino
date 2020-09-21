
#include "HX711.h"
/*
Sensor de peso
*/
#define DOUT  31
#define CLK  30
HX711 balanza;
int CARGADO=0;
float PESO=0;

/*
Motores
*/
int OUTPUT1 = 4;
int OUTPUT2 = 5;
int OUTPUT3 = 6;
int OUTPUT4 = 7;
int ENA1 = 2;
int ENA2 = 3;

/*
Infrarrojos
*/
int LeftInfraPin = 10;
int RightInfraPin = 11;
int EndInfraPin = 33;
int LIVal = 0;
int RIVal = 0;  
int ENDIVal = 0;  
/*
Ultrasonico
*/
int TRIG = 9;      // trigger en pin 10
int ECO = 8;      // echo en pin 9
int DURACION;
int DISTANCIA;
int OBSTACULO = 0;
int CANTIDAD_OBSTACULO = 0;
/*
Ubicacion
0 En buzon
1 En recorrido
2 En punto de entrega
*/
int UBICACION = 0;
/*
Estado
0 inactivo
1 activo 
*/
int ESTADO = 0;
int ESTABA_EN_BUZON=1;
/*Cadena de envio*/
String cad_data = "";

void setup() {
  Serial.begin(9600);
  //Balanza
  balanza.begin(DOUT, CLK);
  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando...");
  Serial.println("...");
  balanza.set_scale(439430.25); // Establecemos la escala
  balanza.tare(20);  //El peso actual es considerado Tara.

  pinMode(OUTPUT1,OUTPUT);
  pinMode(OUTPUT2,OUTPUT);
  pinMode(OUTPUT3,OUTPUT);
  pinMode(OUTPUT4,OUTPUT);
  pinMode(ENA1,OUTPUT);
  pinMode(ENA2,OUTPUT);

  pinMode(LeftInfraPin,INPUT);
  pinMode(RightInfraPin,INPUT);
  pinMode(EndInfraPin,INPUT);

  pinMode(TRIG, OUTPUT);  // trigger como salida
  pinMode(ECO, INPUT);    // echo como entrada

}

void obtenerPeso(){
  Serial.print(balanza.get_units(20),3);
  Serial.println(" kg");
  String peso_tmp = (String(balanza.get_units(20),3));
  PESO = peso_tmp.toFloat();
  if(PESO<=0){
    PESO=0;
  }
  CARGADO = 0;
  if(PESO>0.03){
    CARGADO = 1;
  }
}

void verificarObstaculo(){
  digitalWrite(TRIG, HIGH);     // generacion del pulso a enviar
  delay(1);       // al pin conectado al trigger
  digitalWrite(TRIG, LOW);    // del sensor
  
  DURACION = pulseIn(ECO, HIGH);  // con funcion pulseIn se espera un pulso
            // alto en Echo
  DISTANCIA = DURACION / 58.2;    // distancia medida en centimetros
  Serial.println(DISTANCIA);    // envio de valor de distancia por monitor serial
  delay(25);       // demora entre datos

  if (DISTANCIA <= 10){
    OBSTACULO=1;
  }else{
    OBSTACULO=0;
  }

}
void detenerAuto(){
  digitalWrite(OUTPUT1,0);
  digitalWrite(OUTPUT2,0);
  digitalWrite(OUTPUT3,0);
  digitalWrite(OUTPUT4,0);
}
void conducir(){
  analogWrite(ENA1,105);
  analogWrite(ENA2,105);
  
  if(LIVal==1){
    // Si la lectura del infrarrojo #1 es 1, entonces se cumplira una de las siguientes condiciones:
    if(RIVal==1){
      // Si la lectura del infrarrojo #2 es 1, es decir los dos sensores estan sobre la linea negra, entonces los dos motores avanzaran en linea recta.
      digitalWrite(OUTPUT1,0);
      digitalWrite(OUTPUT2,1);
      digitalWrite(OUTPUT3,0);
      digitalWrite(OUTPUT4,1); 
    }else{
       // Si la lectura del infrarrojo #2 es 0, el sensor#1 esta sobre la linea negra y el sensor#2 esta fuera de la linea negra, entonces solo una rueda gira y esto causara el giro.
      digitalWrite(OUTPUT1,0);
      digitalWrite(OUTPUT2,0);
      digitalWrite(OUTPUT3,0);
      digitalWrite(OUTPUT4,1); 
    }
  }else{
    // Si la lectura del infrarrojo #2 es 1, se daran las siguientes posibilidades:
    if(RIVal==1){
      // Como el sensor#1 esta fuera de la linea negra y el sensor#2 esta sobre la linea negra, entonces solo una rueda gira y esto causara el giro.
      digitalWrite(OUTPUT1,0);
      digitalWrite(OUTPUT2,1);
      digitalWrite(OUTPUT3,0);
      digitalWrite(OUTPUT4,0);
    }else{ 
      // si ambos sensores dan lectura 1, los dos estan fuera de la linea negra, para que vuelva a su trayectoria tiene que retroceder.
      digitalWrite(OUTPUT1,1);
      digitalWrite(OUTPUT2,0);
      digitalWrite(OUTPUT3,1);
      digitalWrite(OUTPUT4,0);
    } 
  }
  
}

void actualizarInfrarojo(){
  LIVal = digitalRead(LeftInfraPin);
  Serial.print("LIVal ");
  Serial.println(LIVal);

  RIVal = digitalRead(RightInfraPin);
  Serial.print("RIVal ");
  Serial.println(RIVal);
  
  ENDIVal = digitalRead(EndInfraPin);
  Serial.print("ENDIVal ");
  Serial.println(ENDIVal);
  if(ENDIVal==1){
    if(ESTABA_EN_BUZON==1){
      UBICACION=2;
      ESTABA_EN_BUZON=0;
    }else{
      ESTABA_EN_BUZON=1;
      UBICACION=0;
    }
  }
}

void enviarInfo(){
  //Ubicacion-Estado-Peso-NumeroObstaculos

  cad_data = String(UBICACION) + "-" + String(ESTADO) + "-" + String(PESO)+"-"+String(CANTIDAD_OBSTACULO);
  Serial2.print(cad_data);
  Serial.println(cad_data);
}
void setInicioBuzon(){
  ESTABA_EN_BUZON=1;
  UBICACION=0;
}
void setInicioRecorrido(){
  CANTIDAD_OBSTACULO=0;
  UBICACION=1;
  ESTADO=1;
}
void setEntrega(){
  UBICACION=2;  
}
void loop() {
  setInicioBuzon();
  obtenerPeso();
  /*AQUI DEBO LEER EL GET A VER SI ME CAMBIAN EL ESTADO*/
  if(CARGADO==1){
    enviarInfo();
    setInicioRecorrido();
    while(UBICACION==1){
      /*Recorrido*/
      verificarObstaculo();
      if(OBSTACULO==1){
        while(OBSTACULO==1){
          verificarObstaculo();
          detenerAuto();
        }
        CANTIDAD_OBSTACULO=CANTIDAD_OBSTACULO+1;
      }else{
        actualizarInfrarojo();
        conducir();
      }
      enviarInfo();
    }
    detenerAuto();
    delay(3000);    
    /*para este punto ya llego al punto de entrega*/
    setEntrega();
    while(CARGADO==1){
      obtenerPeso();
      enviarInfo();
      detenerAuto();
    }
    setInicioRecorrido();
    while(UBICACION==1){
      /*Recorrido*/
      verificarObstaculo();
      if(OBSTACULO==1){
        while(OBSTACULO==1){
          verificarObstaculo();
          detenerAuto();
        }
        CANTIDAD_OBSTACULO=CANTIDAD_OBSTACULO+1;
      }else{
        actualizarInfrarojo();
        conducir(); 
      }
      enviarInfo();      
    }
  }else{
    if(UBICACION==0){
      detenerAuto();      
    }
  }
  enviarInfo();
  delay(1500);
}
