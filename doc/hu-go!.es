            .  ___   ____                _________           ___
             .|  `|_|   `| ___ ___  __ .|    ____/ _______ .|  `|
           .:|     _     ||  `|  `|(__)|    (_  `|/   _  `\|____/:.
      ...:::||____|:|____||_______/::::\_________/\_______/(____)::::...
            `-*-- -  -   -    -      -            -              ^cls


                Core-GraphX / PC Engine / Turbo GraphX Emulador
                                Versión 1.02
                           17 de Septiembre 1999

        << Algunas informaciones pueden no ser en esta documentacion >>

Nota: La marca "(*)" indica los cambios principales

Traducido por Juan Roman Soriano
( Se puede que hay pequenos problemas perque yo (Zeograd) hizo algunos
  cambiados para la nueva version )

- [INFORMACION PARA CONTACTAR ]-----------------------------------------------------------------

 correo electrónico:
		zeograd@caramail.com <---- PREFERIDO
		ojolly@hotmail.com 
		ojolly@caramail.com
      hugo@fr.fm
      zeograd@icqmail.com

 ICQ UIN: 40632933

Página Web:
		http://www.emuunlim.com/hugo (francés e inglés)
             www.hugo.fr.fm
		Esta página puede cambiar, pero siempre se puede encontrar la página correcta en
		http://come.to/zeograd.

		(también hay una agradable entrevista mía en 
		 http://www.emuunlim.com/emailwars/email5)

- [RESPONSABILIDAD]--------------------------------------. 

	No se me puede hacer responsable de ning'un mal uso de este software,
	no tiene ninguna garantía en absoluto. Úselo bajo su responsabilidad.

- [INTRODUCCION ]--------------------------------------.

	Hola todos, bienvenidos a la documentación de Hu-Go!.
	Yo (Zeograd) no soy realmente el autor de este programa. Descubrí las fuentes
	de un emulator de PC Engine en la página Web de Bero 
	(http://www.geocities.co.jp/SiliconValley/7052/)

	Mas las mejoras en la página Web de Hmmx
	(formerly, http://www.geocities.co.jp/Playtown/2004/)
	
	Hmmx ha hecho XPCE(para DirectX) pero yo estaba interesado en continuar su trabajo
	bajo DOS y "voila", usted tiene otro emulador disponible para quemar su teclado jugando 
	toda la noche con los los magnificos juegos de PCE  ;)

	Este emulador es libre y usted tiene el acceso a fuentes tal y como como Bero hizo
	con su trabajo. Me gustar'ia que la gente que estuviera interesada en hacer un buen
	emulador abierto, contactara conmigo para decrime que le falta a HU-GO!.
	No vacile contestar la encuesta de mi página Web ni acudir al foro que usted
	puede encontrar allí. Si usan los fuentes, por favor avisenme. Yo le aseguro que no me 
	rasgaré las vestiduras.


- [ ¿COMO INSTALARLO? ]---------------------------------------. 

	Muy fácil. ¡Si usted obtuvo Hu-Go! mediante un fichero ZIP! (lo mas provable),
	solo debe desempaquetarlo en un directorio de un disco escribible
	(Ejemplo:disco duro o disco flexible pero no un CDROM. ..) 

	También puede asociar la entension "PCE" para las roms de "PC Engine" bajo Win9X.
	Agregue la extensión, asociela con Hu-Go! y use el bonito icono que hizo Raoh (PCE.ICO)
	¿A que está muy bien? ¿ o  no ? 

- [ ¿COMO USARLO? ]---------------------------------------

	Quizas lo más facil sea hacer un enlace directo entre su cerebro y el ordenador para
	decirle el nombre de la rom.
	
	Sólo teclee Hu-Go! [ROM] [BACKUPMEM] [OPTION] Donde ROM es el
   nombre de la ROM o la ISO o la ISQ que quiere ejecutar. Si ROM tiene no
   extensión, Hu-Go! buscará ROM.PCE después ROM.ZIP.
	Si el ROM tiene la extensión de PCE, la intentará ejecutar, despues probrará ROM
	en ROM.ZIP.
	Entonces si ROM tiene la extensión de ZIP, busca el archivo de PCE con mismo nombre
	que ROM...
	(Juegos recomendados - SUPER STAR SOLDAIER o NEUTOPIA 2 ¡no lo lamentará!)
   Un ISO debe tener la extensión ISO para ser reconocida. En este caso, se
   debe poner la variable cdsystem_path correctamente. Debe indicar la
   direction del cd system card. Es la misma cosa con una ISQ.

	Si usted no especifica ninguna rom, usted verá un buen selector de ficheros =),
	donde puede rápidamente seleccionar una rom tecleando sus primeras letras, si teclea
	pulsando Shift, cambiará de disco.

	Las OPCIONES, las veremos mas tarde... 

	BACKUPMEM es el nombre del archivo que contiene la copia de la RAM.
	Estos dos ultimos son opcionales, el orden no debería importar, pero el
   nombre de la ROM debe estar antes del nombre de la copia de la RAM.
	Pronto añadiré un interfaz de usuario, sea paciente...

- [LOS REQUISITOS DEL SISTEMA]--------------------------------------. 

	-- Pentium 166 (quizá funcione sin el sonido; use un 233 o 266 para tener buen sonido).
	   Para graficos EAGLE, necesitará un 266 o 300 Mhz para plena velocidad.

	-- 8MB de RAM. 

	-- Un sistema compatible DOS (Informeme si le funciona con otras versiones de DOS que
	   no sean el MS-DOS de Microsoft). 

	-- Una tarjeta de sonido (sólo si quiere oír el sonido, por supuesto). 

	-- Una pequeña cantidad de neuronas (suficientes para controlar las funciones basicas
	   motoras. Para poner trucos y desensamblar son necesarios unos cuantos trillones 
	   mas de neuronas;)

	-- Dedos (puede ser reemplazado por un enlace directo, estoy
	   trabajando en ello, si quiere hacer de cobaya, digamelo;)
	   Nota: requiere la cantidad de neuronas indicadas arriba.
 
- [EL TECLADO ]--------------------------------------. 

	Teclas de cursor		Dirección
	Alt				Boton 1
	Espacio			Boton 2
	Tabulador		Seleccionar
	Entrada			Empezar
	X				   Cambia autodisparo 1
	C				   Cambia autodisparo 2

	(Estas son las teclas por defecto para todos los jugadores)
	Pueden modificarse editando el fichero HU-GO!.INI (los detalles vienen detras)

	Para jugar al "pinball", las teclas son las siguientes:

	Shift izquierdo		Izquierda
	Z				         Derecha
	Tecla de parar		   Boton 1
	Shift derecho		   Boton 2

	Esto es suficiente para los petacos y empujar la máquina.
	Si encuentra algún juego de petacos que no sea detectado (por ahora solo están
	Time Cruise y Devil Crash), envieme la información.

	Teclas especiales:

	No pueden ser cambiadas ( se pueden asignar a otras normales, estas tienen siempre
	los efectos especiales, eventualmente seguidos por el efecto "normal")

	1 en teclado normal		Cambia los "sprites" (mapas de dibujos)
	2 en teclado normal		Cambia el fondo de la pantalla

-  teclas alpha -----------
   3,4,5,6,7 o 8 teclado normal
                           Puede deblocar algunas ISOs
-  teclas alpha -----------
    ) en teclado normal    Abajo el nivel sonico
    = en teclado normal    Aumenta el nivel sonico
    9 en teclado normal    Abajo la qualidad sonica
    0 en teclado normal    Aumenta la qualidad sonica
	CONTROL+ALT+END			Forma demasiado brusca de salir, si no hay mas remedio...
	ESC					Llama al selector de ficheros para cambiar el juego
	F1					   Pide el valor a buscar (ver mas adelante)
	F2					   Pone un valor en RAM (ver mas adelante)
	F3					   Quita un valor en RAM
	F5					   Graba la pantalla como pict????.pcx
	F10					Cambia el modo de pantalla completa
	F12					Salida (no es demasiado util)
	F6					   Graba la partida como ROM.SAV, donde ROM es
						   el nombre de la ROM o ISO.SVI para ISO ficheros y
                     un unico fichero para los CDs, qual que siera
	F7					   Carga la partida
	PAUSA					Pausa !Esto es creatividad!
   Bloquea Numero			Cambia el modo de sonido (envie o no el sonido en
                        SND????.WAV)

	Teclas usadas del teclado numérico
	+					Se salta mas fotogramas (frames) por segundo
	-					Se salta menos fotogramas
	*					Modo de desensablado (si no lo entiende es que no está
						hecho para usted. Este modo es para desarrolladores y
						"hackers". Ver el rincón del desarrollador...)




