#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

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

void setup()
{
  Serial.begin(115200);     // inicializacion de comunicacion serial a 9600 bps
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  WiFi.begin(ssid,password);
  Serial.print("connecting to wifi!!");
}
String recibido = "";
String recibidox = "";
void loop()
{
  
  delay(100);
  Serial.print("Data recibida:");
  recibido = Serial2.readString();
  //Serial.println(recibido);
  String rex = recibido;
  rex.trim();
  if(rex.equals("")){
  }else{
    recibidox = recibido;
  }
  
    
    if ((WiFi.status() == WL_CONNECTED)){
      HTTPClient client;
      
      client.begin("http://167.71.93.195:3000/cadena");
      client.addHeader("Content-Type","application/json");
      
      const size_t CAPACITY = JSON_OBJECT_SIZE(2);
      StaticJsonDocument<CAPACITY> doc;
      
      JsonObject object = doc.to<JsonObject>();
      object["r"] = recibidox;
      object["recibido2"] = "Hola Rick";
      
      Serial.println("recibido");
      Serial.println(recibido);
    
      Serial.println("recibidox");
      Serial.println(recibidox);
      serializeJson(doc,jsonOutput);
      
      int httpCode=client.POST((String(jsonOutput)));
      
      if (httpCode>0){
        String payload = client.getString();
        Serial.println("\nStatuscode: "+String(httpCode));
        Serial.println(payload);
  
      /*  char json[500];
        payload.replace(" ", "");
        payload.replace("\n","");
        payload.trim();
        
        payload.replace("\n","");
        payload.remove(0,1);
        payload.toCharArray(json,500);
  
        StaticJsonDocument<200> doc;
        deserializeJson(doc,json);
  
        int id = doc["id"];
        const char* email = doc["email"];
        
        Serial.println(String(id) + " - " + String(email) + "\n");
  */


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
   
