var globalWidth;
var globalHeight;
// SpeechRecognition
var SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
if (SpeechRecognition)
{
 var recognition = new SpeechRecognition();
}
var continuous = false;
// SpeechRecognition

let listprgHTML = "";
// swipe
let swipecurrentpos=0;
let swipetagobj;

function loadListener()
{
 document.addEventListener(
                           'swiped',
                           function(e)
                           {
                           }
                          );
 document.addEventListener(
                           'swiped-left',
                           function(e)
                            {
                             if (swipecurrentpos===0)
                              swipecurrentpos=1;
                             else if (swipecurrentpos===1)
                              swipecurrentpos=2;
                             else if (swipecurrentpos===2)
                              swipecurrentpos=3;
                             else
                              return;
                             selectswipe();
                            }
                          );// da destra a sinistra
 document.addEventListener(
                           'swiped-right',
                           function(e)
                            {
                             if (swipecurrentpos===1)
                              swipecurrentpos=0;
                             else if (swipecurrentpos===2)
                              swipecurrentpos=1;
                             else if (swipecurrentpos===3)
                              swipecurrentpos=2;
                             else
                              return;
                             selectswipe();
                            }
                          );// da sinistra a destra
}
function vanish(obj)
{
 obj.innerHTML="";
}

function selectswipe()
{
 let fs = document.getElementById('frameswipe');
 // tolgo la classe a tutti a prescindere per pulizia
 swipetagobj[0].classList.remove("selected");
 swipetagobj[1].classList.remove("selected");
 swipetagobj[2].classList.remove("selected");
 swipetagobj[3].classList.remove("selected");
 // rimetto la classe solo a quello oggetto di selezione
 swipetagobj[swipecurrentpos].classList.add("selected");
 switch(swipecurrentpos)
 {
  case 0:
   fs.style.left="0px"; // prima posizione
   break;
  case 1:
   fs.style.left="-340px"; // seconda posizione
   break;
  case 2:
   fs.style.left="-680px"; // terza posizione
   break;
  case 3:
   fs.style.left="-1020px"; // quarta posizione
   break;
 }
}
/////////////////////////////////////////////////////////////////////////////
const numberWords =
 {
  "uno": 1, "una": 1, "un": 1, "1": 1,
  "due": 2, "2": 2,
  "tre": 3, "3": 3,
  "quattro": 4, "4": 4,
  "cinque": 5, "5": 5,
  "sei": 6, "6": 6,
  "sette": 7, "7": 7,
  "otto": 8, "8": 8,
  "nove": 9, "9": 9,
  "dieci": 10, "10": 10
 };
const numberCode = ["39","29","30","31","32","33","34","35","36","37"];
let vocabulary = null;

let lastCommand = null;
let lastrepeatCount = null;

/////////////////////////////
window.addEventListener('DOMContentLoaded', () =>
 {
  loadVocabulary();
  loadSaveVocabFunction();
  loadListener();
  // inizializzo l'array degli id qui dopo che sono stati creati i tag corrispondenti 
  swipetagobj = [
                 document.getElementById("st0"),
                 document.getElementById("st1"),
                 document.getElementById("st2"),
                 document.getElementById("st3")
                ];
  selectswipe();// inizializzo la posizione
 } 
                       );  