- [OPCIONES ]--------------------------------------. 

	Hay dos clases de opciones: de línea de comandos o por el fichero HU-GO!.INI.
	El emulador lee primero el fichero INI y después las opciones de línea de comandos,
	estas pueden reemplazar las generales del INI.

	*** opciones de línea de comandos ***
	Pueden ser -FV o -F V donde F es la bandera (flag) y V el valor

   -- v 	: Video preferido
		0 -> intenta 256x224, despues 256x240 despues 256x256 despues 320x240 y despues 320x200
		1 -> intenta 256x240 despues 256x256 despues 320x240 y despues 320x200
		2 -> intenta 256x256 despues 320x240 y despues 320x200 (por el defecto)
		3 -> intenta 320x240 y despues 320x200
		4 -> solo 320x200
      5 -> solo 800x600

	-- s	: Sonido
		0 -> Sin sonido
     -1 -> Autodetectado (por defecto)

	-- d	: Punto de ruptura
		0 -> Sin punto de ruptura (por defecto)
		1 -> Se pone en el vector de reset

   -- c	: Emulación de CD (no es necesitado para la emulación ISO pero functiona tambien,
                     Se debe utilizar para la emulación de los CDs)
		0 -> Solo emulación de la tarjeta Hu (por defecto)
      1 -> Emulación de CD (un poquito ahora!, en DOS solamente)
		2 -> Emulación de fichero ISO ( esto... tampoco funciona)
      3 -> Emulación de fichero ISQ
      4 -> Emulación de fichero BIN ( raw total dump de un CD )

   -- i  : Nombre del ISO (o ISQ o BIN) a ejecutar o CD lectore.
      (no es necesitado para la emulation ISO pero functiona tambien)
      por ejemplo : Hu-Go! -c2 -iDraculaX.iso
      hace functionar Hu-Go! con el ISO DraculaX.iso.
      cuando '-c1' es especificado, puede ser utilisado para indicar el lectore
      a utilisar como '-if' si el CD es en F:

	-- e	: modo EAGLE
		0 -> Modo normal, sin interpolación (por defecto)
		1 -> Modo EAGLE, gráficos abanzados con interpolación

