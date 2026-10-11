# WiFi_IR

## Obiettivi
L'obiettivo del progetto è quello di sostituire un normale telecomando TV a infrarossi di un "vecchio" TV Sony con un piccolo dispositivo che gestisca l'emissione dei treni di impulsi IR, grazie a comandi ricevuti attraverso una semplice interfaccia web. Pre-requisiti sono:
* l'uso di un piccolo controllore ESP8266, nello specifico un ESP-12F che offre 4 MB di memoria totale e alcune porte in più rispetto al ESP-01S, che pure dispone di un solo MB di memoria.
* la generazione dei treni di impulsi verso il LED IR offerto tramite le librerie IRremoteESP8266 e IRsend (https://github.com/crankyoldgit/irremoteesp8266).
* l'accesso web offerto tramite la libreria ESP8266WebServer (https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
* l'utilizzo di tecniche di comunicazione con il client web tramite delle semplici chiamate di tipo webservice, delegando alla interfaccia sviluppata in HTML, tutta la complessità di interpretazione del comportamento utente
* la possibilità di poter gestire tale prestazione oltre che attraverso la selezione di bottoni, anche tramite semplici comandi vocali.

## Configurazione hardware
Data la presenza di possibili interferenze in media e alta frequenza (dell'ordine dei MHz) e a valle di numerose prove di compatibilità, si è ritenuto necessario separare fisicamente il modulo di gestione del LED a InfraRossi con quello di governo e interfaccia del microcontrollore, in modo eventualmente da poter utilizzare diversi modelli di ESP8266.
Gli schemi di collegamento quindi sono almeno due: uno propriamente di "potenza", per la gestione diretta del LED IR, e uno di alimentazione e interfacciamento del microcontrollore e in particolare quello specifico per il ESP-12F.

![schema elettrico cricuito di potenza](https://github.com/robertopapi/ESP8266-01s/blob/fcc14a7b672705b6838940ba3d4204f068edf33b/WiFi_IR/20260930%20IR%20POWER%20RED.png)

La ragione per cui si è prestata molta cura nel cercare di separare elettricamente i due circuiti è stata data da diversi fattori e tra questi:
* dalla diversa alimentazione, 3.3v per il microcontrollore e 5v per il LED IR,
* dalle interferenze del circuito ad alta potenza nei confronti del microcontrollore ed in particolare per diversi problemi legati all'accesso della memoria flash, problemi durante l'avvio del microcontrollore, dipendente dallo stato delle porte collegate, e diversi altri che rendevano il circuito poco stabile.
Da questo la necessità di disaccoppiare la porta del microcontrollore dal resto del circuito tramite l'optoisolatore 6N137, la cui scelta è stata dettata dalle specifiche prestazioni in media ed alta frequenza.
L'uso del transistor NPN 2N2222 (che potrebbe essere sostituito anche con un transistor PNP che tuttavia richiederebbe la inversione della polarizzazione del circuito e alcune altri ritocchi al software), si è reso necessario per gestire opportunamente la richiesta di corrente da parte del LED IR e non gravare sul optoisolatore.

![schema elettrico cricuito di controllo del microcontrollore](https://github.com/robertopapi/ESP8266/blob/09eacabcb990314327c1ac017bc2e53ee9efac2b/WiFi_IR/20261009%20IR%20micro%20RED.png)

Lo schema che rappresenta il circuito di governo del microcontrollore presenta, come è evidente, un connettore USB dedicato distinto da quello on-board della scheda ESP-12F, per permettere un maggiore disaccoppiamento tra l'alimentazione del microcontrollore e il circuito di potenza, benché inevitabilmente la massa è in comune (d'altra parte avere due alimentatori distinti per un circuito così modesto non era il caso). Il circuito RC posto sul pin RST è per limitare l'effetto bouncing al momento dell'alimentazione del circuito che a volte non permetto lo startup pulito del circuito. I ponticelli invece sono stati inseriti per avere la possibilità di utilizzare GPIO diversi all'occorenza.
Il connettore rappresentato in alto in questo secondo schema corrisponde a quello del circuito di potenza ma con un diverso ordine dei pin, per ragioni di pulizia grafica dello schema.

## Software
Durante la realizzazione del sistema, sono stati riscontrati diversi problemi legati all'accesso alla memoria flash. I problemi erano probabilmente dovuti al fatto che il software accedeva a tale memoria quando il microcontrollore era ancora in fase di avvio, fase evidentemente delicata, soprattutto in presenza di spike di corrente, cali di tensione e fenomeni simili. Per tanto lo sviluppo del software è stato teso a limitare il più possibile, se non ad escluderlo completamente, l'acceso alla memoria flash, anche se questo ha richiesto l'implementazione del filesystem LittleFS, dove memorizzare i file esposti dal webserver.

Un altro accorgimento, utile durante la fase di sviluppo, è stato quello di definire un vocabolario editabile, dove specificare le parole chiave che il software di interpretazione del parlato (in italiano suona proprio male: parlo del speech to text) utilizza per attivare i singoli comandi che vanno trasformati in sequenze di impulsi per il led IR.

Il modulo principale dello sketch è molto semplice e si limita a caricare le librerie, attivare l'interfaccia verso la rete e il webserver.

Il modulo specifico per l'accesso alla rete e la gestione del webserver, prevede l'utilizzo delle librerie standard, senza approfittare delle librerie più evolute, come wifimanager, per cui si è preferito riscrivere le parti che gestiscono le diverse casistiche di accesso, come la prima attivazione in modalità AccessPoint per permettere la memorizzazione delle credenziali dela rete wifi.

Come anticipato in questo secondo modulo si è preferito utilizzare un piccolo file json per memorizzare le credenziali wifi al posto di approfittare della memoria flash, a causa dei problemi di accesso alla memoria flash.

Il modulo di gestione dell'aggiornamento del software nella modalità OTA è quasi vuoto ma può essere arricchito con delle diagnostiche aventi la seriale come output.

Il modulo più delicato è il file header contenente i codici che devono essere trasmessi da led IR corrispondenti ai tasti del telecomando TV.

I codici sono stati determinati con un circuito diverso che ha permesso appunto la rilevazione dei trani di impulsi emessi dal telecomando e che sono stati memorizzati in una struct specifica.

Ciascuna sequenza corrisponde ad un elemento di un array il cui indice è utilizzato per la scambio di informazioni tra pagina web e software che materialmente emette la sequenza corrispondente che va ad attivare il led IR.

La sequenza binaria corrispondente a ciascun codice in questo specifico caso è stata "ruotata", a causa del fatto che, benchè è stata utilizzata la stessa libreria software, il circuito di rilevazione dei codici emessi dal telecomando fisico è stato realizzato con un classico arduino mentre il presente circuito è stato realizzato con il nostro ESP-12F. Non mi è chiaro perché, ma il passaggio da una tecnologia a l'altra ha richiesto appunto tale intervento di rotazione dell'ordine di emissione dei singoli bit delle sequenze.

# Funzionamento
Collegati i componenti e alimentato i circuiti, una volta che il browser web client interrroga il server.

## Conclusioni

 



