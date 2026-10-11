//
// definisco le variabili che conterranno le credenziali wifi
#define CREDENTIAL "/json/c.json"
String new_ssid = "";
String new_pass = "";

////////////////////////////////////////////////////////////////////////////////////////
// READ/WRITE CREDENTIAL
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////
// readCredential
////////////////////////////////
int readCredential()
{
 // exit code
 // ret=0 -> credenziali caricate
 // ret<0 -> errore con error che contiene l'eventuale errore json
 // ret>0 -> credenziali assenti
 
 int ret=0;
 
 StaticJsonDocument<256> jBuffer;
 DeserializationError error;
 
 if (LittleFS.exists(CREDENTIAL))
 {
  File file = LittleFS.open(CREDENTIAL, "r");
  
  if (file) //lettura del file e caricamento delle credenziali nelle variabili, se le stringhe sono vuote esce con errore
  {
   size_t size = file.size();

   // Allocate a buffer to store contents of the file.
   std::unique_ptr<char[]> buf(new char[size]);

   file.readBytes(buf.get(), size);

   error = deserializeJson(jBuffer, buf.get());

   if (error) // file presente ma vuoto o json mal formato
   {
    Serial.println("readCredential() - file credential corrotto");
    file.close();
    ret=-1;
   }
   else
   {
    new_ssid = String(jBuffer["ssid"]);
    new_pass = String(jBuffer["pass"]);
   }
   
   if (new_ssid=="" || new_pass=="") // credenziali assenti: esco con errore perché vengano richieste le nuove credenziali
   {
    Serial.println("readCredential() - credenziali assenti");
    ret=1;
   }
   else
   {
    Serial.println("readCredential() - credentiali presenti");
   }
   file.close();
  }
 }
 else
 {
  Serial.println("readCredential() - credenziali assente");
  ret=1;
 }  
 return (ret);
}

////////////////////////////////
// writeCredential
////////////////////////////////
int writeCredential()
{
 // exit code
 // ret=0 -> credenziali caricate
 // ret<0 -> errore con error che contiene l'eventuale errore json
 
 int ret=0;
 
 String json = "{\"ssid\":\""+new_ssid+"\",\"pass\":\""+new_pass+"\"}";

 if (LittleFS.exists(CREDENTIAL)) // cancello il file se già presente
 {
  if (LittleFS.remove(CREDENTIAL))
   Serial.println("writeCredential() - file credential cancellato");
  else
  {
   Serial.println("writeCredential() - accesso credential fallito");
   ret=-1;
   return (ret);
  }
 }

 File file = LittleFS.open(CREDENTIAL, "w");

 if (file)
 {
  file.print(json);
  file.close();
  Serial.println("writeCredential() - credentiali aggiornate");
 }
 else
 {
  Serial.println("writeCredential() - creazione file credentiali fallita");
  ret=-1;
 }

 return (ret);
}

////////////////////////////////////////////////////////////////////////////////////////
// setup
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////
//setup normal
////////////////////////////////
void setup_webserver()
{
 Serial.println("\nsetup_webserver()");
 
 server.onNotFound([]()
  {
   if (!handleGeneric(server.uri()))
   {
    server.send(404, "text/plain", String("404: File "+server.uri()+" Not Found"));
   }
  });

 server.on("/cmd/inline", [] ()
  {
   server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
   server.sendHeader("Connection", "close");
   server.send(200, "text/plain", "this works as well");
  } 
 );

 server.on("/cmd/direct", handleDirectCommand);
 
 server.on("/cmd/savevocab", handleSaveVocab);

 // esegue solo il restart del microcontrollore
 server.on("/cmd/restart",restart);

 // esegue la pulizia dei dati di accesso al wifi e il restart del microcontrollore
 server.on("/cmd/reset",reset);

 //start webserver
 server.begin();
}

////////////////////////////////////////////
// configurazione alternativa alla normale per gestire la configurazione dei parametri di accesso al wifi
////////////////////////////////////////////
void setup_webserverinitwifi()
{
 Serial.println("\nsetup_webserverinitwifi()");
 
 server.onNotFound([]()
  {
    server.send(404, "text/plain", String("404: File "+server.uri()+" Not Found"));
  });
 
 server.on("/", HTTP_GET, handleInitWiFi);
 
 server.on("/inline", []()
  {
   server.send(200, "text/plain", "this works as well");
  }
          );


 server.on("/cmd/saveconfwifi", HTTP_POST, handleSaveConfWiFi);

 //start webserver
 server.begin(); 
}

