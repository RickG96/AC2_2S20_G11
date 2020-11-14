#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
/*
David Enriquez
201602744
*/

const char* ssid = "TheSkyIsTheLimit";
const char* password = "FamEnriquez$$";
char jsonOutput [128];

/*
  Capitulo 5 de Arduino desde cero en Español
  Primer programa que envía mediante el Monitor Serial el valor de distancia
  leído por el sensor ultrasónico HC-SR04.

  Autor: bitwiseAr  

*/

#define RXD2 16
#define TXD2 17

String recibido = "";

void enviar_data(){
  delay(100);

  String rex = recibido;
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient client;
    
    client.begin("http://64.227.23.182:3000/infoed3");
    client.addHeader("Content-Type","application/json");
    
    const size_t CAPACITY = JSON_OBJECT_SIZE(2);
    StaticJsonDocument<CAPACITY> doc;
    
    JsonObject object = doc.to<JsonObject>();
    object["data"] = recibido;
    
    Serial.println("recibido");
    Serial.println(recibido);
  
    serializeJson(doc,jsonOutput);
      
    int httpCode=client.POST((String(jsonOutput)));
    
    if (httpCode>0){
      String payload = client.getString();
      Serial.println("\nStatuscode: "+String(httpCode));
      Serial.println(payload);
      client.end();
    }else{
      Serial.println("Error on HTTP request");
    }
  }else{
    Serial.println("Connection lost");
    WiFi.begin(ssid,password);
    Serial.print("connecting to wifi!!");
  }
}

void leer_data(){
  delay(100);
  Serial.print("Data recibida en ESP32:");
  recibido = Serial2.readString();
  Serial.println(recibido);
}


void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);// inicializacion de comunicacion serial a 9600 bps
  WiFi.begin(ssid,password);
  Serial.print("Conectando al wifi!");
}

void loop()
{
  leer_data();
    if(recibido!=NULL){
      enviar_data();
    }
    delay(1500);
  Serial.println("Funcionando");
}