(*)-- S : modo scanline
      0 -> normal (por defecto)
      1 -> con el modo scanline

   -- u : Rom codado en US
      0 -> normal, sin inversion de los bits (por defecto)
      1 -> rom codado en US, los bits son invertidos
   Hu-Go! deberia reconecer normalmente esos roms, si no es el caso
   (no functiona normalmenete pero con -u1, no hay problemas) dimelo
   que lo pone en la base de Hu-Go!.

	** Opciones del fichero de inicialización **
	Este fichero admite lineas de comentario que empiezen con #.
	El formato es F=V donde F es la opción (flag) y V el valor.

	---TECLAS (KEYS):
		Es un poco largo explicarlo detalladamente, pero veremos el método.
		Cada función se representa por un nombre ("UP","DOWN","LEFT",
		"RIGHT","I","II","SELECT","START","AUTOI","AUTOII") inmediatamente seguido por
		el numero del controlador. El valor a poner el de rastreo del teclado, que se 
		puede leer en SCANCODE.TXT.
		Ahora, se pueden crear hasta 15 configuraciones diferentes usando secciones como
		[config5]. Estas secciones deben estar al final del fichero INI y contienen
		definiciones de teclado que se explican a continuación, pero tambien contienen
		información de los controladores.

		ejemplo:	poniendo input2=key3 se consigue que el segundo controlador de la 
				PC Engine sea controlado por el tercer mapeado de teclado (UP3, 				LEFT3,...) etc...

		Como palabras válidas tenemos desde "input1" hasta "input5" para cada uno de los 
		cinco mandos de la PC Engine (sólo se emulan cuatro), y desde "key1" hasta "key5"
		para cada uno de los cinco mapeados de teclado. Desde "joy1" hasta "joy4" para
		cuatro posibles mandos del ordenador y también "mouse1" y "mouse2" para dos ratones
		(aún sin implementar), tampoco está implementado "synaplink" para enlaces.


	EJEMPLO DE FICHERO INI
	======================

     config=1

     [config1]

       input1=key1

       UP1=0x16
       LEFT1=0x23
       DOWN1=0x24
       RIGHT1=0x25

       input2=joy1
			   

	Esto cambiara las teclas de dirección para el primer jugador a "u","h","j","k",
	mientras que el segundo jugador jugará con el primer mando del ordenador, advierto que
	cuando se especifica una entrada desde un mando de juegos, sólo las direcciones y dos
	botones se mapean, aún se pueden seleccionar las teclas para las funciones de selección,
	inicio, auto-1 y auto-2.
	Como ejemplo el config1 es para un jugador usando una palanca, por eso si quiere usar una
	palanca de juegos, sólo debe descomentar la linea que tiene "config=1".
	No está muy claro pero hacer un menú ayudaría a configurar.

	-- Lenguaje: el idioma de la mayoría de los mensajes durante la emulación
		0 ->	Inglés (rebeldía) (mío)
		1 ->	Francés (mío)
		2 ->	Español (por mi novia y Juan Ramon Soriano)
		3 ->	Sloveno (por mi ]pit[)
		4 ->	Portugés (por CodeMaster)
		5 ->	Alemán (por SALTY)
		6 ->	Holandés (por Fixo)
		7 ->	Polaco (por Dox y Faust)
      8 ->  Italiano (por Gareth Jax)

	-- rom_dir: Directorio para las roms. Es el directorio que el selector de ficheros
			abrirá por defecto.
			Use "/" o bien "\". (por defecto es c: \jeu\pce [mio!])
			Si este directorio no existe (lo más probable), entonces se usa el actual.

	-- exact_draw:
			¿ Vamos a usar una manera más exacta pero más lenta para dibujar
			los sprites?
			0 -> la manera más sucia, pero más rápida (por defecto)
			1 -> la manera tradicional y lenta dibujar sprites

	-- smode: modo de sonido, como antes...
	
	-- vmode: modo de video, como antes...

	-- static_refresh:
		Dentro del directorio se muestran todos los ficheros de rom para seleccionar el
		va a usar con Hu-Go! provinientes de un rastreo actual o anterior.
		Rastrear todos los ficheros puede ser lento (sobre todo si tiene muchos ficheros
		nuevos de roms en formato zip), pero usando un rastreo anterior, el listado sigue
		inmaculado (los ficheros pueden ser borrados o añadidos otros nuevos y el listado
		de Hu-Go! sigue siendo el mismo).
		0 ->	Rastrea cada vez
		1 -> 	Rastrea solo una vez (F1 para forzar refresco de listado, ¡no lo olvide!)

	 -- eagle: modo EAGLE, como antes...

    -- snd_freq : Frequencia del sonido PCM. Mas es alto y mejor es la
    qualidad. Utilisa 11025 on pequenas maquinas y 22050 o 44100 sobre
    mas potentas maquinas. Se puede tambien donar frequencias entre aquellas.
       22050 Hz es defecto

    -- buffer_size : Tamano del 'buffer' utilisado para el sonido PCM.
    Se debe intentar differentes tamanos para encontrar el mejor para
    vuestra maquina. Utiliso 1000 (defecto) para mi K6-2 350 y 1500 sobre mi P200.

    -- joy_type : numero del piloto a utilizar para el gamepad.
    se debe buscar el numero de vuestro piloto en la lista dentro del fichero
    .INI. Si el numero no es bueno o si se pone un numero negativo, una
    autodetecion es hecha (como antes)

