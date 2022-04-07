#include "lang.h"

char *MESSAGE[NB_LANG][NB_MESSAGES] = {
  {				// English Version
   // Zeograd's work
   "ALL FRAMES DRAWN",
   "FRAME SKIP : %d",
   "FULL SPEED",
   "SYNCHRONISATION TO 60HZ",
   "SCREEN SHOT SAVED IN PICT%04X.PCX",
   "GAME STATE SAVED",
   "GAME STATE RESTORED",
   "DUMPING SOUND BEGAN",
   "DUMPING SOUND ENDED",
   "UNKNOWN ROM",
   "F2 - TOGGLE BREAKPOINT",	/* 10 */
   "R  - RAM EDITOR",
   "Z  - VIEW ZERO PAGE",
   "",
   "F5 - SET PC TO CURRENT POS",
   "F6 - NOP CURRENT INSTRUCTION",
   "F7 - TRACE TROUGHT INSTRUCTIONS",
   "F8 - STEP TROUGHT INSTRUCTIONS",
   "",
   "",				/* 20 */
   " � SHUTTING DOWN...",
   " � Initialising Dos/VGA Allegro driver.\n � Allocating buffers...\n",
   " � FAILED\n",
   " � Clearing main buffer...",
   " � OK\n � Initing sound...",
   "\n � Can't set graphic mode!!",
   "Not enough mem!",
   "\n � Entering Autodetection mode...",
   "\n � no audio device found.",
   "\n � %s device found.",	/* 30 */
   "\n � Audio initialization failed.\n",
   "\n � Audio device initialized at %d bits %s %u Hz\n",
   "BYTE AT %d SET TO %d",
   "SEARCH FAILED",
   "FOUND AT %d",
   "STILL NEED TO SEARCH",
   " � Allocating sprite array\n",
   "You played %s.\nThank you very much.\nSee you soon.\n",
   "Unknown ROM\nIf you want to make the science progress\nPlease contact me at ZEOGRAD@CARAMAIL.COM ;)\nI'd be interested in having it.",
   "See Ya!\n",			/* 40 */
   "time elapsed : %.0f sec.\n",
   "frame per sec. : %.2f (very informal)\n",
   "READING DIRECTORY",
   "\n � gamepad detected\n",
   "\n � gamepad non detected\n",
   " � EAGLE mode asked\n",
   " � EAGLE mode not supported\n",
   " � ENGLISH selected\n � Translation by Zeograd\n",
   " � ISO file not found : %s\n",
   "VOLUME SET TO %d",
   "SOUND OUTPUT AT %dHz",
   "VIDEO DUMPING STARTED (%X)",
   "VIDEO DUMPING STOPPED (%X)"},
  {				// French Version
   // Zeograd's work
   "PAS D'IMAGES SAUTEES",	/* 0 */
   "IMAGES SAUTEES : %d",
   "A FOND, A FOND, A FOND",
   "SYNCHRONISATION A 60HZ",
   "IMAGE SAUVEE EN PICT%04X.PCX",
   "PROGRESSION SAUVEE",
   "PROGRESSION RESTOREE",
   "SAUVEGARDE DU SON ENTAMEE",
   "FIN DE LA SAUVEGARDE DU SON",
   "ROM INCONNUE",
   "F2 - BASCULER BREAKPOINT",	/* 10 */
   "R  - EDITEUR DE RAM",
   "W  - VOIR LA ZERO PAGE",
   "",
   "F5 - MET LE PC A L'INSTRUCTION ACTUELLE",
   "F6 - NOPPE L'INSTRUCTION ACTUELLE",
   "F7 - TRACE",
   "F8 - STEP",
   "",
   "",				/* 20 */
   " � JE FERME...",
   " � Initialisation du pilote DOS/VGA Allegro.\n � Allocation des buffers...\n",
   " � ECHOUE\n",
   " � Effacement du buffer principal...",
   " � OK\n � Initialisation du son...",
   "\n � Pas de mode graphique correct !!",
   "Pas assez de m�moire!!",
   "\n � Tentative d'autod�tection...",
   "\n � Pas de carte son trouv�e.",
   "\n � %s trouv�e.",		/* 30 */
   "\n � Initialisation audio �chou�e!\n",
   "\n � Initialisation audio � %d bits %s %u Hz\n",
   "OCTET A %d MIS A %d",
   "RECHERCHE ECHOUEE",
   "TROUVE A %d",
   "PLUSIEURS ENDROITS POSSIBLES",
   " � Allocation du tableau de sprites\n",
   "Vous avez jou� � %s.\nMerci beaucoup.\nA bient�t.\n",
   "ROM inconnue\nSi vous voulez faire avancer la science\nS'il vous plait, prenez contact avec moi � ZEOGRAD@CARAMAIL.COM ;)\nJ'aimerai bien l'avoir.",
   "A+ !\n",			/* 40 */
   "temps �coul� : %.0f sec.\n",
   "images par seconde : %.2f (� titre informel)\n",
   "LECTURE DU REPERTOIRE",
   "\n � joypad d�tect�\n",
   "\n � joypad non d�tect�\n",
   " � mode EAGLE demand�\n",
   " � mode EAGLE non support�\n",
   " � FRANCAIS choisi\n � Traduction par Zeograd\n",
   " � Fichier ISO introuvable : %s\n",
   "VOLUME MIS A %d",
   "SORTIE DU SON A %dHz",
   "DEBUT DUMP VIDEO (%X)",
   "FIN DUMP VIDEO (%X)"},
  {				// spanish version
   // a bit my work
   // Juan Roman Soriano also helped fixing some sentences
   "IMAGENES NO SALTADAS",
   "IMAGENES SALTADAS : %d",
   "VELOCIDAD MAXIMUM",
   "SYNCHRONISACION A 60HZ",
   "IMAGEN SALVADA EN PICT%04X.PCX",
   "ESTADO DEL JUEGO SALVADO",
   "ESTADO DEL JUEGO CARGADO",
   "PRINCIPIO DEL DUMP DEL SONIDO",
   "FINAL DEL DUMP DEL SONIDO",
   "ROM DESCONOCIDA",
   "F2 - CAMBIAR BREAKPOINT",	/* 10 */
   "R  - EDITOR DE RAM",
   "W  - VISUALISADOR DE ZERO PAGE",
   "",
   "F5 - PONE EL PC A LA INSTRUCTION ACTUAL",
   "F6 - NOP LA INSTRUCTION ACTUAL",
   "F7 - TRACE OPCODE",
   "F8 - STEP OPCODE",
   "",
   "",
   " � ME MARCHO ...",
   " � Inicializacion del gestor DOS/VGA Allegro.\n � Ubicacion de los buffers...\n",
   " � FALLADO\n",		/* 20 */
   " � Borrando el buffer principal...",
   " � Correcto\n � Initialisacion del sonido...",
   "\n � No hay modo graphico correcto !!",
   "No hay bastante memoria!!",
   "\n � Intento de autodeteccion...",
   "\n � No hay tarjeta de sonido.",
   "\n � %s encontrada.",
   "\n � Inicializacion del sonido fallada!\n",
   "\n � Inicializacion del sonido a %d bits %s %u Hz\n",
   "BYTE A %d PUESTO A %d",
   "BUSCADA FALLADA",
   "ENCONTRADO A %d",
   "SE DEBE SEGUIR BUSCANDO",
   " � Ubicando de los sprites\n",
   "Juguadas %s.\nMuchas gracias.\nHasta la proxima vez.\n",
   "ROM desconocida\nSi quieres hacer avanzar la sciencia\nEnviame un mail a ZEOGRAD@CARAMAIL.COM ;)\nQueria tenerla.",
   "Hasta la proxima!\n",
   "tiempo passado : %.0f sec.\n",
   "imagen por sec. : %.2f (informal solo)\n",
   "LECTURA DEL REPERTORIO",
   "\n � Encontrado joypad\n",
   "\n � No hay joypad encontrado\n",
   " � modo EAGLE pedido\n",
   " � modo EAGLE no es soportado\n",
   " � ESPANOL elegido\n � Traduction de Ochoa Sandrine y Juan Roman Soriano\n",
   " � ISO no puede ser encontrado\n",
   "VOLUMA PUESTO A %d",
   "SONIDO SALE A %dHz",
   "VIDEO DUMPING STARTED (%X)",
   "VIDEO DUMPING STOPPED (%X)"
   },
  {				// Slovenian Version
   // ]pit[ 's work
   "VSI OKVIRJI SO NARISANI",
   "OKVIR SPUSCEN: %d",
   "POLNA HITROST",
   "SINHRONIZACIJA NA 60HZ",
   "SCREEN SHOT SHRANJEN V PICT%04X.PCX",
   "POLOZAJ IGRE SHRANJEN",
   "POLOZAJ IGRE PRIKLICAN",
   "ZAJEMANJE ZVOKA SE JE ZACELO",
   "ZAJEMANJE ZVOKA JE KONCANO",
   "NEZNAN ROM",
   "F2 - SPREMENI BREAKPOINT",
   "R  - UREJEVALNIK RAMA ",
   "Z  - VIEW ZERO PAGE",
   "",
   "F5 - SET PC TO CURRENT INSTRUCTION",
   "F6 - NOPPE CURRENT INSTRUCTION",
   "F7 - TRACE",
   "F8 - STEP",
   "",
   "",
   " � ZAUSTAVITEV...",
   " � Inicializiranja Dos/VGA Allegro goninika.\n � Zaznavanje vmesnega pomilnika...\n",
   " � NI USPELO\n",
   " � Ciscenje vmesenga pomilnika...",
   " � OK\n � zaznavanje zvoka...",
   "\n � Nemore nastavit graficnega nacina!!",
   "Ni dovol spomina!",
   "\n � Vstopanje v ssamo zaznamovalen nacin...",
   "\n � nobena avdio naprava ni najdena.",
   "\n � %s naprava najdena.",
   "\n � Avdio initializaja je bila neuspesna.\n",
   "\n � Audio naprava initializirana na %d bits %s %u Hz\n",
   "BYTE NA %d NASTAVLJENO NA %d",
   "ISKANJE NEUSPESNO",
   "NAJDENO NA %d",
   "SE VEDNO MORA ISKATI",
   " � Zaznavanje 'sprite' zaporedja\n",
   "Igral si %s.\nHvala lepa.\nSe Se vidimo kasneje.\n",
   "Neznan ROM\nCe hoces naredit napredek v vedi\nme kontaktirajte na ZEOGRAD@CARAMAIL.COM ;)\nSem zainterisiran za vse prispevke.",
   "Adijo!\n",
   "porabljen cas : %.0f sek.\n",
   "slik na sekundo. : %.2f (zelo nepravilno)\n",
   "BRANJE DIREKTORIJA",
   "\n � gamepad najden\n",
   "\n � gamepad ni najden\n",
   " � EAGLE nacin izbran\n",
   " � EAGLE nacin ni podpiran\n",
   " � SLOVENSKO izbrano\n � Prevedel ]pit[\n",
   "VOLUME SET TO %d",
   "SOUND OUTPUT AT %dHz",
   "VIDEO DUMPING STARTED (%X)",
   "VIDEO DUMPING STOPPED (%X)"
   },
  {				// Portuguese Version
   // CodeMaster's work
   "TODOS OS QUADROS",
   "AVAN�O DE QUADRO : %d",
   "VELOCIDADE TOTAL",
   "SINCRONIZACAO PARA 60HZ",
   "IMAGEM DA TELA SALVA EM PICT%04X.PCX",
   "ESTADO DO JOGO SALVO",
   "ESTADO DO JOGO RESTAURADO",
   "DUMP DE SOM INICIADO",
   "DUMP DE SOM TERMINADO",
   "ROM DESCONHECIDA",
   "F2 - MUDAR O BREAKPOINT",	/* 10 */
   "R  - EDITOR DA RAM",
   "Z  - VIEW ZERO PAGE",
   "",
   "F5 - PONE EL PC AL INSTRUCTIONNE CURRENTA",
   "F6 - NOPPE LA INSTRUCTIONNE CURRENTA",
   "F7 - TRACE",
   "F8 - STEPPING",
   "",
   "",				/* 20 */
   " � FECHANDO...",
   " � Inicializando Dos/VGA Alegro driver.\n � Alocando buffers...\n",
   " � FALHOU\n",
   " � Limpando Buffer principal...",
   " � OK\n � Iniciando som...",
   "\n � Impossivel selecionar o modo gr�fico!!",
   "Mem�ria insuficiente!",
   "\n � Entrando no modo de autodete��o...",
   "\n � dispositivo n�o encontrado.",
   "\n � %s dispositivo encontrado.",	/* 30 */
   "\n � A inicializa��o de audio falhou.\n",
   "\n � Dispositivo de audio inicializado em %d bits %s %u Hz\n",
   "BYTE EM %d FIXO PARA %d",
   "A PROCURA FALHOU",
   "ENCONTRADO EM %d",
   "AINDA PRECISA PROCURAR",
   " � Alocando a ordem do sprite\n",
   "Voc� jogou %s.\nMuito Obrigado.\nVejo voc� logo.\n",
   "ROM Desconhecida\nSe voc� deseja informa��es sobre o progresso\nPor favor me contate em ZEOGRAD@CARAMAIL.COM ;)\nE ser interessado em ter isto.",
   "Falo!\n",			/* 40 */
   "\ntempo decorrido : %.0f sec.\n",
   "quadro por seg. : %.2f (muito informal)\n",
   "LENDO DIRETORIO",
   "\n � Gamepad detectado\n",
   "\n � Nenhum Gamepad detectado\n",
   " � modo EAGLE pedido\n",
   " � modo EAGLE n�o suportado\n",
   " � PORTUGUES selecionado\n � Tradu��o por Code_Master\n",
   " � Arquivo ISO n�o encontrado : %s\n",
   "FIXAR VOLUME A %d",
   "SAIDA DE SOM A %dHz",
   "VIDEO DUMPING STARTED (%X)",
   "VIDEO DUMPING STOPPED (%X)"
   },
  {				// German Version
   // sALTY's work
   "ALLE FRAMES AUSGEFUHRT ",	/* 0 */
   "FRAME(S) AUSSETZEN : %d",
   "VOLLE GESCHWINDIGKEIT",
   "SYNCHRONISATION ZU 60HZ",
   "BILD GESPEICHERT UNTER PICT%04X.PCX",
   "GAME STATE GESPEICHERT",
   "GAME STATE GELADEN",
   "DUMPING SOUND BEGAN",
   "DUMPING SOUND ENDET",
   "UNBEKANTES ROM",
   "F2 - SETZE BREAKPOINT",	/* 10 */
   "R  - RAM EDITOR",
   "Z  - VIEW ZERO PAGE",
   "",
   "F5 - SET PC TO CUURENT INSTRUKTION",
   "F6 - NOPPE INSTRUKTION",
   "F7 - TRACE DURCH INSTRUKTION",
   "F8 - STEP DURCH INSTRUKTION",
   "",
   "",				/* 20 */
   " � SHUTTING DOWN...",
   " � Initialisiere Dos/VGA Allegro driver.\n � Lokalisiere Buffer...\n",
   " � FEHLER\n",
   " � Loesche Hauptspeicher...",
   "\n � OK\n � Initialisiere sound...",
   "\n � Kann Grafik modus nich ausfuehren!!",
   "Nicht genug Speicher!",
   "\n � Starte Autodetection modus...",
   "\n � Kann audiodevice nicht finden.",
   "\n � %s device gefunden.",	/* 30 */
   "\n � Audio initialisierung gescheitert.\n",
   "\n � Audio device initialisiert bei %d bits %s %u Hz\n",
   "BYTE BEI %d ZU %d GESCHRIEBEN",
   "SUCHE GESCHEITERT",
   "GEFUNDEN BEI %d",
   "ES WIRD NOCH GESUCHT",
   " � Lokalisiere Sprite array",
   "Du hast %s gespielt.\nVielen Dank.\nSee you soon.\n",
   "Unbekanntes ROM\nWenn du einen Vorschlag hast \nSchick mir ne Mail ZEOGRAD@CARAMAIL.COM ;)\nIch waere sehr interessiert ihn zu wissen ;).",
   "Ciao Ciao!\n",		/* 40 */
   "Vergangende Zeit : %.0f sec.\n",
   "Bilder pro sek. : %.2f (very informal)\n",
   "LESE DIRECTORY",
   " � Gamepad gefunden\n",
   " � Gamepad nicht gefunden\n",
   " � EAGLE mode gefragt\n",
   " � EAGLE mode wird nicht unterstuetzt\n",
   " � DEUTSCH ausgewaehlt\n",
   " � ISO file nicht gefunden : %s\n",
   "LAUTSTAERKE NACH %d GESETZT",
   "SOUND OUTPUT BETRAEGT %dHz",
   "BILD AUFNAHME GESTARTET (%X)",
   "BILD AUFNAHME GESTOPPED (%X)"
   },
  {				// Nederlandse Versie
   // Dutch version
   // Fixo's work
   "ALLE BEELDEN OPGETEKEND",
   "BEELD SKIP : %d",
   "VOLLEDIGE SNELHEID",
   "SYNCHRONISATIE NAAR 60HZ",
   "BEELDSCHERM OPGESLAGEN IN PICT%04X.PCX",
   "SPEL STATUS OPGESLAGEN",
   "SPEL STATUS OPGELADEN",
   "GELUIDSDUMP BEGONNEN",
   "GELUIDSDUMP BEEINDIGD",
   "ONBEKENDE ROM",
   "F2 - WISSELING BREAKPOINT",	/* 10 */
   "R  - RAM EDITOR",
   "Z  - ZERO PAGE BEZICHTER",
   "",
   "F5 - SET PC TO CURRENT OPCODE",
   "F6 - NOP OPCODE",
   "F7 - TRACEER EERSTE OPCODE",
   "F8 - DOORZOEK HUIDIGE OPCODE",
   "",
   "",				/* 20 */
   " � BEZIG MET AFSLUITEN...",
   " � Initialiseren van Dos/VGA Allegro driver.\n � Toewijzen van buffers...\n",
   " � GEFAALD\n",
   " � Wissen hoofd buffer...",
   " � OK\n � Initialieren van geluid...",
   "\n � Kan deze graphics mode niet gebruiken!!",
   "Niet genoeg geheugen!",
   "\n � Begin geluidsautodetectie modus...",
   "\n � Geen geluidsapparaat gevonden",
   "\n � %s apparaat gevonden.",	/* 30 */
   "\n � Geluidsinitialitie mislukt.\n",
   "\n � Geluidsapparaat ingesteld op %d bits %s %u Hz\n",
   "BYTE AT %d SET TO %d",
   "FOUT BIJ HET ZOEKEN",
   "GEVONDEN OP %d",
   "ZOEKEN BLIJFT DOORGAAN",
   " � Allocating sprite array\n",
   "Je speelde %s.\nDank u vriendelijk.\nTot later.\n",
   "Onbekende ROM\nAls je de wetenschap wil helpen\nstuur een mailtje naar ZEOGRAD@CARAMAIL.COM ;)\nI'd be interested in having it.",
   "Groeten!\n",		/* 40 */
   "gebruikte tijd : %.0f sec.\n",
   "beelden per sec. : %.2f (erg informatief)\n",
   "Doorzoeken van de directory",
   "\n � gamepad gedetecteerd\n",
   "\n � gamepad niet gedetecteerd\n" " � EAGLE modus geactiveerd\n",
   " � EAGLE modus niet beschikbaar\n",
   " � Nederlandse Versie\n � Vertaald door fixo\n",
   " � ISO bestand niet gevonden : %s\n",
   "VOLUME INGESTELD OP %d",
   "GELUIDSWEERGAVE OP %dHz",
   "VIDEO DUMPING STARTED (%X)",
   "VIDEO DUMPING STOPPED (%X)"
   },
  {				// Polish Version
   // Dox's and Faust's work
   "WSZYSTKIE KLATKI WYSWIETLANE",
   "ILOSC OPUSZCZANYCH KLATEK : %d",
   "PELNA SZYBKOSC",
   "SYNCHRONIZACJA 60HZ",
   "ZRZUT EKRANU ZAPISANY - PICT%04X.PCX",
   "STAN GRY ZAPISANY",
   "STAN GRY ODTWORZONY",
   "ROZPOCZETO ZAPISYWANIE DZWIEKU",
   "ZAKONCZONO ZAPISYWANIE DZWIEKU",
   "NIEZNANY ROM",
   "F2 - PULAPKA",		/* 10 */
   "R  - EDYTOR RAM",
   "Z  - ZERO PAGE VIEWER",
   "",
   "F5 - SET PC TO CURRENT INSTRUCTION",
   "F6 - NOP OPCODE",
   "F7 - SLEDZENIE PROGRAMU",
   "F8 - POJEDYNCZA INSTRUKCJA",
   "",
   "",				/* 20 */
   " � ZAMYKAM...",
   " � Inicjalizacja sterownika Dos/VGA Allegro .\n � Rezerwacja buforow...\n",
   " � BLAD\n",
   " � Czyszczenie glownego bufora...",
   " � OK\n � Inicjalizacja dzwieku...",
   "\n � Nie mozna ustawic trybu graficznego!!",
   "Za malo pamieci!",
   "\n � Tryb Autodetekcji...",
   "\n � nie znaleziono urzadzenia audio.",
   "\n � znaleziono %s .",	/* 30 */
   "\n � Blad urzadzenia audio.\n",
   "\n � Parametry urzadzenia aduio : %d bitow %s %u Hz\n",
   "BAJT W %d USTAWIONO NA %d",
   "NIE ZNALEZIONO",
   "ZNALEZIONO W %d",
   "KONIECZNE DALSZE POSZUKIWANIE",
   " � Rezerwacja tablicy obiektow\n",
   "Grales w %s.\nDziekuje ...\nDo zobaczenia.\n",
   "Nieznany ROM\n Jesli chcesz wspomoc emulacje\nSkontaktuj sie ze mna: ZEOGRAD@CARAMAIL.COM ;)\nChetnie sie tym zajme.",
   "Czesc!\n",			/* 40 */
   "czas emulacji : %.0f sek.\n",
   "klatek na sek. : %.2f \n",
   "ODCZYT KATALOGU",
   "\n � gamepad zostal wykryty\n",
   "\n � gamepad nie zostal wykryty\n",
   " � wybrano tryb EAGLE\n",
   " � tryb EAGLE nie obslugiwany\n",
   " � wybrano POLSKI\n � Tlumaczenie Dox i Faust\n",
   " � nie znaleziono pliku ISO : %s\n",
   "VOLUME SET TO %d",
   "SOUND OUTPUT AT %dHz",
   "VIDEO DUMPING STARTED (%X)",
   "VIDEO DUMPING STOPPED (%X)"
   },
  {				// Italian Version
   // Gareth Jax's work
   "DISEGNA OGNI FRAME",
   "SALTA FRAME: %d",
   "ALLA GRANDE!",
   "SINCRONIA A 60HZ",
   "SCHERMATA SALVATA IN PICT%04X.PCX",
   "GIOCO SALVATO",
   "GIOCO RICARICATO",
   "INIZIO SALV. DEL SUONO",
   "FINE SALV. DEL SUONO",
   "ROM IGNOTA",
   "F2 - METTI/TOGLI BREAKP.",	/* 10 */
   "R  - RAM EDITOR",
   "Z  - VEDI PAGINA ZERO",
   "",
   "F5 - SET PC TO CURRENT OPCODE",
   "F6 - NOP OPCODE",
   "F7 - TRACCIA ESECUZIONE",
   "F8 - ESECUZIONE PASSO-PASSO",
   "",
   "",				/* 20 */
   " � SPEGNIMENTO...",
   " � Preparazione driver Dos/VGA Allegro.\n � Allocazione buffers...\n",
   " � ERRORE\n",
   " � Pulizia del Buffer...",
   " � OK\n � avvio audio...",
   "\n � Non c'e' la risol. video!!",
   "Non basta la memoria!",
   "\n � Rilevamento automatico...",
   "\n � Nessuna scheda audio.",
   "\n � %s rilevato.",		/* 30 */
   "\n � l'audio non funziona!!\n",
   "\n � La scheda sonora funziona a %d bits %s %u Hz\n",
   "BYTE A %d MESSO A %d",
   "RICERCA FALLITA",
   "TROVATO A %d",
   "CONTINUA A CERCARE!",
   " � Preparo sprite array \n",
   "hai giocato a %s.\nGrazie mille!.\nCi vediamo!.\n",
   "ROM Ignota\nSe vuoi aiutare la scienza\nScrivimi a  ZEOGRAD@CARAMAIL.COM ;)\nMi interesserebbe averla.",
   "Ciao!\n",			/* 40 */
   "Tempo trascorso : %.0f sec.\n",
   "frame per sec. : %.2f (approssim.)\n",
   "LETTURA DIRECTORY",
   "\n � gamepad trovato\n",
   "\n � gamepad non trovato\n",
   " � EAGLE mode attivo\n",
   " � EAGLE mode not usabile\n",
   " � ITALIAN selected\n � Traduzione by GarethJax\n",
   " � ISO file not trovato: %s\n",
   "VOLUME MESSO A %d",
   "QUALITA' AUDIO: %dHz",
   "VIDEO DUMPING STARTED (%X)",
   "VIDEO DUMPING STOPPED (%X)"
   }

};
// predefined messages to display
