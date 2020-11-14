const int Trigger = 2;   //Pin digital 2 para el Trigger del sensor
const int Echo = 3;   //Pin digital 3 para el echo del sensor

const int pinAgua = 7;
double distancia = 0;
/*
David Enriquez
201602744
*/

void medirDistancia(){
  long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros
  long total = 0;
  for(int i=0;i<20;i++){
    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(Trigger, LOW);
    
    t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
    d = t/59;             //escalamos el tiempo a una distancia en cm
    
    Serial.print("Distancia: ");
    Serial.print(d);      //Enviamos serialmente el valor de la distancia
    Serial.print("cm");
    Serial.println();
   // delay(100);          //Hacemos una pausa de 100ms    
    total+=d;
  }
  distancia = total/20;
}

void rocear(){
  Serial.println("Roceaar");
  digitalWrite(pinAgua, LOW);
  delay(1000);          //Enviamos un pulso de 10us
  digitalWrite(pinAgua, HIGH);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//iniciailzamos la comunicación
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
  //Pin agua
  pinMode(pinAgua, OUTPUT);
  digitalWrite(pinAgua, HIGH);//Inicializamos el pin con 0
}

void loop() {
  medirDistancia();
  if(distancia<15){
    delay(2500);
    medirDistancia();
    if(distancia<15){
      while(distancia<100){
        medirDistancia();
      }      
      rocear();
    }
    
  }
}