/////////////////////////////
// Caricamento iniziale del vocabolario
async function loadVocabulary()
{
 debug("loadVocabulary()");
 try
 {
  const response = await fetch('/json/vocabulary.json');
  vocabulary = await response.json();
  debug("Vocabolario LittleFS caricato con successo:" + JSON.stringify(vocabulary));
  
  buildVocabContent();
 }
 catch (err)
 {
  debug("Errore nel caricamento del vocabolario:" + String(err));
 }
}
// Funzione principale di interpretazione dello Speech
function processVoiceCommand(transcript)
{
 if (!vocabulary)
 {
  debug("Vocabolario non ancora pronto!");
  return;
 }

// cancello tutto prima di iniziare
 document.getElementById("cleanText").value = "";
 document.getElementById("matchedCommand").value = "";
 document.getElementById("repeat").value = "";

 const cleanText = transcript.toLowerCase().trim();
 debug("processVoiceCommand(" + cleanText + ")");
 document.getElementById("cleanText").value = cleanText;

 let matchedCommand = null;
 let repeatCount = 1; // 2. Estrazione del moltiplicatore/quantità, Default: 1 sola volta
 
 // prima verifico se il comando è nella lista dei nomi dei programmi
 
 const allChannels = Object.values(vocabulary.program.channels).flat();

 for (const cmd of allChannels)// 1. Ricerca del comando corrispondente
 {
  const found = cmd.keywords.some(kw => cleanText.includes(kw));
  if (found)
  {
   sendIrNumbers(cmd.chnum.trim());
   debug("Comando: " + cmd.prg + "(canale: " + cmd.chnum + ") Ripetizioni: " + repeatCount);
   document.getElementById("matchedCommand").value = String(cmd.prg);
   document.getElementById("repeat").value = String(repeatCount);
   return; // esco dalla funzione
  }
 }

 // se NON è nella lista dei programmi allora lo cerco come comando semplice

 // Unisco i comandi (direct e navigation) per la ricerca
 const allCommands = Object.values(vocabulary.commands).flat();

 for (const cmd of allCommands)// 1. Ricerca del comando corrispondente
 {
  const found = cmd.keywords.some(kw => cleanText.includes(kw));
  if (found)
  {
   matchedCommand = cmd;
   break;
  }
 }

 if (!matchedCommand)
 {
  debug("Nessuna corrispondenza trovata per: " + cleanText);
  return;
 }

 const words = cleanText.split(/\s+/);

 let s = "";
 for (let n=0;n<words.length;n++)
 {
  s += words[n];
  s += ",";
 }
 debug("words:"+String(s));
 
 if (matchedCommand.code===99) // sono canali o pagine
 {
  let snum="";
  for (let n=0;n<words.length;n++)
  {
   if (!(isNaN(words[n]))) // è un numero
   {
    snum=words[n];
    break;
   }
  }
  debug("num="+snum);
  
  if (snum!="")
   sendIrNumbers(snum);
  else
   debug("numero canale/pagina non trovato");
  
  lastCommand=null;
  lastrepeatCount=null;
 }
 else if (matchedCommand.code===98) // ripete il comando precedente
 {
  if (lastCommand != null && lastrepeatCount !=null)
   sendBurstIrCommands(lastCommand, lastrepeatCount);
 }
 else
 { 
  if (matchedCommand.repeat)
  {
   debug("selezione ripetibile");
   
   for (const word of words)
   {
    if (numberWords[word])
    {
     repeatCount = numberWords[word];
     break; // Prende il primo numero valido trovato
    }
   }
   // memorizza il comando e il numero delle ripetizioni per eventuali ripetizioni successive
   lastCommand = matchedCommand.code;
   lastrepeatCount = repeatCount;
  }
  else
  {
   lastCommand=null;
   lastrepeatCount=null;
  }

  // 3. Invio della sequenza al microcontrollore
  sendBurstIrCommands(matchedCommand.code, repeatCount);
 }
 
 debug("Comando: " + matchedCommand.action + "(Codice: " + matchedCommand.code + ") Ripetizioni: " + repeatCount);
 document.getElementById("matchedCommand").value = String(matchedCommand.action);
 document.getElementById("repeat").value = String(repeatCount);
}

function sendBurstIrCommands(code, count, delayMs = 180)
{
 debug("sendBurstIrCommands("+String(code)+", "+String(count)+", "+String(delayMs)+")");
 let executed = 0;

 function sendNext()
  {
   if (executed >= count)
    return;
   
   callServer("resp", "/cmd/direct", "cmd=" + code); // la risposta del server va a finire sempre sulla div resp

   executed++;

   if (executed < count)
   {
    setTimeout(sendNext, delayMs); // Pausa per far respirare il ricevitore IR della TV
   }
  }

 sendNext();
}

function sendIrNumbers(snum)
{
 debug("sendIrNumbers("+snum+")");

 let arr=snum.split("");
 for(let n=0;n<arr.length;n++)
 {
  debug("arr["+String(n)+"]="+arr[n]+" numberCode="+numberCode[parseInt(arr[n])]);
  callServer("resp", "/cmd/direct", "cmd=" + numberCode[parseInt(arr[n])]);
 }
}

