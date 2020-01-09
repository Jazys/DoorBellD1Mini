#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
Ticker ticker;

//0 pour UDP
//1 pour HTTP
#define MODE 1

#ifndef STASSID
#define STASSID "xxxxx"
#define STAPSK  "xxxxxxx"
#endif

const char* host = "192.168.1.40";
unsigned int port = 8080;      

WiFiUDP Udp;

int start_wake_up=0;

void setup() {

  start_wake_up =millis();
  
  Serial.begin(115200);
  Serial.println(""); 
  Serial.print("Reason startup :");
  Serial.println(ESP.getResetReason());

  //pour jouer avec la led
  pinMode(BUILTIN_LED, OUTPUT);
  ticker.attach(0.5, tick);

  //pour se connecter au wifi
  connectToWifi();

  //pour s'endormir
  sleepForEver();
  
  
}

//Pour arreter de faire clignoter la led et endormir le D1
void sleepForEver()
{
  ticker.detach();
  digitalWrite(BUILTIN_LED, LOW);
  ESP.deepSleep(0);
}

//Pour changer la led d'etat
void tick()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // obtient l'etat
  digitalWrite(BUILTIN_LED, !state);     // inverse l'etat
}


void connectToWifi()
{
  //Indique que c'est un Sonnette
  String reason = "Sonnette_"+ESP.getResetReason();
  
  //converti la raison en char
  char _reason[sizeof(reason)];
  reason.toCharArray(_reason, sizeof(_reason));
  
  int _try = 0;
  //si au bout de 3 sec pas connecté alors ona arrete
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
    
    _try++;
    if ( _try >= 10 ) {
        Serial.println("Impossible to connect WiFi network, go to deep sleep");

        //pour indiquer qu'on ne peut pas se connecter
        ticker.attach(0.2, tick);
        
        int not_connect=0;
        while( not_connect<5)
        {
          delay(100);
          not_connect++;
        }

        //s'endort
        sleepForEver();
    }
  }
  
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  //choix du mode
  if( MODE==1)
  {
    SendFrameToHTTP(reason);
  }
  else
  {
    SendUDP(_reason);
  }
}

//Envoi un paquet UDP
void SendUDP(const char* reason)
{
  
  Udp.beginPacket(host, port);
  Udp.write(reason);
  Udp.endPacket();

  delay(100);
}

//Envoi une trame HTTP
void SendFrameToHTTP(String reason)
{    
    WiFiClient client;
  
    if (!client.connect(host, port)) {
      Serial.println("connection failed");
      return;
    }

    String url = "/hello";
       
    // Envoi la requete au serveur - This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

    //Si pas de réponse au bout de 5 seconde
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
  
    //lit la réponse
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  
}

void loop() { 

}