//called from main loop
void webserver_loop()
{
 server.handleClient();
}

// --- DETERMINAZIONE CONTENT-TYPE ---
String getContentType(String filename)
{
 if (filename.endsWith(".html"))
  return "text/html";
 else if (filename.endsWith(".css"))
  return "text/css";
 else if (filename.endsWith(".js"))
  return "application/javascript";
 else if (filename.endsWith(".json"))
  return "application/json";
 else if (filename.endsWith(".ico"))
  return "image/x-icon";
 else if (filename.endsWith(".jpg"))
  return "image/jpeg";
 else if (filename.endsWith(".jpeg"))
  return "image/jpeg";
 else if (filename.endsWith(".svg"))
  return "image/svg+xml";
 return "text/plain";
}
//handle generic request
bool handleGeneric(String path)
{
 Serial.println("\nhandleGeneric("+path+")");
 
 server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
 server.sendHeader("Connection", "close");

 if (connected)
 {
  if (path.endsWith("/")) path += "index.html"; // da creare
  
  String contentType = getContentType(path);
  
  if (LittleFS.exists(path))
  {
   File file = LittleFS.open(path, "r");
   server.streamFile(file, contentType);
   file.close();
   return true;
  }
  else
  {
   Serial.println("\nhandleGeneric("+path+") - 404: File "+path+" Not Found");
   
   server.send(404, "text/plain", String("404: File "+path+" Not Found"));
  }
 }
 else
 {
  Serial.println("\nhandleGeneric("+path+") - 200: Not connected");
  
  server.send(200, "text/plain", "200: Not connected");
 }
 return false;
}

//AJAX command /cmd/direct
////////////////////////////
// handleDirectCommand()
////////////////////////////
void handleDirectCommand()
{
//////////////////////////
//
// gestisce comandi diretti richiamati tramite browser con la seguente sintassi
// 192.168.1.70/cmd/direct?cmd=1
// dove 1 corrisponde all'indice del comando corrispondente (es. 1=IR_ON_OFF)
//
//////////////////////////

 Serial.println("\nhandleDirectCommand()");

 uint8_t cmd;

 cmd = server.arg("cmd").toInt();
 
 MyIRSend(cmd);
 
 server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
 server.sendHeader("Connection", "close");
 server.send(200, "text/plain", String("tasto "+str_IR_Key[cmd]+" premuto"));
}

// --- ENDPOINT SALVATAGGIO VOCABOLARIO: /save-vocab ---
void handleSaveVocab()
{
 Serial.println("\nhandleSaveVocab()");
 
 if (server.hasArg("plain"))
 {
  String jsonContent = server.arg("plain");
    
  File file = LittleFS.open("/json/vocabulary.json", "w");
  if (file)
  {
   file.print(jsonContent);
   file.close();

   Serial.println("\nhandleSaveVocab() - VOCABOLARIO SALVATO");
   server.send(200, "text/plain", "VOCABOLARIO SALVATO");
   return;
  }
 }
 Serial.println("\nhandleSaveVocab() - ERRORE SCRITTURA FILE");
 server.send(500, "text/plain", "ERRORE SCRITTURA FILE");
}

//AJAX command /cmd/reset
void reset()
{
 Serial.println("\nreset()");
 ESP.eraseConfig(); // cancella il contenuto della EPROM compresi i parametri di accesso alla rete wifi
 delay(5000);
 ESP.restart();
}

//AJAX command /cmd/restart
void restart()
{
 Serial.println("\nrrestart()");
 delay(5000);
 ESP.restart();
}

////////////////////////////////
// InitWiFi
////////////////////////////////
void handleInitWiFi()
{
 Serial.println("\nhandleInitWiFi()");
 String path;
 
 server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
 server.sendHeader("Connection", "close");
 server.sendHeader("Pragma", "no-cache");
 server.sendHeader("Expires", "-1");

 path = "initWiFi.html"; // nuovo file da creare
  
 String contentType = getContentType(path);
  
 if (LittleFS.exists(path))
 {
  File file = LittleFS.open(path, "r");
  server.streamFile(file, contentType);
  file.close();
 }
 else
 {
  Serial.println("\nhandleInitWiFi() - 404: File "+path+" Not Found");
  
  server.send(404, "text/plain", String("404: File "+path+" Not Found"));
 }
}

