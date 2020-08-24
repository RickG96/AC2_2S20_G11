

#include "HX711.h"

HX711 balanza;

int TRIG1 = 7;      // trigger en pin 10
int ECO1 = 6;      // echo en pin 9
int DURACION1;
int DISTANCIA1;

int TRIG2 = 11;      // trigger en pin 10
int ECO2 = 10;      // echo en pin 9
int DURACION2;
int DISTANCIA2;
//Sensor de peso
#define DOUT  8
#define CLK  9


//Pin bomba de agua
int PIN_AGUA = 5;
//logistica
bool abierto = false;
int distancia_puerta = 7;
String cad_data_persev="";
void setup() {
  Serial.begin(9600);     // inicializacion de comunicacion serial a 9600 bps
  Serial2.begin(9600);
  // put your setup code here, to run once:
  pinMode(TRIG1, OUTPUT);  // trigger como salida
  pinMode(ECO1, INPUT);    // echo como entrada
  
  pinMode(TRIG2, OUTPUT);  // trigger como salida
  pinMode(ECO2, INPUT);    // echo como entrada

  pinMode(PIN_AGUA, OUTPUT);  // ping squirtle!
  
  balanza.begin(DOUT, CLK);
//  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
//  Serial.println("No ponga ningun  objeto sobre la balanza");
//  Serial.println("Destarando...");
//  Serial.println("...");
  balanza.set_scale(439430.25); // Establecemos la escala, esta es la escala para kilogramo
  balanza.tare(20);  //El peso actual es considerado Tara.
  
//  Serial.println("Listo para pesar");  
}

void loop() {
  /*
  El ultrasonico 1 es el de la puerta
  */
  
  // put your main code here, to run repeatedly:
  digitalWrite(TRIG1, HIGH);     // generacion del pulso a enviar
  delay(1);       // al pin conectado al trigger
  digitalWrite(TRIG1, LOW);    // del sensor
  DURACION1 = pulseIn(ECO1, HIGH);  // con funcion pulseIn se espera un pulso
    // alto en Echo
  DISTANCIA1 = DURACION1 / 58.2;    // distancia medida en centimetros
//  Serial.println(DISTANCIA1);    // envio de valor de distancia por monitor serial
  delay(200);       // demora entre datos


  /*
  El ultrasonico 2 es el de la cantidad de liquido
  */
  // put your main code here, to run repeatedly:
  digitalWrite(TRIG2, HIGH);     // generacion del pulso a enviar
  delay(1);       // al pin conectado al trigger
  digitalWrite(TRIG2, LOW);    // del sensor
  DURACION2 = pulseIn(ECO2, HIGH);  // con funcion pulseIn se espera un pulso
    // alto en Echo
  DISTANCIA2 = DURACION2 / 58.2;    // distancia medida en centimetros
  //Serial.print("Distancia entre el sensor del liquido y el nivel del liquido");
  //Serial.println(DISTANCIA2);    // envio de valor de distancia por monitor serial
  delay(200);       // demora entre datos

  /*
  Codigo del sensor de peso
  */
  Serial.println("");

  Serial.print("Peso: ");

  String peso_tmp = (String(balanza.get_units(20),3));
  float peso = peso_tmp.toFloat();
  Serial.print(peso);
  Serial.println(" kg");
  
  
  int load = 0;
  if(peso>0.00){
    load = 1;
  }else{
    load = 0;
  }
    if(abierto){
      if(DISTANCIA1<distancia_puerta && load>0){
        Serial.println("Squirtle anti covid!: usa pistola de desinfectante");
        Serial.println("Aplicando desinfectante");
        
        digitalWrite(PIN_AGUA, HIGH);     // generacion del pulso a enviar
        delay(5000);       // al pin conectado al trigger
        digitalWrite(PIN_AGUA, LOW);    // del sensor
      }
      abierto=false;
    }
    if(DISTANCIA1>distancia_puerta){
      abierto=true;
    }
    Serial.print("Nivel del desinfectante: ");
    int dist = DISTANCIA2-2;
    int cantidad_liquido = 10-dist;
    int nivel_liquido = 0;
    if(cantidad_liquido>=5){
      nivel_liquido=3;
    }else if(cantidad_liquido>1 && cantidad_liquido<5){
      nivel_liquido=2;
    }else if(cantidad_liquido<=1){
      nivel_liquido=1;
    }
    cantidad_liquido=cantidad_liquido*10;
    Serial.print(cantidad_liquido);
    Serial.println("%");
    Serial.print("Distancia puerta: ");
    Serial.println(DISTANCIA1);
    if(abierto){
      Serial.println("Esta abierto");
    }else{
      Serial.println("Cerrado");      
    }
    if(load){
      Serial.print("Tiene contenido y su peso es: ");
      Serial.println(peso);
    }else{
      Serial.println("Esta vacio");      
    }
    if (peso<=0){
      peso=0;
    }
    String cad = "Enviando: cargado - peso - nivel del liquido";
    String cargado = "false";
    if(load){
      cargado="true";
    }else{
      cargado="false";      
    }
    
//    peso = peso*1000;
//    peso = (int)peso;
    String cad_data = String(cargado) + "-" + String(peso) + "-" + String(nivel_liquido);
  /*  if(cad_data_persev.equals(cad_data)){
      
    }else{
      cad_data_persev = cad_data;      
      Serial2.println(cad_data);
    }*/
    Serial2.println(cad_data);
    Serial.println(cad);
    Serial.print(cad_data);
  delay(1500);
}
