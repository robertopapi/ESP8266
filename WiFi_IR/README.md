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

Il risultato del montaggio è rappresentato dalle foto di seguito

![Montato](https://github.com/robertopapi/ESP8266/blob/09eacabcb990314327c1ac017bc2e53ee9efac2b/WiFi_IR/20261009%20IR%20micro%20RED.png)
![Esploso](https://github.com/robertopapi/ESP8266/blob/09eacabcb990314327c1ac017bc2e53ee9efac2b/WiFi_IR/20261009%20IR%20micro%20RED.png)


## Software

### Scelte generali
Durante la realizzazione del sistema, sono stati riscontrati diversi problemi legati all'accesso alla memoria flash. I problemi erano probabilmente dovuti al fatto che il software accedeva a tale memoria quando il microcontrollore era ancora in fase di avvio, fase evidentemente delicata, soprattutto in presenza di spike di corrente, cali di tensione e fenomeni simili. Per tanto lo sviluppo del software è stato teso a limitare il più possibile, se non ad escluderlo completamente, l'acceso alla memoria flash, anche se questo ha richiesto l'implementazione del filesystem LittleFS, dove memorizzare le credenziali di accesso alla rete wifi (data/json/c.json) i file esposti dal webserver (a partire dalla directory data, dove sono memorizzati i file html, e dalle subdirectory css, js e json).

Un altro accorgimento, utile durante la fase di sviluppo, è stato quello di definire un vocabolario editabile (data/json/vocabulary.json), dove specificare le parole chiave che il software di interpretazione del parlato (in italiano suona proprio male: parlo del speech to text) utilizza per attivare i singoli comandi che vanno trasformati in sequenze di impulsi per il led IR.

La logica di funzionamento del sistema è stata distribuita in modo da gravare il meno possibile sulle prestazioni del microcontrollore. per tanto tutta la logica relativa alla interpretazione dei comandi vocali e tutta la complessità della interfaccia utente è stata allocata lato interfaccia web mentre al piccolo microcontrollore è delegato la gestione dell'interfaccia di rete, la pubblicazione delle pagine web e la scarna gestione dell'invio degli impulsi IR.

Il modulo principale dello sketch (esp8266_IR_WIFI.ino) è molto semplice e si limita a caricare le librerie, attivare l'interfaccia verso la rete e il webserver.

Il modulo specifico per l'accesso alla rete e la gestione del webserver (webserver.ino), prevede l'utilizzo delle librerie standard, senza approfittare delle librerie più evolute, come wifimanager, per cui si è preferito riscrivere le parti che gestiscono le diverse casistiche di accesso, come la prima attivazione in modalità AccessPoint per permettere la memorizzazione delle credenziali dela rete wifi. Come anticipato in questo secondo modulo si è preferito utilizzare un piccolo file json per memorizzare le credenziali wifi al posto di approfittare della memoria flash, a causa dei problemi di accesso alla memoria flash.

Il modulo di gestione dell'aggiornamento del software nella modalità OTA (ota.ino) è quasi vuoto ma può essere arricchito con delle diagnostiche aventi la seriale come output.

Il modulo più delicato è il file header (IRCodeConf_sony.h) contenente i codici che devono essere trasmessi dal led IR, corrispondenti ai tasti del telecomando TV.

I codici sono stati determinati con un circuito diverso che ha permesso appunto la rilevazione dei treni di impulsi emessi dal telecomando e che sono stati memorizzati in una struct specifica.

La sequenza binaria corrispondente a ciascun codice è "ruotata" via software con la funzione LSB2MSB(), a causa del fatto che il circuito di rilevazione dei codici emessi dal telecomando fisico è stato realizzato con un classico arduino mentre il presente circuito è stato realizzato con il nostro ESP-12F, benché è stata utilizzata la stessa libreria software. Non mi è chiaro il motivo, ma il passaggio da una tecnologia a l'altra ha richiesto appunto tale intervento di rotazione dell'ordine di emissione dei singoli bit delle sequenze.

Ciascuna sequenza corrisponde ad un elemento di un array il cui indice è utilizzato per la scambio di informazioni tra pagina web e software che materialmente emette la sequenza corrispondente che va ad attivare il led IR.

### Il vocabolario
Come detto prima, il "vocabolario" definisce le parole chiavi corrispondenti ai singoli tasti del teleecomando che possono essere premuti. Va precisato tuttavia che la pressione dei tasti del telecomando è subordinata alla funzione-contesto attivo in quel momento sul televisore e che il nostro piccolo sistema ovviamente non conosce. Da ciò la necessità di suddividere i comandi in alcune categorie che riassumo di seguito:

* comandi "diretti" (direct) - sono i comandi che attivano una determinata funzione, che corrispondono esattamente al tasto del telecomando (eccetto uno di cui spiego più avanti) e che non dipendono direttamente dal contesto attivo in quel momento sulla tv.
* comandi "di navigazione" (navigation) - sono i comandi dell'ipotetico cursore (es. freccia su e giù), che pure corrispondono ai tasti del telecomando (eccetto uno come nella categoria precedente) e che tuttavia sono efficaci solo nell'ambito dei contesti che prevedono questo tipo di navigazione.
* comandi "programmi-canali" (program-channels) - sono comandi "virtuali" che NON corrispondono ai singoli tasti del telecomando ma a loro brevi sequenze (banalmente corrispondonon al canale memorizzato sulla tv che può essere costituito da una o più cifre)

Tutti i comandi hanno l'attributo "repeat" che sta ad indicare al software che se posto a vero, a seguire potrebbe esserci il numero delle volte che tale comando (cioè virtualmente tale tasto del telecomando) deve essere ripetuto (ad esempio "DIMINUISCI il volume di 3" sta a significare che il tasto VOL_DOWN, cioè il treno di impulsi IR corrispondenti, deve essere ripetuto tre volte). Laddove l'attributo repeat è falso allora l'eventuale numero presente nel testo del comando vocale deve essere ignorato, ovvero deve essere trattato in modo particolare in base a quanto descritto poi.

Tra i comandi diretti e quelli di navigazione ci sono due comandi speciali che hanno codice rispettivamente 99 e 98 e che sono necessari per l'intepretazione dei comandi vocali successivi.
Nello specifico il comando "CHANNEL-PAGE" (codice 99) dice al software che il testo corrispondente al comando vocale contiene un numero che deve essere interpretato non come il numero di volte che il treno di impulsi IR deve essere ripetuto ma come il numero di canale o pagina che deve essere inviato sottoforma di una sequenza di numeri/tasti del telecomando (ad esempio "metti il CANALE 32" sta a significare che devono essere emessi di seguito i treni di impulsi IR corrispondenti ai tasti "3" e "2").
Il secondo comando speciale che è solo una commodity, il comando "MORE", sta ad indicare al software che deve essere ripetuto il comando vocale precedente che è stato pronunciato se ripetibile (repeat == true).
Per finire l'elenco dei programmi risente dell'ordine di memorizzazione dei canali sul televisore (che non è quella offerta di default normalmente dai televisori), pertanto è stata gestita sempre nel vocabolario per permetterne la personalizzazione (la a scelta di uno sfondo di background dei tasti per editore è del tutto personale).

### Interfaccia utente
la GUI è suddivisa in tre parti
* un header con il titolo (e un bottone nascosto che se cliccato mostra un textbox con alcuni testi di diagnostica del codice javascript)
* un corpo centrale con le diverse raprresentazioni dei comandi selezionabili con il gesture swipe (per pigrizia ho utilizzato la libreria javascript (https://github.com/john-doherty/swiped-events) per facilitarmi il compito della implememtazione
* un footer con una div normalmente nascosta che contiene la risposta del server al comando inviato e i piccoli tasti di selezione delle pagine del corpo centrale

Nel corpo centrale l'ultima pagina da la possibilità di modificare e salvare il vocabolario, sia editandolo direttamente che permettendo di caricare un file. Ovviamente il formato del json deve essere rigorosamente rispettato.

Di seguito gli screenshot delle singole pagine

![comandi vocali](https://github.com/robertopapi/ESP8266/blob/d8abb9b4873a8ba08e6ad45b32e8c074ce7c7d3a/WiFi_IR/Screenshot_20261012_092616_Chrome.jpg)
![tasti](https://github.com/robertopapi/ESP8266/blob/3b53863d000733e056cd2832a5466b588742b160/WiFi_IR/Screenshot_20261012_092637_Chrome.jpg)
![scorciatoie](https://github.com/robertopapi/ESP8266/blob/3ef1ef6fe0f1d6b2a3bd31c65d9db7dff08c2de9/WiFi_IR/Screenshot_20261012_092648_Chrome.jpg)
![vocabolario](https://github.com/robertopapi/ESP8266/blob/1ac65b33f3dec39886d34fa02804662483117853/WiFi_IR/Screenshot_20261012_092813_Chrome.jpg)


# Funzionamento
La comodità di avere un telecomando virtuale ovviamente sta nel poterlo utilizzare col telefono cellulare e l'interfaccia web è stata sviluppata con questo approccio. Tuttavia vanno fatte due premesse importanti.
Per ora l'unico browser in grado di fornire la prestazione di speech2text sena limitazioni è il browser chrome. Va pure specificato che quest'ultimo, in base alle moderne normative in fatto di sicurezza, non permette l'attivazione del microfono da parte di una pagina web se non pubblicata via https, cosa che sul piccolo ESP-12F è particolarmente gravosa e complessa da implementare. La soluzione è quella di intervenire sui flag di personalizzazione di chrome ed in particolare scrivendo su la barra di ricerca il comando

chrome://flags/#unsafely-treat-insecure-origin-as-secure

e specificando l'indirizzo IP considerato sicuro e seguendo la procedura indicata per l'attivazione del flag

Ulteriore limitazione è data dal tempo di attivazione del microfono che sui cellulare è di poche decine di secondi. A questo ho provveduto con il tasto "cont" presente nella GUI e che se selezionato, riattiva il microfono automaticamente. Tale prestazione diventa utile sopratutto durante la navigazione nei menu contestuali.

 