- [SONIDO]--------------------------------------. 

   Ahora, he cambiabo el sonido. E soy bastante contento del resultado.
   Pero cambiando la manera de hacer sonido, se puede que las tajetas sonidas
   no sean reconocidas de la misma manera.
	Si no especificado en la línea de comandos -s0 o bien -s 0, entonces Hu-Go! intentará
	autodetectar su tarjeta de sonido y seleccionará la primera que identifique (en caso
	de tener más de una).
	Quizás una variable de entorno correctamente asignada ayude en la detección (ya sabe,
	esas estrañas ordenes en el AUTOEXEC.BAT como:
	SET MI_TARJETA = I1 D0 NT UN DER ST@ND, HE1P)

	En lo concerniente a la calidad, el sonido sale sólo en 8 bits mono.
   Ahora, entre dos pulsaciones de "Num lock" el sonido sería grabado en
   .WAV el disco y podría oirse con un buen reproductor multimedia.

   Por supuesto, en el futuro, todo esto será configurable.

- [COMO ACELERARLE]--------------------------------------. 

	El poner activas nuevas opciones del emulador, pueden ralentizarle, he decidido
	obviar algunas para hacerle más jugable.

	-- 	Intente la opción -v4 en la línea de comandos. En 320x200 el emulador va sobre
		un 10% más rápido, pero... (siempre hay un pero) la pantalla se cortará
		(ejemplo: no podrá ver las primeras y últimas 12 líneas)

	--	Intente la opción "exact_draw=0" en el fichero .INI.
	
	--	Si normalmente juega bajo Win9x. Intentelo bajo DOS, ganará un 15%
		de velocidad.

	-- 	Desactive el sonido con la opción de línea de comandos -s0 o poniendo
		sound=0 en el fichero .INI.

	-- 	Desactive el modo EAGLE con la opción de línea de comandos -e0 o
		poniendo eagle=0 en el fichero .INI.

	--	No olvide que se pueden saltar fotogramas (frames) usando "+" o "-",
		con esto se pierde continuidad, pero se ganas hasta 16 veces en velocidad.

	--	Si su disco duro se mantiene accediendo a datos mientras juega, significa
		que no tiene suficiente RAM y está usando memoria virtual en el disco (que
		es mucho más lenta). Usted debiera probar a hacer un disco de arranque limpio
		o sea, sin drivers, etc...
		Si esto no es suficiente, entonces tendrá que comprar más RAM (No creo que 
		tenga que hacerlo nunca, Hu-Go! es muy económico ;)

	--	Comprese un ordenador más rápido ;)


	Para hacerse una idea, bajo DOS, en 320x200 sin sonido, se pueden obtener
	125 fotogramas por segundo en un K6 de 350 Mhz (y 55 en un P200).
	En mi K6-350 obtengo un limite de 60 fotogramas/segundo siempre.
	En un P200 con el sonido activado, sólo necesito rechazar un fotograma para
	obtener la máxima velocidad y buen sonido bajo Win95 (en cualquier modo de video).
	[ y sobre 7 fotogramas/segundo en un 486DX33 con 4Mb en modo 256x240 sin sonido ]
		


