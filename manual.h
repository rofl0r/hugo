static char manual_content[] = "\
            .  ___   ____                _________           ___\n\
             .|  `|_|   `| ___ ___  __ .|    ____/ _______ .|  `|\n\
           .:|     _     ||  `|  `|(__)|    (_  `|/   _  `\\|____/:.\n\
      ...:::||____|:|____||_______/::::\\_________/\\_______/(____)::::...\n\
            `-*-- -  -   -    -      -            -              ^cls\n\
                Core-GraphX / PC Engine / Turbo GraphX Emulator\n\
                             Version 2.10\n\
                             04 March 2003\n\
\n\
\n\
        N.B. When reading this .TXT - \"(*)\" indicates the main changes\n\
\n\
--[ CONTACT INFORMATION ]----------------------------------------------------\n\
    E-Mail :   zeograd@zeograd.com \n\
\n\
    ICQ UIN :  40632933\n\
\n\
    Homepage : http://www.zeograd.com\n\
\n\
\n\
--[ DISCLAIMER ]-------------------------------------------------------------\n\
\n\
    I can't be held responsible for any misuse of this software, it comes\n\
    with absolutely no warranty. Use it at your own risks.\n\
\n\
--[ INTRODUCTION ]-----------------------------------------------------------\n\
\n\
    Hi everyone, Welcome to the Hu-Go! documentation.\n\
    I (Zeograd) am not really the author of this program. I discovered\n\
    sources of a PC Engine emulator at Bero's Homepage\n\
    (http://www.geocities.co.jp/Playtown/2004/)\n\
\n\
    Plus improvements at Hmmx's Homepage\n\
    (formerly, http://www.geocities.co.jp/SiliconValley/7052/).\n\
\n\
    Hmmx made XPCE (for DirectX) but I've been interested in continuing\n\
    their work under DOS and voila, you've got another emulator available\n\
    to burn your keyboard playing all night with great PCE games ;)\n\
\n\
    This one is free and you've got access to sources as Bero did with his\n\
    work. I'd like people interested in making a good emulator for free to\n\
    contact me to talk about what is missing in Hu-Go!. Don't hesitate\n\
    answering the poll on my webpage or filling the forum you can find there.\n\
\n\
--[ HOW TO INSTALL IT ? ]----------------------------------------------------\n\
\n\
    Hu-Go! is now available in binary form for several platforms, such as\n\
	Microsoft windows, i386 debian, i386 Mandrake as well as in sources form\n\
	which can be compiled on various environments.\n\
	\n\
	Check the INSTALL file for further instructions concerning the source\n\
	distribution.\n\
	\n\
--[ HOW TO USE IT ? ]--------------------------------------------------------\n\
\n\
    You can hardly make it easier (maybe with a direct synaptic link between\n\
    your brain and your computer to guess the name of the rom...) :\n\
	Just type hugo with no options to bring the gtk gui which is quite straight\n\
	forward. You can load a rom (or .iso) or fire a cd. You can load and save\n\
	file using the menu, or change the settings, which are those you can change\n\
	in the .ini file.\n\
    You can also use hugo without the gui, just type\n\
	hugo [ROM] [BACKUPMEM] [OPTION] where ROM is the name of the\n\
    ROM or ISO or ISQ or HCD. Check out the HCD section later for details.\n\
    you want to play with. If ROM has no extension, Hu-Go!\n\
    will look for ROM.PCE then ROM.ZIP. If the ROM has PCE extension, try it,\n\
    then try ROM in ROM.ZIP. Then if ROM has ZIP extension, look for the\n\
    PCE file with same basename as ROM...\n\
    ISO games must be specified with ISO extension to be recognized.\n\
\n\
    (Recommended games - try SUPER STAR SOLDIER or NEUTOPIA 2, you won't\n\
    regret it!. If you don't have any rom, you can try the little demo\n\
    rom included made with Huc [www.zeograd.com])\n\
\n\
    For OPTIONS, see later...\n\
\n\
    BACKUPMEM is the name of the file containing the backup RAM.\n\
    These two last are optionals. The order shouldn't import but the ROM name\n\
    must be before the name of backup RAM.\n\
\n\
--[ HOW TO CONFIGURE HU-GO! FOR PLAYING CDS ? ]------------------------------\n\
\n\
	Under linux, you first have to configure the cd path (ie /dev/cdrom or such)\n\
	either on the command line (-i <cd path>), the .ini file (variable cd_path),\n\
	or in the gui, under the tab 'CD emulation'.\n\
	FreeBSD doesn't have yet the CD support.\n\
	For any platform, choose the system card rom location. It's the rom which\n\
	actually boots the CD, and it's called syscard.pce or such. As for all\n\
	commercial roms, you can only use it if you've bought a system card hucard\n\
	or an hardware containing it (Any duo for instance).\n\
	It's a known bug that you can't always play several CDs in a row without\n\
	restarting the whole emulator.\n\
\n\
--[ SYSTEM REQUIREMENTS ]----------------------------------------------------\n\
\n\
    -- Pentium 166 (maybe less without sound; something like 233 or 266 for\n\
       good sound).\n\
\n\
    -- 8MB of RAM.\n\
\n\
    -- A Linux system or FreeBSD or Microsoft windows (only 95 to XP tested)\n\
\n\
    -- A sound card (only if you want to hear sound, of course).\n\
\n\
    -- A small amount of brain cells (enough to control basic motor\n\
       functions. For Cheating and Disassembling, a few trillion more may be\n\
       required ;)\n\
\n\
    -- Fingers (may be replaced by a direct synaptic link, I'm working on it,\n\
       if you want to become a guinea pig, let me know too ;) Note - requires\n\
       brain cells specified above.\n\
\n\
--[ KEYBOARD CONTROLS ]------------------------------------------------------\n\
\n\
    Cursor keys          Direction\n\
    Alt                  Button 1\n\
    Space                Button 2\n\
    Tab                  Select key\n\
    Enter                Start key\n\
    X                    Toggle Autofire 1\n\
    C                    Toggle autofire 2\n\
\n\
    (These are default keys for all players)\n\
    You can edit yours using the HU-GO!.INI (dos/windows) or hugo.ini (un*x)\n\
	file (See details later)\n\
\n\
    Special keys:\n\
\n\
    Can't be changed (you can always assign these keys to \"normal\" ones,\n\
    it will mostly have the \"special\" effects, eventually followed by the\n\
    \"normal\" ones)\n\
    \n\
\n\
    tilde key (under ESC key)   Display the number of frame elapsed\n\
             since the beginning of the track (to create subtitles)\n\
    1 on alpha pad   Toggle the sprites display\n\
    2 on alpha pad   Toggle the background display\n\
\n\
-- alpha keys --\n\
   3,4,5,6,7 and 8 on alpha pad\n\
                     May unblock some isos\n\
-- alpha keys --\n\
\n\
    - on alpha pad   Lower the sound level\n\
    = on alpha pad   Raise the sound level\n\
    9 on alpha pad   Lower the sound quality\n\
    0 on alpha pad   Raise the sound quality\n\
    F1               Enter a value to search for (See details later)\n\
    F2               Set a value in RAM (See details later)\n\
    F3               Freeze a value in RAM (See details later)\n\
    F4               Test purpose key, now display the draft gui\n\
    F5               Save ScreenShot (named pict????.pcx) (disabled right now)\n\
	F9				 Toggle full screen mode\n\
    ESC, F12         Exit (not really useful)\n\
    F6               Save game progression (named ROM.SAV where ROM is the\n\
                     ROM name, Also work for iso file in file names ISO.SVI,\n\
                     For games on CD, only 1 saved game is allowed, whatever the CD)\n\
    F7               Load game progression\n\
    'Num Lock'       Toggle sound dumping mode\n\
\n\
    Keys Used on Numeric Keypad\n\
    /                Toggle image dumping mode (in VIDEO/*.PCX)\n\
    +                Skip one more frame per sec.\n\
    -                Skip one less frame per sec.\n\
\n\
--[ OPTIONS ]----------------------------------------------------------------\n\
\n\
    There are two kinds of options. Command line or HU-GO!.INI options. The\n\
    emu first reads in the INI file then the command line, so you can\n\
    over-ride general options.\n\
\n\
    ** Command line options\n\
    Can be either -FV or -F V where F is the flag and V the value\n\
\n\
   -- c : CD emulation (This way of emulating ISO is no more compulsory but still works,\n\
                         is compulsory for using CDs)\n\
        0 -> Only Hu-card emulation (Default)\n\
        1 -> CD emulation on\n\
        2 -> ISO file emulation on\n\
        3 -> ISQ file emulation on\n\
        4 -> BIN file emulation on (it's a raw full dump, only 1 track supported\n\
          right now, and only if <280 Mb)\n\
		  \n\
	-- f : Fullscreen model\n\
		0 -> Start in window mode\n\
		1 -> Start in fullscreen mode\n\
\n\
    -- m : No bios mode\n\
        0 -> Use bios as much as possible (Default, faster but sometime less\n\
                                           accurate)\n\
        1 -> Use no bios at all as possible (slower but make more game works)\n\
\n\
	-- s : Stereo mode\n\
		0 -> Mono mode (Default)\n\
		1 -> Stereo mode\n\
\n\
    -- u : US encoded rom\n\
        0 -> Normal rom (Default)\n\
        1 -> US encoded rom, with invertion of bits\n\
       Hu-Go! should automaticaly recognize such rom but if not (i.e.\n\
       it works only is -u1 is specified) tell me so that I can add it\n\
       to the database.\n\
	\n\
	-- w : Window size multiplier\n\
		1 -> normal window size (Default)\n\
		2 -> doubled window size\n\
		3 -> tripled window size\n\
		4 -> the meaning of this option is let to the reader's attention		\n\
\n\
    ** Initialisation file options\n\
    This file accepts comments in lines beginning with a #, the form is\n\
    F=V where F the name of the option and V the value.\n\
\n\
    --KEYS : it's a bit long to explain in details but I'll explain the\n\
    method. Each function is represented by a string (\"UP\",\"DOWN\",\"LEFT\",\n\
    \"RIGHT\",\"I\",\"II\",\"SELECT\",\"START\",\"AUTOI\",\"AUTOII\") immediately followed\n\
    by the number of the contoller. The value is then the scancode as you\n\
    can find it in the SCANCODE.TXT.\n\
    When mapping a joypad related configuration, you can also use \"PI\",\"PII\",\n\
   \"PSELECT\", \"PSTART\", \"PAUTOI\", \"PAUTOII\" to map button I, II, Select, Start\n\
    and autoI, autoII. The given value of the right being the number of the\n\
    button on your pad, of course.\n\
    Now, you can create up to 15 different configs by using sections like\n\
    [config5]. These sections must be at the end of the INI files. They\n\
    contain key definitions as explained above but also info on controllers\n\
    e.g. putting input2=key3 will make the 2nd pad of the PC Engine to be\n\
    controlled by the 3rd keyboard mapping (UP3, LEFT3,...). As valid strings\n\
    you have 'input1' to 'input5' for the 5 PC Engine Gamepads (only 4\n\
    emulated so far), and 'key1' to 'key5' for 5 key mappings, 'joy1' to\n\
    'joy4' for 4 different joypads, 'mouse1' and 'mouse2' for 2 mice (not yet\n\
    implemented) and 'synaplink' for a direct synaptic link (also not\n\
    implemented ;)\n\
\n\
    E.G. :\n\
    Setting in the INI file\n\
\n\
     config=1\n\
\n\
     [config1]\n\
\n\
       input1=key1\n\
\n\
       UP1=0x16\n\
       LEFT1=0x23\n\
       DOWN1=0x24\n\
       RIGHT1=0x25\n\
\n\
       input2=joy1\n\
\n\
    This will change direction keys for the first player to 'u','h','j','k'\n\
    while the second player will play with the first joypad. Note that when\n\
    you specify a joy input, only the directions and the 2 buttons are\n\
    remapped so you can still choose keys for select, start, autoI and\n\
    autoII. As an example, the config1 is just a standard input with 1 player\n\
    using a joypad so if you want to play with a joypad just uncomment the\n\
    line with config=1. It's not very clear but the built-in menu allows easier\n\
	configuration.\n\
\n\
    -- language : Language of most messages during the emulation\n\
          0 -> English (default) (by me)\n\
          1 -> Français (by me)\n\
          2 -> Espanol (by my gf, and Juan roman Soriano)\n\
          3 -> Slovensko (by ]pit[ )\n\
          4 -> Portuguese (by CodeMaster)\n\
          5 -> German (by sALTY)\n\
          6 -> Dutch (by Fixo)\n\
          7 -> Polish (by Dox and Faust)\n\
          8 -> Italian (by Gareth Jax)\n\
		  9 -> Russian (by Lord Mardulak)\n\
\n\
    -- rom_dir : Directory of your roms. It's the default directory the\n\
    fileselector will look in. Use either '/' or '\\'.\n\
    (If no value given, current path is assumed)\n\
\n\
    -- snd_freq : Frequency of the PCM sound output. The higher, the best\n\
    quality but the slower. Use 11025 on slow machines and 22050 or 44100\n\
    on fast machines. You can also give intermediate values.\n\
       22050 Hz is default\n\
\n\
    -- buffer_size : Size of the buffer used for PCM sound output. the best\n\
    I can tell you here is to make tests. All depends of the frequency you use\n\
    and your machine speed. 256 bytes is the default (Fine for XP1900+) you can\n\
	also try 512 or 1024 if you got sound which stops at times.\n\
	\n\
	-- cd_path : Device name of the cd drive under UN*X systems.\n\
	   /dev/cdrom is default\n\
	   \n\
	-- stereo : Whether to output sound in stereo.\n\
	   0 -> Mono sound (Default)\n\
	   1 -> Stereo sound\n\
	   \n\
	-- fullscreen_width : Preferred width of fullscreen mode\n\
	   640 is the default\n\
	   \n\
	-- fullscreen_height : Preferred height of fullscreen mode\n\
	   480 is the default\n\
	   \n\
	-- window_size : Zoom factor when in windowed mode\n\
	   1 to 4 are valid zoom factor (1 being the default)\n\
	   \n\
\n\
--[ HCD ]--------------------------------------------------------------------\n\
\n\
   Then what is this new format.\n\
   HCD means : Hu-Go! Cd Definition. Such a file allow you to use multi-code\n\
   CDs and emulate audio tracks with MP3\n\
   It's only a beginning and there are still troubles with ISQ to emulate\n\
   Code tracks but it should work fine later.\n\
   HCD are divided in sections :\n\
   [main]\n\
   for main info :\n\
   first_track=XX\n\
   last_track=XX\n\
\n\
   that indicates the first and last track (easy)\n\
\n\
   ISO_path=XXX\n\
   MP3_path=XXX\n\
\n\
   which are the default path for ISO and MP3 files, convenient when moving\n\
   of directory e.g.\n\
   If you don't specify anything there, the default path is the .HCD one\n\
\n\
   minimum_bios=X\n\
\n\
   where X is 0 or 1. Use to enable this mode (see in command line option)\n\
   when launching a particular HCD. (Now, this mode is 'no bios' but for\n\
   obvious compatibility reason, I've not changed the name...)\n\
\n\
   Then for each track you can find a section :\n\
   [trackXX]\n\
   where you'll find :\n\
\n\
   filename=...\n\
\n\
   i.e. the filename of the file to use to emulate this track (.ISO or .ISQ\n\
   for code or .MP3 for audio)\n\
   then :\n\
\n\
   type=CODE\n\
\n\
   that specify if the track is a code one (audio is default)\n\
   et finally\n\
\n\
   begin=lsn,XXX\n\
\n\
   to specify the beginning of the track in LSN format (# of sector since the\n\
   beginning of the first one) or LBA (# of sector since the beginning of the\n\
   CD) or MSF (theoricaly, only LSN really tested) (as for MSF, just do :\n\
   begin=msf,mm:ss:ff\n\
   add zeros if compulsory)\n\
   even if just a beginning, it allows playing multi-code tracks\n\
   and MP3 in games. (even if .MP3 control isn't yet perfect)\n\
   You'll find an example in the archive.\n\
\n\
   There's now another mode available. It's the cd one.\n\
   Write\n\
   type=CD\n\
   and you'll have the ability to fake a track from the .hcd file with a\n\
   track coming from a real cd.\n\
   In this mode, you have 2 mode fields:\n\
   drive=0\n\
   which specify the drive in which is the track you want to use, using the\n\
   format in which the cd driver is used to. i.e. 0, 1 and such.. for aspi\n\
   driver and letter D, E, and such for the mscdex driver.\n\
\n\
   Then you can specify the number of the track to use on the cd driver with\n\
   track=5\n\
   If you don't specify this field, the default value for the track field will\n\
   be the track number of the track to fake.\n\
\n\
\n\
   You can also use patching functions\n\
\n\
   patch_number=XX\n\
\n\
   where XX is the number of patch you want to apply\n\
\n\
   then\n\
\n\
   patchX=0X????????,0X??\n\
\n\
   where X goes from 0 to patch_number-1 and two hexa numbers represent\n\
   the offset and new val of the patch in the ISO file.\n\
\n\
   On the same model, you can also find a way of adding subtitle when audio\n\
   tracks are played (even if you don't own the MP3 for it). You can choose\n\
   the number of subtitles with : \n\
   \n\
   subtitle_number=XX\n\
   \n\
   where XX is the number of subtitle you want to have for the track.\n\
\n\
   You can also choose to synchronize the subtitles with the game or the\n\
   music with\n\
\n\
   synchro_mode=XX\n\
\n\
   if XX = 0, synchronisation is done with the game as in the first release\n\
   supporting it, else it's synchronised with music and you can replace\n\
   the words 'number of frame' by '60th of sec' in the following explanation,\n\
   concerning the meaning of the begin field.\n\
\n\
   Then, for each subtitle, you must choose the time where it should begin,\n\
   its duration and the message to display.\n\
   The syntax is as follow :\n\
   \n\
   subtitleXX=<begin>,<duration>,<message>\n\
   \n\
   where XX is the number of the subtitle (XX is between 0 and subtitle_number-1)\n\
   <begin> is the number of frame elapsed since the beginning of the track.\n\
   There's normally 60 frame per sec but using the number of frame instead\n\
   of time make subtitles synchronized with the real game and not the music.\n\
   Since it's harder to guess how many frame have been displayed, I've added\n\
   a display of such a number with the tilde key. The value that you'll see\n\
   when pressing the key is the exact value you'll have to put in the <begin>\n\
   field (ajusting them a bit if needed...)\n\
   The <duration> field is in 60th of second, i.e. putting 60 there will\n\
   display the message for 1 sec and finally, the last field is the message\n\
   that will be displayed. It shouldn't contain any comma.\n\
   One last thing : you must sort the subtitle according to the order of\n\
   appearition.\n\
   Don't hesitate looking at the example or sending a mail if needed.\n\
\n\
--[ CHEATING ]---------------------------------------------------------------\n\
\n\
    I've pondered at length as to whether to include such an option. If I'm\n\
    not mistaken, Hu-Go! is the first emulator to propose this type of\n\
    option. Cheating is very primitive and there is no visual help so you\n\
    can't see what you type...\n\
\n\
    BUT...\n\
\n\
    It works! If you want to cheat with the number of lives in a game,\n\
    first you run your game, then you type F1, the game stops. This is\n\
    normal. It's waiting for the value to search. Then you enter the number\n\
    of lives you actually have (In fact, it only considers the lower byte of\n\
    the value you entered), then it makes a search and shows you the result.\n\
    Play around and lose a life, repeat the same as above, pressing F1 and\n\
    entering the current value. Repeat it again and again until the search\n\
    only finds one value..\n\
    Note carefully this value, it's the offset in RAM of a variable that\n\
    contains the value you've entered. Is it the correct value? Easy, press\n\
    F2, the game stops then enter the offset noted before and the new value.\n\
    Normally you should have as many lives as you want.\n\
    If you got a 'SEARCH FAILED' then ... it failed, see below for help.\n\
    Using this method, the value is only changed once, but if you press F3\n\
    then enter the offset and the value, it will be changed to the specified\n\
    value 60 times per sec., so it looks like it is frozen. Pressing F3 then\n\
    the address will unfreeze the value.\n\
    Now, you can research value using a relative method. that's to say that\n\
    instead of searching exact values, you can simply tell hu-go! to search\n\
    for variation of the variable. e.g. each time you lose a life, you can\n\
    make a research on -1, i.e. the variable you're looking for have been\n\
    decreased by 1. With this method, you don't have to worry about the base\n\
    value used to represent the variable (see hints concerning this).\n\
    When you want to make a relative research, you have to use F1 as\n\
    previously but instead of typing the plain number, you must prefix it\n\
    with '+' or '-'. For the first research, you don't have to worry about\n\
    the value you're giving, it's simply used to initialize the temporary\n\
    file containing all the possible variables with all the current values.\n\
    Once done, you can press F1 then '+1' or '-3' and such. From a technical\n\
    point of view, it's possible to mix normal and relative research but if\n\
    you do absolute research, there's no point on doing relative one.\n\
\n\
--[ CHEATING HINTS ]---------------------------------------------------------\n\
\n\
     -- Be careful, you may think the cheat didn't work, in fact, some\n\
     variables aren't updated each frame so try to lose a life or if you\n\
     modified money try to buy something... then you may see the difference.\n\
     -- You aren't obliged to search for altered values, you can make a\n\
     search for 2 lifes then 5 sec. later you search for 2 lifes again,...\n\
     I suggest you search for the same value twice very quickly. If you look\n\
     twice for the same value with only a half a sec. interval, you will\n\
     certainly eliminate a good amount of 'bad' variables.\n\
     -- Once a search is complete and you no longer get a lot values each\n\
     time you search, you may stay stuck because two variables have the same\n\
     value at the same moment (often for core and screen purposes). If that\n\
     is the case go and check the file named ROM.FP0 (where ROM is the name\n\
     of the rom), this contains the different possible offsets. It's up to\n\
     you.\n\
     -- Sessions of research are kept between two launches of the emulator.\n\
     -- If you want to start a new session, just make a 'SEARCH FAILED'\n\
     (Search for 25 then 6 then 79 in a very reduced interval, if you can't\n\
     get an 'SEARCH FAILED' then you will be the next on my guinea pig list\n\
     for trying the direct synaptic link ;).\n\
     -- Sometimes, the value on screen isn't stored like shown, e.g. if you\n\
     have 5 lives, maybe the variable 'LIFE' is set at 4 or 6, try to search\n\
     for 'value'+1 or 'value'-1, it may work... (like the number of balls in\n\
     Devil Cr*sh).\n\
     The best to avoid this kind of problem is using relative value research.\n\
     -- The search only works on bytes but a variable may take 2, 3 or more\n\
     bytes. In this case try modifying the byte just 1 byte above or below\n\
     the one you found (e.g. if you've found that putting a value at address\n\
     X would change the money a bit, putting a value at address X+1 will\n\
     certainly change the money a lot).\n\
\n\
--[ DEVELOPERS CORNER ]------------------------------------------------------\n\
\n\
	Hu-Go! now exposes a large part of the current game internal variables via\n\
	shared memory (only linux/freebsd for now, soon windows too).\n\
	You can use the shared memory of identifier 25679. It contains a structure\n\
	of type struct_hard_pce as defined in shared_memory.h (you should grab the\n\
	source archive of course). You can then alter or read all the quite self\n\
	explanatory variables out there. As for controlling the cpu, you can use\n\
	the s_external_control_cpu field.\n\
	  -1 => Full speed\n\
	  0  => Emulation paused\n\
	 >0  => Number of opcodes to run before pausing\n\
	You have to recompile Hu-Go! using --enable-shared-memory to benefit this\n\
	feature.\n\
\n\
--[ THANKS ]-----------------------------------------------------------------\n\
\n\
   Special thanks :\n\
 *> -- Bacon boy 99 (Ex-Planet Rom webmaster, the former logo,    <*\n\
       having sent me some Cds and in fact, lots of things)\n\
	-- Ixion for the lots of bugs/improvements he worked on recently\n\
	-- Blake- for the bugs he fixed while doing his pce emu on gp32\n\
    -- ]pit[ for all his help (among all the new logo and banners)\n\
    -- Nyef for all the infos he gave me on CD emulation\n\
    -- Lester Barrows for his help on the linux code (notably, pad support from\n\
              his work)\n\
	-- Bt Garner (ideas, lots of stuff including hosting my site now, ...)\n\
    -- Ishan (ex Vlendr'it) for the mailing list, tweak mode tests, finding\n\
       nice code resources on the web and scanline mode\n\
    -- TOZTWO for the nice HTML doc\n\
    -- Tormod Tjaberg for the MP3 length detection code\n\
    -- fixo, cools, sALTY, codemaster, Raoh, MrF, dus, Yuu and all the #emu\n\
       dudes on IrcNet ;)\n\
    -- ElmerYud coz' he's Hu-Go! publicity manager ^_^\n\
    -- tAGGY for two nice pictures\n\
    -- SirJaguar, Nico, Ded, ZePoulpe, Leto_II, Ziller, Readiosys and guys on\n\
       #utopiasoft or #nec4ever\n\
    -- Dirk Steven (EAGLE is really very nice)\n\
    -- Dave Shadoff (TGSIM and TGSIMGPP, longs mails and lots of ideas,\n\
       author of the little rom included, very nice guy)\n\
    -- Marat Fayzullin (BASE SOURCE for 6502)\n\
    -- Bero (Additional SOURCE)\n\
    -- Hmmx (More additional SOURCE)\n\
    -- DJ Delorie (DJGPP, what a great thing!)\n\
    -- Shawn Hargreaves (ALLEGRO is great)\n\
    -- David Michel (MAGIC ENGINE is still the best emu!!!, futhermore he's\n\
       very friendly and fair-play. Also the Author of the Magic Kit which\n\
       allow you to create your own PC Engine games)\n\
    -- The info-zip group for making unzipping easy (sometimes ^_^)\n\
\n\
    -- Zophar domain (www.zophar.net)\n\
    -- Jens Christian Restemeier (DOCS)\n\
    -- Joe LoCicero (DOCS)\n\
    -- David Woodford (DOCS)\n\
    -- Videoman (DOCS)\n\
    -- Charles W Sandmann (CWSDPMI, this program is under GPL license and you\n\
       can get the source at http://www.delorie.com/djgpp/ )\n\
    -- Markus F.X.J. Oberhumer & Laszlo Molnar (contribution to UPX, an\n\
       excellent EXE-packer used in this release)\n\
\n\
    Diverse thanks:\n\
    -- Bouli (www.multimania.com/booemu/\n\
              arcades.le-village.com/megrom/)\n\
       for using Hu-Go! for his screenshots ;)\n\
    -- Michael Sheldows (www.homestead.com/sheldows)\n\
       for trying to port Hu-Go! to a new OS of his own\n\
    -- Mr Iceberg (www.multimania.com/pceaudit)\n\
       for offering a PCE ROM manager based on FPCEID\n\
\n\
    Others I may have forgotten (Sorry!!!)\n\
    (send me a mail if you consider you should be credited...)\n\
\n\
    If you have any questions, suggestions, propositions for help or\n\
    mirroring, bug reporting, or non working games, don't hesitate,\n\
    send a e-mail (I love e-mails) to : ZEOGRAD@ZEOGRAD.COM\n\
\n\
    (You can also send money, car keys, credit card with PIN number,\n\
    video games, girlfriends, ask for snail mail address !! ;)\n\
\n\
      .  ___   ____                _________           ___\n\
       .|  `|_|   `| ___ ___  __ .|    ____/ _______ .|  `|\n\
     .:|     _     ||  `|  `|(__)|    (_  `|/   _  `\\|____/:.\n\
...:::||____|:|____||_______/::::\\_________/\\_______/(____)::::...\n\
      `-*-- -  -   -    -      -            -              ^cls\n\
\n\
";