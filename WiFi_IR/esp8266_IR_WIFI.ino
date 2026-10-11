#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>    //https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
//#include <DNSServer.h> // <--- Fondamentale per il Captive Portal
#include <LittleFS.h>
#include <ArduinoJson.h>

// librerie per la gestione del led IR
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

//ESP-01S
//const uint16_t kIrLed = 2; // GPIO2-SCL
/////////////////////////////////////////////////////////////////////////////
// ATTENZIONE CON QUESTE CONFIGURAZIONE NON è POSSIBILE UILIZZARE LA SERIALE
/////////////////////////////////////////////////////////////////////////////
//const uint16_t kIrLed = 3; // GPIO3-RX
//const uint16_t kIrLed = 1; // GPIO1-TX

//ESP-12F
//const uint16_t kIrLed = 5; // GPIO5-SCL -> ESP-12F
const uint16_t kIrLed = 14; // D5 - GPIO14-HSPI-clk - CLK -> ESP-12F

//IRsend irsend(kIrLed);

// in caso di inversione del output al GPIO il parametro va a true
IRsend irsend(kIrLed, true);

// codici di configurazione IR per il Sony
#include "IRCodeConf_sony.h"

// inizializzazione dei parametri di accesso
boolean connected;                //flag to signal active connection

ESP8266WebServer server(80);
//DNSServer dnsServer;

void setup()
{
 delay(5000); // per permettere di attivare l'interfaccia seriale
 Serial.begin(115200);
 delay(100); 

 Serial.printf("\n\nsetup() - Reset Reason: %s\n", ESP.getResetReason().c_str());

 pinMode(kIrLed, OUTPUT); // necessario per spegnere il led IR
 digitalWrite(kIrLed, HIGH); // necessario per spegnere il led IR
 delay(100); // Piccolo ritardo per stabilizzare la linea
 
 Serial.println("setup() - reset porta output eseguito");

// Inizializzazione File System LittleFS
 if (LittleFS.begin()) // necessario per gestire i file web
 {
  Serial.println("\nsetup() - LittleFS montato correttamente.");
  connected = initWiFi();

  if (connected)
  {
   Serial.println("\nsetup() - WiFi connesso.");
   //setup the web server and the over the air update
   setup_webserver();
   setup_ota();
   irsend.begin();
  } 
  else
   Serial.println("\nsetup() - Errore durante la connessione WiFi.");
 } 
 else
  Serial.println("\nsetup() - Errore durante il montaggio di LittleFS!");
}

void loop()
{
 //Check over the air update
 ArduinoOTA.handle();

 //Check for http requests
 webserver_loop();
 delay(100);
}