- [LAS MEJORAS ]--------------------------------------. 

	Aquí encontrará todo tipos de ideas para mejorar Hu-Go! Puede
	esperar hasta que siguentes sabrosas opciones se apliquen o hechar una mano,
	de usted depende.
	He agregado un indicador de dificultad (E: Fácil, M: Medio, H: Duro), para al gente
	que me quiera ayudar, así que usted puede seleccionar un objetivo en su nivel,
	para la mayoría de los usuarios, primero deberían hacer las tareas fáciles.


EMH
X  --	Localización de mensajes (necesito a voluntarios para otros idiomas, aunque ahora,
	 varios estén aplicados)
X  --	La manera de salvar los trucos de juego en un pequeño menú con una breve descripción.
X  --	Nuevas transitiones (me encanta esto, no se realmente por qué).
X  --	Por el momento, ¿ por qué no una traducción de este documento a otros idiomas ?
X  --	Da unos pocas texturas para la página web, el selector de ficheros o el fondo del GUI
	 ¿ Ha visto lo que ]pit[ ha hecho con el logo ;) ?
X  --	Auto ajuste de fotogramas
 X --	Quizá un pequeño instalador para la primera instalación.
 X --	Un bonito GUI para cambiar las opciones miestras se juega.
 X --	Precalcular los sprites (puede incrementar la velocidad)
 X --	Hacer entender a la gente que necesito desesperadamente alguna alimentación
 	¿ la ha hecho, amado, odiado, lo come?
 X --	Nuevas propiedades para ayudar a los desarrolladores de PCE (Ejemplo: ver/editar, ...)
 X --	Emulación de CD e ISO/MP3
  X--	Cambiar el volcado de fuentes (de Marat Fayzullin a Dave Shadoff)
  X--	Localización del error que hace que algunos canales de sonido no se oigan.
  X--	Migrarlo a otras plataformas (Linux, Win32, BeOS, Amiga etc...)