void handleSaveConfWiFi()
{
 Serial.println("\nhandleSaveConfWiFi()");
 
 int ret=0;

 if (server.hasArg("ssid") && server.hasArg("password"))
 {
  new_ssid = server.arg("ssid");
  new_pass = server.arg("password");

  ret = writeCredential();
  
  if (ret < 0)
  {
   Serial.println("\nhandleSaveConfWiFi() - errore durante la creazione del file credential");
   return;
  } 
    
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Connection", "close");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  server.send(200, "text/html", "<!DOCTYPE html><html><body><h3>Dati ricevuti! Riavvio in corso...</h3></body></html>");

  delay(2000);
    
  // Passiamo i dati all'SDK che li memorizzerà nella flash interna
  WiFi.persistent(true);
  
  // Forzo la modalità Station PRIMA di salvare
  WiFi.mode(WIFI_STA);

  // Salva le credenziali e la modalità STA nel settore riservato di Flash
  WiFi.begin(new_ssid.c_str(), new_pass.c_str());
  
  // ciclo d'attesa che aspetta l'effettiva connessione. In questo modo si è sicuri al 100% che l'SDK abbia completato l'handshake e scritto i dati in Flash
  uint8_t cnt = 0;
  while ((WiFi.status() != WL_CONNECTED) && (cnt<20))
  {
   delay(500);
   cnt++;
  }
 // Un secondo extra di margine per la scrittura fisica
  delay(1000);
  
  // Riavvia il microcontrollore per tentare la connessione nel setup()
  ESP.restart();
 }
}

//////////////////////////////////////////////
// WLAN
//////////////////////////////////////////////

//attivazione Station mode
void activateStationMode()
{
 Serial.println("\nactivateStationMode()");
 
 WiFi.mode(WIFI_AP);
 
 // Definiamo i parametri di rete espliciti
 IPAddress local_IP(192, 168, 4, 1);
 IPAddress gateway(192, 168, 4, 1);  // L'ESP fa da gateway
 IPAddress subnet(255, 255, 255, 0);

 // Applichiamo la configurazione all'Access Point
 WiFi.softAPConfig(local_IP, gateway, subnet);
 
 WiFi.softAP("irwifi");
 delay(100);

 // avvio il web server per gestire la configurazione dei parametri di accesso wifi
 setup_webserverinitwifi();
 // Loop temporaneo bloccante finché l'utente non inserisce i dati
 while (true)
 {
  server.handleClient();
  yield(); // Evita il reset del Watchdog dell'ESP8266
 }
}

//connect to the WLAN
boolean initWiFi()
{
 Serial.println("\ninitWiFi()");
 
 int ret;
 boolean connected = false;
 delay(200);

 WiFi.persistent(false);
 WiFi.setSleepMode(WIFI_NONE_SLEEP);

 //first disconnect
 WiFi.disconnect();
 WiFi.softAPdisconnect(true);
 delay(100);
 
 ret = readCredential();
 if (ret > 0) // assenza di credenziali
 {
  Serial.println("\ninitWiFi() - credenziali assenti");
  activateStationMode(); // se non ho connessione o parametri di configurazione o i parametri sono errati attivo la modalità access point e richiedo le credenziali
  return connected;
 }
 else if (ret < 0) // file delle credenziali corrotto
 {
  Serial.println("\ninitWiFi() - file delle credenziali corrotto");
  return connected;
 }
 
 WiFi.mode(WIFI_STA);
 delay(100);

 // Chiamo WiFi.begin() CON i parametri letti dal file delle credenziali
 WiFi.begin(new_ssid.c_str(), new_pass.c_str());
 
 delay(500);

 uint8_t cnt = 0;
 while ((WiFi.status() != WL_CONNECTED) && (cnt<40))
 {
  delay(500);
  cnt++;
 }
 if (WiFi.status() == WL_CONNECTED)
  connected = true;
 else
  activateStationMode(); // se non ho connessione o parametri di configurazione o i parametri sono errati attivo la modalità access point e richiedo le credenziali

 Serial.println("\ninitWiFi() - connessione avvenuta dopo "+String(cnt)+" tentativi");

 return connected;
}