function buildVocabContent()
{
 debug("buildVocabContent()");
 
 const allChannels = Object.values(vocabulary.program.channels).flat();

 listprgHTML="";
 
 for (const program of allChannels)
 {
  listprgHTML+='<div class="frameprg ';
  listprgHTML+=program.bg.trim()+'" ';
  listprgHTML+='onclick="javascript:sendIrNumbers(';
  listprgHTML+='\''+program.chnum.trim()+'\'';
  listprgHTML+=');">';
  listprgHTML+='<span>'+program.prg.trim()+'</span></div>';
 }
 
 document.getElementById("listprg").innerHTML=listprgHTML;
 
 document.getElementById("jsonTextarea").value = JSON.stringify(vocabulary, null, 1);
}

function debug(txt)
{
 if (txt=="")
  document.getElementById("debug").value = "";
 else
  document.getElementById("debug").value = document.getElementById("debug").value+"\n"+txt;
}
function isJSON(str)
{
 try
 {
  JSON.parse(str);
 }
 catch (e)
 {
  return false;
 }
 return true;
}
async function callServer()
{
 var obj;
 var ftrans="";
 var url="/";
 var param="";
 switch (arguments.length)
 {
  case 4:
   ftrans=arguments[3];
  case 3:
   param=arguments[2];
  case 2:
   url=arguments[1];
  case 1:
   id=arguments[0];
   break;
  default:
   return(-1);
 }
 debug("callServer(id="+id+", url="+url+", param="+param+", ftrans="+typeof ftrans+")");
 obj=document.getElementById(id);
 
 try
 {
  const myheaders = new Headers();
  if (param!="")
  {
   if (isJSON(param))
    myheaders.append('Content-Type', 'application/json');
   else
    myheaders.append('Content-Type', 'application/x-www-form-urlencoded');
  }
  const response = await fetch(url,
    {
     method: 'POST',
     headers: myheaders,
     body: param,
     credentials: 'include'
    });
  if (!response.ok)
   throw new Error(`Errore server: ${response.status}`);
  const contentType = response.headers.get("Content-Type");
  if (contentType.includes("application/json"))
  {
   if (ftrans=="")
   { 
    debug("callServer() ftrans assente");
    return;
   }  
   const data = await response.json();
   return ftrans(id, url, data);
  }
  else
  {
   const data = await response.text();
   obj.innerHTML=data;
   setTimeout(vanish, 5000, obj);
   return;
  }
 } 
 catch (err)
 {
  alert("Errore durante la fetch:" + err);
 }
}
function onLoad()
{
 window.addEventListener("resize", checkScreenSize, false);
 window.addEventListener("orientationchange", checkScreenSize, false);
 var HTMLoutput="";
 checkScreenSize();
 
 if (SpeechRecognition)
 {
  recognition.onstart = () =>
   {
    recognition.running = true;
    document.getElementById("micIcon").classList.add("micActive");
   };
  recognition.onend = () =>
   {
    debug("ascolto fermato");
    if (continuous)
    {
     debug("riavvio automatico ascolto");
     setTimeout(() =>
      {
       try
       {
        recognition.start();
       }
       catch (error)
       {
        debug("Errore durante il riavvio automatico:" + error);
       }
      }, 100);
    }
    else
    {
     recognition.running = false;
     document.getElementById("micIcon").classList.remove("micActive");
    }
    // If the user triggered a change that stopped the engine,
    // restart it now with the new configuration.
    if (continuous !== recognition.continuous)
     applySettings();
   };
  recognition.onresult = function(event)
   {
    debug("letto "+ event.results.length + " frasi");
    for (let i = event.resultIndex; i < event.results.length; i++)
    {
     const transcript = event.results[i][0].transcript;
     // Controlla se la frase è definitiva o se il browser sta ancora elaborando (interim)
     if (event.results[i].isFinal)
     {
      debug("Nuova frase:" + transcript.trim());// QUI puoi usare o salvare solo l'ultima parte del testo pronunciato
      processVoiceCommand(transcript);
     }
     else
      debug("Trascrizione temporanea:" + transcript.trim());
    }
   };
  recognition.onerror = function(event)
   {
    debug("Errore:", event.error);
   };
 }
 else
  debug("Il browser non supporta il riconoscimento vocale.");
}
function toggleRec()
{
 if (SpeechRecognition)
 {
  if (recognition.running)
  {
   if (recognition.continuous)
    toggleContinuousMode(document.getElementById("contBtn"));
   else
    recognition.stop();
  }
  else
  {
   // If already stopped, apply immediately and start
   applySettings();
   recognition.start();
   debug("registrazione iniziata con continuous = " + recognition.continuous);
  }
 }
 else
  debug("Il browser non supporta il riconoscimento vocale.");
}
function checkScreenSize()
{
 if (navigator.userAgent.toLowerCase().lastIndexOf("android") == -1 && navigator.userAgent.toLowerCase().lastIndexOf("iphone") == -1)
 { 
  globalWidth = window.innerWidth;
  globalHeight = window.innerHeight;
 }
 else
 { 
  globalWidth = window.outerWidth;
  globalHeight = window.outerHeight;
 }
 debug("globalWidth="+globalWidth+",globalHeight="+globalHeight);
}
function showdebug()
{
// document.getElementById("paneldebug").classList.toggle("visible");
 const paneldebug = document.getElementById("paneldebug");
 const debugBtn = document.getElementById("debugBtn");
 
 if (paneldebug.style.display==='grid')
 { 
  document.getElementById("paneldebug").style.display = 'none';
  debugBtn.classList.remove("debugActive");
 }
 else
 {
  document.getElementById("paneldebug").style.display = 'grid';
  debugBtn.classList.add("debugActive");
 }
 checkScreenSize();
}
function toggleContinuousMode(obj)
{
 if (SpeechRecognition)
 {
  if (continuous)
  {
   obj.children[0].classList.remove("contActive");
   continuous = false;
   debug("Ascolto continuo interrotto");
  }
  else
  {
   obj.children[0].classList.add("contActive");
   continuous = true;
   debug("Ascolto continuo avviato");
  }
  if (recognition.running)
   recognition.stop(); // If running, stop it first. The 'onend' event will handle the restart.
  else
   applySettings(); // If already stopped, apply immediately and start
 }
 else
  debug("Il browser non supporta il riconoscimento vocale.");
}
function applySettings()
{
 recognition.continuous = continuous;
 recognition.lang = 'it-IT';
}
async function loadSaveVocabFunction()
{
 debug("loadSaveVocabFunction()");
 const saveTextareaBtn = document.getElementById('saveTextareaBtn');
 const fileInput = document.getElementById('fileInput');
 const uploadFileBtn = document.getElementById('uploadFileBtn');
 
 // SALVATAGGIO DA TEXTAREA
 saveTextareaBtn.addEventListener(
                                  'click',
                                  async () =>
                                   {
                                    try
                                    {
                                     const parsedJson = JSON.parse(jsonTextarea.value); // Validazione sintassi JSON
                                      
                                     await saveVocabularyToServer(parsedJson);
                                     vocabulary = parsedJson; // Aggiorna il vocabolario in RAM sul browser
                                      
                                     buildVocabContent();
                                      
                                     alert('Vocabolario salvato con successo!');
                                    }
                                    catch (err)
                                    {
                                     alert('Errore: Il testo inserito non è un JSON valido!\n\n' + err.message);
                                    }
                                   }
                                 );
 
 // SALVATAGGIO DA FILE INPUT (.json)
 uploadFileBtn.addEventListener(
                                'click',
                                () =>
                                 {
                                  const file = fileInput.files[0];
                                  if (!file)
                                  {
                                   alert('Seleziona prima un file .json!');
                                   return;
                                  }
                                
                                  const reader = new FileReader();
                                  reader.onload = async (e) =>
                                                   {
                                                    try
                                                    {
                                                     const fileContent = e.target.result;
                                                     const parsedJson = JSON.parse(fileContent); // Validazione JSON
                                                
                                                     await saveVocabularyToServer(parsedJson);
                                                     vocabulary = parsedJson; // Aggiorna il vocabolario in RAM sul browser
                                                
                                                     buildVocabContent();
                                                
                                                     alert('File caricato e salvato con successo!');
                                                    }
                                                    catch (err)
                                                    {
                                                     alert('Errore nel file selezionato: non è un JSON valido.');
                                                    }
                                                   };
                                  reader.readAsText(file);
                                 }
                               );
 
 // SALVATAGGIO VOCABOLARIO -> ESP-12F
 async function saveVocabularyToServer(jsonObject)
  {
   const response = await fetch(
                                '/cmd/savevocab',
                                {
                                 method: 'POST',
                                 headers: { 'Content-Type': 'application/json' },
                                 body: JSON.stringify(jsonObject)
                                }
                               );
   if (!response.ok)
    throw new Error('Errore nella risposta del server ESP');
 }
}

function cmd2server(cmd)
{
 callServer("resp", "/cmd/direct", "cmd="+cmd);
}