- [ TRUCOS ]--------------------------------------. 

	He reflexionado largamente sobre incluirlo como opción. Si no me equivoco, Hu-Go! es
	el primer emulador en proponer este tipo de opción.
	El uso de los trucos es muy primitivo y no hay apoyo visual, por lo que no se ve lo
	que se escribe...

	Pero...

	¡ Funciona ! Si quiere hacer trucos con el número de vidas en un juego, primero debe
	ejecutar el juego, después pulsar F1, el juego se para. Esto es normal. Está esperando
	el valor a buscar. Introduzca el número de vidas que actualmente tiene. De hecho sólo
	considera el el byte más bajo del valor introducido. Hace la búsqueda y muestra el
	resultado. Sigua jugando y pierda una vida y repita la operación como antes.
	Vuelva a perder una vida y repita hasta que sólo quede un valor común.

	Anote cuidadosamente este valor, es el desplazamiento en RAM de la variable que
	contiene el valor que ha introducido. ¿ Es correcto este valor ? Entonces pulse F2,
	el juego se para, e introduzca el desplazamiento y el nuevo valor, normalmente debería
	tener las vidas pedidas.
	
	Si obtiene un "SEARCH FAILED" (BUSQUEDA ERRONEA) entonces ... es que ha fallado, mire
	a continuación para buscar ayuda.

	Usando este método, el valos se cambia sólo una vez, pero si pulsa F3 e introduce el
	desplazamiento y el valor, este será cambiado unas 60 veces por segundo, forzandolo.
	Pulsando F3 entonces esta dirección será desbloqueada.

- [OBSERVACIONES PARA LOS TRUCOS]-------------------------------------


	--- 	Sea cuidadoso, debe pensar que los trucos no siempre funcionan, de hecho,
		alguna variables no se actualizan en cada pantallazo, por eso trate de
		perder una vida o si ha modificado el dinero que tinene, trate de comprar algo,
		así verá la diferencia.

	--- 	No siempre debe buscar valores modificados, puede buscar dos vidas
		y más tarde volver a buscarlas. Le sugiero que cuando busque el mismo valor lo
		haga rápidamente. Si la separación entre busquedas es de medio segundo, entonces
		eliminará una gran cantidad de "malas variables".

	---	Una vez que la búsqueda está completada y no localiza una gran cantidad de
		valores cada vez que busca, se puede quedar atascado porque dos variables
		mantienen el mismo valor, puede ser que una sea para el programa y la otra
		para pintar en pantalla. Si este es su caso, compruebe el fichero ROM.FP0,
		donde ROM es el nombre de su rom, aquí verá los posibles desplazamientos,
		con lo que saldrá del atasco.

	---	Las sesiones de búsqueda se mantienen entre dos ejecuciones del emulador.

	---	Si quiere iniciar una nueva sesión de búsqueda, fuerze un "SEARCH FAILED",
		buscando 25, luego 6 y luego 79 en un plazo reducido, si no puede forzar un
		"SEARCH FAILED", entonces será el siguiente en mi lista de cobayas para probar
		el eslabón directo sináptico ;).

	---	Algunas veces el valor no es almacenado tal y como se muestra, por ejemplo
		si tiene cinco vidas, intente buscar cuatro o seis, o sea "valor + 1" o 	
		"valor - 1", debería funcionar... (esto ocurre con el número de bolas en
		Devil Crash).

	---	La búsqueda sólo funciona con bytes, pero la variable puede ser de 2, 3 o
		más bytes. En este caso intente modificar un byte por encima o debajo del
		encontrado (Ejemplo: si modificando la dirección X obtiene una moneda más,
		cambiando la dirección X+1 debería modificar mucho las monedas).


