#include <Adafruit_MLX90614.h>

#include <Servo.h>
/*
David Enriquez
201602744
*/
//Dispensador alcohol gel
//Motor servo
Servo servoMotor;

// Temp
// Instanciar objeto
Adafruit_MLX90614 termometroIR = Adafruit_MLX90614();

//Pines
const int bom_agua = 7;

//TEMP toma 22 y 23
const int SERVO_ALC_GEL = 30;
const int ULTRA_TEMP_ECHO = 32;
const int ULTRA_TEMP_TRIGGER = 33;

const int INFRA_PUERTA = 35;

//variables utilizadas
int dist_puerta;
int dist_temperatura;
float temperatura_persona;
float temperatura_ambiente;

bool estado_puerta;
int num_fase = 1;
int usuario_estado_temp = 1;
int desinfectados_objetos = 0;
String cadena_enviar="";
//
void resetDefault(){
  desinfectados_objetos=0;
  temperatura_persona=0;
  temperatura_ambiente=0;
  num_fase=0;
}
void prepararCadenaEnviar(){
  cadena_enviar = "";
  cadena_enviar = cadena_enviar + desinfectados_objetos+"-";
  cadena_enviar = cadena_enviar + temperatura_persona+"-";
  cadena_enviar = cadena_enviar + temperatura_ambiente+"-";
  if (temperatura_persona<37.5){
    cadena_enviar = cadena_enviar+"1-";    
  }else{
    cadena_enviar = cadena_enviar+ "0-";  
  }
  cadena_enviar = cadena_enviar + num_fase+"";
  Serial.println(cadena_enviar);
  Serial3.println(cadena_enviar);
}

void dispensarAlcohol(){
    // Desplazamos a la posición 0º
  servoMotor.write(0);
  // Esperamos 1 segundo
  delay(1000);
  
  // Desplazamos a la posición 90º
  servoMotor.write(140);
  // Esperamos 1 segundo
  delay(1000);
  servoMotor.write(0);
  delay(1000);

}

void medirTemperatura(){
  // Obtener temperaturas grados Celsius
  float temperaturaAmbiente = termometroIR.readAmbientTempC();
  float temperaturaObjeto = termometroIR.readObjectTempC();
 
  // Mostrar información
  Serial.print("Temp. ambiente => ");
  Serial.print(temperaturaAmbiente);
  Serial.println("ºC");
 
  Serial.print("Temp. objeto => ");
  Serial.print(temperaturaObjeto);
  Serial.println("ºC");
  temperatura_persona = temperaturaObjeto;
  temperatura_ambiente = temperaturaAmbiente;
 
  // Si quieres mostrar la información en grados Fahrenheit utiliza las funciones
  // readAmbientTempF() para temperatura ambiente
  // readObjectTempF() para temperatura del objeto
}

int medirDistancia(int pinEcho,int pinTrigger){
  long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros
  long total = 0;
  float distancia = 0;
  for(int i=0;i<20;i++){
    digitalWrite(pinTrigger, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(pinTrigger, LOW);
    
    t = pulseIn(pinEcho, HIGH); //obtenemos el ancho del pulso
    d = t/59;             //escalamos el tiempo a una distancia en cm
    
    Serial.print("Distancia: ");
    Serial.print(d);      //Enviamos serialmente el valor de la distancia
    Serial.print("cm");
    Serial.println();
   // delay(100);          //Hacemos una pausa de 100ms    
    total+=d;
  }
  distancia = total/20;
  return distancia;
}

bool hayObstaculo(){
  int value = 0;
  value = digitalRead(INFRA_PUERTA );  //lectura digital de pin
 
  if (value == HIGH) {
      Serial.println("Puerta abierta");
  }else{
      Serial.println("Puerta cerrada");    
  }
  return value;
}

void rocear(){
  Serial.println("Roceaaar");
  digitalWrite(bom_agua, LOW);
  delay(1000);          //Enviamos un pulso de 10us
  digitalWrite(bom_agua, HIGH);
}

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
 
  // Pin servo
  servoMotor.attach(SERVO_ALC_GEL);
  // Iniciar termómetro infrarrojo con Arduino
  termometroIR.begin();
 /* pinMode(6, OUTPUT); //pin como salida
  pinMode(7, INPUT);  //pin como entrada
  digitalWrite(6, LOW);//Inicializamos el pin con 0
*/
  pinMode(INFRA_PUERTA , INPUT);

  //ULTRA Temperatura
  pinMode(ULTRA_TEMP_TRIGGER, OUTPUT); //pin como salida
  pinMode(ULTRA_TEMP_ECHO, INPUT);  //pin como entrada
  digitalWrite(ULTRA_TEMP_TRIGGER, LOW);//Inicializamos el pin con 0


  //Pin de agua
  pinMode(bom_agua, OUTPUT);
  digitalWrite(bom_agua, HIGH);
  servoMotor.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  resetDefault();
  bool estado_puerta = hayObstaculo();
  Serial.println(estado_puerta);
  num_fase=0;
  prepararCadenaEnviar();
  delay(100);
  desinfectados_objetos=0;
  if(estado_puerta==1){
    num_fase=1;
    desinfectados_objetos=0;
    prepararCadenaEnviar();
    delay(100);
    while(estado_puerta==1){
      Serial.println("Esperando a cerrar la puerta");
      estado_puerta = hayObstaculo();
      prepararCadenaEnviar();
      delay(100);
    }
    delay(2000);
    dist_temperatura = medirDistancia(ULTRA_TEMP_ECHO,ULTRA_TEMP_TRIGGER);
    while(dist_temperatura>10){
      Serial.println("Esperando a que llegue persona");
      dist_temperatura = medirDistancia(ULTRA_TEMP_ECHO,ULTRA_TEMP_TRIGGER);
    }
    num_fase=2;
    desinfectados_objetos=1;
    prepararCadenaEnviar();
    rocear();
    medirTemperatura();
    delay(3000);
    num_fase=3;
    prepararCadenaEnviar();
    dispensarAlcohol();
    delay(5000);
  }
}