- [RINCON DEL PROGRAMADOR]-------------------------------------

	Esta sección no es para usuarios normales (o jugadores). Está diseñada para la gente
	que sabe cómo funciona internamente la PC Engine y que quier aprender más, rastrear
	sus propias roms, traducir, etc...

	¡Bien!, el resto sólo debería ser leído por la gente interesada  ;)
	
	Lo primero es saber que cuando pulsa una tecla, el efecto se produce cuando la
	PC Engine lee la entrada. Esto explica porqué usted sólo puede ver la misma
	fracción de código cada vez que pulsa la tecla de desensamblar (creo).

	Cuando desensambla, se puede mover fácilmente haca adelante, pero cuando retrocede
	se intentan desensamblar 1,2,3,4 o 7 bytes antes. Se espera que en el primer resultado
	no se obtengan instrucciones ilegales. Si esto no funciona entonces debe retroceder
	un byte.

	No hay mucha protección, no debería ser muy dificil colgar el emulador y al ordenador
	al mismo tiempo si lo fuerza mucho. Sin envargo, si ha encontrado una forma de hacer
	consistente el cuelgue, en determinadas circustancias, por favor dígamelo.

	Los puntos de ruptura se implementan usando instrucciones ilegales 0x?B, los primeros
	14 los puede definir el usuario, el último se reserva para "ir al cursor", paso a paso,
	rastreo y otros usos. Y el otro está hecho para simular la ejecución, Cuando se para
	en un punto de ruptura, usted puede querer continuar, y puede hacerlo, pero ¿donde 
	guardo el puntero de el lugar que se estaba ejecutando ?
	Es por eso que uso un pseudo punto de ruptura, que se para justo después de la
	ejecución de la instrucción previa, la reemplaza y se reemplaza así mismo, por
	eso la próxima vez el puntero estará ahí.
	Estoy obligado a esto porque no tengo una función de paso a paso, y he intentado
	simularla tan bien como he podido, con la función de ejecutar a máxima velocidad,
	(Si conoce una mejor forma de hacerlo [sin mucho trabajo ;)] digamela), por ahora
	no funciona a la perfección, pero funciona.
	
	Cuando se desensambla, las instrucciones inválidas de los puntos de ruptura, sólo
	se reemplazan por instrucciones válidas si no son el primer byte de la instrucción,
	quiero decir, que si coloca un punto de ruptura, al retroceder en el codigo fuente,
	se pierde la instrucción, teniendo argumentos invalidos para las instrucciones 
	precedentes donde están los puntos de ruptura.
	Esto no es muy claro, por eso se lo aviso. Si esto le molesta, puedo cambiar los
	puntos de ruptura, pero entoces el desensamblado sería más lento...

	Por el momento los puntos de ruptura no están completamente implementados, con F2
	puede ponerlos y retirarlos, con lo que la ejecución se renauda...
	
	Con F7 y F8, puede ejecutar las instrucciones una por una. Cada vez que pulsa F8,
	avanza una instrucción, cuando llegue a una llamada de subrutina, F8 ejecutará la
	subrutina entera, diferenciándose de F7 que se parará en la primera instrucción de 
	la subrutina.

	    Con F3 el desensamblador esperará a que introduzca un valor hexadecimal y 
	seleccione un punto de parada. Con F4, la rom se ejecuta hasta el desplazamiento
	actualmente seleccionado.

	Con "G", puede introducir la dirección a la que quiere ir en hexadecimal, sin 
	prefijarla.

	No he comprobado a fondo estas funciones, si encuentra algún problema, intente
	reproducir los pasos para hecermelos llegar y que pueda localizar el error y hacerlo
	desaparecer del planeta Hu-Go!.

	Para poder utilizar "arriba", "abajo", "página arriba", "página abajo"; F1 les da un
	pequeña pantala de ayuda.

	El editor de ram se llama con "r" mientras se encuentre en la pantalla de desensamblado.
	Para moverse utilice las teclas normales, con "espacio" reemplazará el valor actualmente
	seleccionado, introduzca el nuevo valor y valídelo con "entrada" o anule el cambio usando
	"escape".
	
	Para la emulación de un CD (y uno normal), he introducido dos opciones:
	Con "i", tendrá una pantalla de información de dos páginas, pero que se puede ampliar
	si es necesario, dando información de los registros generales y de las variables
	del CD. Usando derecha e izquierda, pasará de una página a otra, para salir use F12 o
	escape.

	Con "w" en unteclado QWERTY, obtendrá un visualizador de página cero. Los primeros 256
	bytes será visibles pero inmodificables.

	Me gustaría que las personas que usen estas opciones me digan cómo les ha ido, me den
	consejos, trucos, ampliaciones que les gustaría ver implementadas. Sé que hay muchas
	cosas por hacer en este apartado.


--[ AGRADECIMIENTOS ]-----------------------------------------------------------------

   Especialmente agradecido a:
 *> -- Bacon boy 99 (Planet Rom webmaster [www.cantinaband.com/planetrom/]<*
       por el logo y de hecho muchas cosas)
    -- ]pit[ por toda su ayuda (contando todo el nuevo logo y banners)
    -- Nyef por toda la información que me ha dado para la emulación de unc CD
    -- TOZTWO por el bonito documento HTML
    -- fixo, cools, sALTY, codemaster, Raoh, MrF, dus, ElmerYud, Yuu y todos
       los #emu dudes de IrcNet ;)
    -- Dirk Steven (EAGLE es especialmente bueno)
    -- Dave Shadoff (TGSIM y TGSIMGPP, cientos de correos y miles de ideas,
       autor de la pequeña rom incluida, muy interesante)
    -- Marat Fayzullin (CODIGO BASE del 6502)
    -- Bero (Código addicional)
    -- Hmmx (más código addicional)
    -- DJ Delorie (DJGPP, ¡que es algo grande!)
    -- Shawn Hargreaves (ALLEGRO es grande)
    -- David Michel (¡ MAGIC ENGINE todavía es el mejor emulador!, además el es muy amigable
	    y sabe estar. Además es el autor del Magic Kit que le perimte crear sus propios
       PC Engine games; la demo incluída viene con ejemplos Magic Kit)
    -- Emu unlimited (www.emuunlim.com)
       por almacenar mi página.
    -- Al grupo info-zip por hace el unzip más facil

    -- Zophar domain (www.zophar.net)
    -- Carlos Hassan (su SEAL es muy util aunque la licencia sea muy restrictiva)
    -- Jens Christian Restemeier (DOCS)
    -- Joe LoCicero (DOCS)
    -- Bt Garner (DOCS)
    -- David Woodford (DOCS)
    -- Videoman (DOCS)
    -- Charles W Sandmann (CWSDPMI, este programa está bajo licencia GPL y usted puede
       obtener el fuente en http://www.delorie.com/djgpp/ )
    -- Markus F.X.J. Oberhumer & Laszlo Molnar (contribución para UPX, un excelente
       empaquetador de EXE, usado en esta versión)

    Agradecimientos diversos:
    -- Bouli (www.multimania.com/booemu/
              arcades.le-village.com/megrom/)
       por usar Hu-Go! por sus pantallazos ;)
    -- Michael Sheldows (www.homestead.com/sheldows)
       por intentar migrar Hu-Go! a su nuevo sistema operativo
    -- Mr Iceberg (www.multimania.com/pceaudit)
       por ofrecer u gestor de roms PCE  basado en FPCEID

	Otros, si el olvidado a alguno, lo siento.
	(envieme un correo si cree que debería aparecer en los créditos...)

	Si tiene cualquier sugerencia, proposición para ayudar ofreciendo hospedaje,
	informe de errores, o juegos que no funcionan, no lo dude, envieme un correo
	(me encantan los correos) a : ZEOGRAD@CARAMAIL.COM

	(También puede enviar dinero, laves de coche, tarjetas de credito con el código,
	video juegos, novias, pregunte por la dirección de envío ;)

      .  ___   ____                _________           ___
       .|  `|_|   `| ___ ___  __ .|    ____/ _______ .|  `|
     .:|     _     ||  `|  `|(__)|    (_  `|/   _  `\|____/:.
...:::||____|:|____||_______/::::\_________/\_______/(____)::::...
      `-*-- -  -   -    -      -            -              ^cls
