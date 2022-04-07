#include "osd_keyboard.h"
#include "osd_machine.h"
input_config config[16]={
             {
              {0,0,0,0,0},

              {KEYBOARD1,NONE,NONE,NONE,NONE},

               {
                {KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEY_ALT,KEY_SPACE,
                 KEY_TAB,KEY_ENTER,KEY_C,KEY_X},
                {},
                {},
                {},
                {}
                }
              }
             };
// info about the input config

UChar current_config;
// the number of the current config

char tmp_buf[100];

#define	JOY_A		0x01
#define	JOY_B		0x02
#define	JOY_SELECT	0x04
#define	JOY_START	0x08
#define	JOY_UP		0x10
#define	JOY_RIGHT	0x20
#define	JOY_DOWN	0x40
#define	JOY_LEFT	0x80
#define JOY_AUTOI	0x100
#define JOY_AUTOII	0x200

extern int UPeriod;

extern unsigned char joy_mapping[5][16];

char auto_fire_A[5];

char auto_fire_B[5];

char already_fired_A[5]={0,0,0,0,0};
/* Used to know whether the auto fire must set or not the button */

char already_fired_B[5]={0,0,0,0,0};
/* Used to know whether the auto fire must set or not the button */

/* for nothing */
UInt16 noinput()
 {
  return 0;
  }


/* for keyboard */
UInt16 read_keyboard(int port)
{
 UInt16 tmp=0;

     if (key[config[current_config].joy_mapping[port][J_DOWN]])
		 tmp|=JOY_DOWN;
     else
	    if (key[config[current_config].joy_mapping[port][J_UP]])
		   tmp|=JOY_UP;

     if (key[config[current_config].joy_mapping[port][J_LEFT]])
		 tmp|=JOY_LEFT;
     else
       if (key[config[current_config].joy_mapping[port][J_RIGHT]])
		   tmp|=JOY_RIGHT;

     if (key[config[current_config].joy_mapping[port][J_II]])
		 if (!auto_fire_A[port])
		   tmp|=JOY_A;
       else
			{
 			  already_fired_A[port]=!already_fired_A[port];
			  if (!already_fired_A[port])
				 tmp|=JOY_A;
			 }

     if (key[config[current_config].joy_mapping[port][J_I]])
		 if (!auto_fire_B[port])
		   tmp|=JOY_B;
       else
			{
 			  already_fired_B[port]=!already_fired_B[port];
			  if (!already_fired_B[port])
				 tmp|=JOY_B;
			 }

     if (key[config[current_config].joy_mapping[port][J_SELECT]])
		 tmp|=JOY_SELECT;

     if (key[config[current_config].joy_mapping[port][J_START]])
		 tmp|=JOY_START;

     if (key[config[current_config].joy_mapping[port][J_AUTOI]])
	 if (!key_delay)
         {
		     auto_fire_A[port]=!auto_fire_A[port];
			  key_delay=10;
         }

     if (key[config[current_config].joy_mapping[port][J_AUTOII]])
	 if (!key_delay)
	 {
		     auto_fire_B[port]=!auto_fire_B[port];
			  key_delay=10;
         }

 return tmp;
 }

UInt16 keyboard1()
{
  return read_keyboard(0);
 }

UInt16 keyboard2()
{
  return read_keyboard(1);
 }

UInt16 keyboard3()
{
  return read_keyboard(2);
 }

UInt16 keyboard4()
{
  return read_keyboard(3);
 }

UInt16 keyboard5()
{
  return read_keyboard(4);
 }

/* for joypad */
UInt16 read_joypad(int port)
 {
  UInt16 tmp;

  poll_joystick();

  tmp=0;

    if (joy[port].stick[0].axis[0].pos < 0)
		tmp|=JOY_LEFT;

    if (joy[port].stick[0].axis[0].pos > 0)
		tmp|=JOY_RIGHT;

    if (joy[port].stick[0].axis[1].pos < 0)
		tmp|=JOY_UP;

    if (joy[port].stick[0].axis[1].pos > 0)
		tmp|=JOY_DOWN;

    if (joy[port].button[config[current_config].joy_mapping[port][J_PSTART]].b
        || key[config[current_config].joy_mapping[port][J_START]])
		tmp|=JOY_START;
    
    if (joy[port].button[config[current_config].joy_mapping[port][J_PSELECT]].b
	|| key[config[current_config].joy_mapping[port][J_SELECT]])
		tmp|=JOY_SELECT;

    // Set Autofire I to Autofire II button status
    auto_fire_A[port]=(joy[port].button[config[current_config].joy_mapping[port][J_PAUTOI]].b
	|| key[config[current_config].joy_mapping[port][J_AUTOI]]);

    // Set Autofire II to Autofire II button status
    auto_fire_B[port]=(joy[port].button[config[current_config].joy_mapping[port][J_PAUTOII]].b
	|| key[config[current_config].joy_mapping[port][J_AUTOII]]);

    // Button I, fixed Autofire
    if (joy[port].button[config[current_config].joy_mapping[port][J_PI]].b
	|| key[config[current_config].joy_mapping[port][J_I]])
          {
	    if (auto_fire_A[port])
	        {
	             already_fired_A[port]=!already_fired_A[port];
    	             if (!already_fired_A[port])
	                  tmp|=JOY_A;
		}
	    else
	             tmp|=JOY_A;
	  }

    // Button II, fixed Autofire
    if (joy[port].button[config[current_config].joy_mapping[port][J_PII]].b
	|| key[config[current_config].joy_mapping[port][J_II]])
	  {
	    if (auto_fire_B[port])
		 {
	              already_fired_B[port]=!already_fired_B[port];
	              if (!already_fired_B[port])
	                   tmp|=JOY_B;
		 }
            else
	              tmp|=JOY_B;
	  }

  return tmp;
  }

UInt16 joypad1()
 {
  return read_joypad(0);
 }

UInt16 joypad2()
 {
  return read_joypad(1);
 }

UInt16 joypad3()
 {
  return read_joypad(2);
 }

UInt16 joypad4()
 {
  return read_joypad(3);
 }

/* for mouse */
UInt16 mouse1()
 {
  return 0;
  }

UInt16 mouse2()
 {
  return 0;
  }

/* for synaptic link */
UInt16 synaplink()
 {
  return 0;
  }


int osd_keyboard(void)
{
// char tmp_joy;

 while (key[KEY_PAUSE]) ;

 if(( key[KEY_ASTERISK]) && (!key_delay))
	{
     UInt32 sav_timer = timer_60;

    if (sound_driver == 1)
      set_volume(0,-1);

	  disass_menu();

    if (sound_driver == 1)
      set_volume(gen_vol,-1);

     timer_60 = sav_timer;
	  key_delay=10;
   }

 if(( key[KEY_F4] ) && (!key_delay))
   {
     UInt32 sav_timer = timer_60;
     UChar error_code;

     error_code = gui();

     key_delay=10;
     timer_60 = sav_timer;

     if (error_code)
       return 1;

    }

 if(( key[KEY_ESC] ) && (!key_delay))
	{
     UInt32 sav_timer = timer_60;

/*
     old_vol=voice_get_volume(PCM_voice);
     voice_set_volume(PCM_voice,0);
*/
   if (sound_driver == 1)
     set_volume(0,-1);

     //menu(); //There will be the GUI

	  while (keypressed())
		 readkey();

	  select_rom("*.pce");

	  while (keypressed())
		 readkey();

     key_delay=10;
     timer_60 = sav_timer;

     if (!strcmp("TRUE EXIT",selected_rom))
       {
	   cart_reload=0;
           return 1;
       }

/*
     voice_set_volume(PCM_voice,old_vol);
*/
  if (sound_driver == 1)
     set_volume(gen_vol,-1);


  if (strcmp("NO FILE",selected_rom))
	 {
	   cart_reload=1;
	   strcpy(cart_name,selected_rom);
	   return 1;
       }


   }

 if( key[KEY_F12] )
   return 1;

if (!key_delay)
  {
 if( key[KEY_F6] )
   {
     UInt32 sav_timer = timer_60;
		 {

//	      AGetVoiceVolume(hVoice,&vol);
//	      ASetVoiceVolume(hVoice,0);

         if (!silent)
           {
/*
         voice_stop(PCM_voice);
         set_volume(1,1);
         voice_set_volume(PCM_voice,1);
         voice_start(PCM_voice);
*/
            if (sound_driver == 1)
               set_volume(0,-1);
           }
       }

     if (!savegame())
       {
        osd_gfx_set_message(MESSAGE[language][game_save]);
        message_delay=180;
       }

	  if (!silent)
		 {
	if (sound_driver == 1)
          set_volume(gen_vol,-1);
       }
     timer_60 = sav_timer;
   };

 if( key[KEY_F5] )
     {
       UInt32 sav_timer = timer_60;
       sprintf(tmp_buf,MESSAGE[language][screen_shot],osd_gfx_savepict());
       osd_gfx_set_message(tmp_buf);
       message_delay=180;
       key_delay=10;
       timer_60 = sav_timer;
     };

 if( key[KEY_F10])
      {
        synchro=!synchro;
        osd_gfx_set_message(MESSAGE[language][full_speed+synchro]);
        message_delay=180;
        key_delay=10;
      };

 if( key[KEY_NUMLOCK])
	  {
		if (dump_snd)
		  {
         UInt32 dummy;

         dummy = filesize(out_snd);

         fseek(out_snd,4,SEEK_SET);

         fwrite(&dummy,1,4,out_snd);

         dummy -= 0x2C;

         fseek(out_snd,0x28,SEEK_SET);

         fwrite(&dummy,1,4,out_snd);

         fclose(out_snd);
	 osd_gfx_set_message(MESSAGE[language][dump_off]);

	}
      else
       {
	 unsigned short tmp=0;

	 strcpy(tmp_buf,"SND0000.WAV");

	 while ((tmp<0xFFFF) && (exists(tmp_buf)))
	  sprintf(tmp_buf,"SND%04X.WAV",++tmp);

	 out_snd=fopen(tmp_buf,"wb");

         fwrite("RIFF\145\330\073\0WAVEfmt ",16,1,out_snd);

         putc(0x10,out_snd); // size
         putc(0x00,out_snd);
         putc(0x00,out_snd);
         putc(0x00,out_snd);

         putc(1,out_snd); // PCM data
         putc(0,out_snd);

         putc(1,out_snd); // mono
         putc(0,out_snd);

         putc(freq_int,out_snd);    // frequency
         putc(freq_int >> 8,out_snd);
         putc(freq_int >> 16, out_snd);
         putc(freq_int >> 24, out_snd);


         putc(freq_int,out_snd);    // frequency
         putc(freq_int >> 8,out_snd);
         putc(freq_int >> 16, out_snd);
         putc(freq_int >> 24, out_snd);

         putc(1,out_snd);
         putc(0,out_snd);

         putc(8,out_snd);  // 8 bits
         putc(0,out_snd);

         fwrite("data\377\377\377\377",1,9,out_snd);

	 osd_gfx_set_message(MESSAGE[language][dump_on]);
      }

      dump_snd=!dump_snd;
      key_delay=10;
      message_delay=180;
    }

 if( key[KEY_PLUS_PAD])
      {
        if (UPeriod<15)
          {
            UPeriod++;
	    sprintf(tmp_buf,MESSAGE[language][frame_skip],UPeriod);
            osd_gfx_set_message(tmp_buf);
            message_delay=180;
          };
        key_delay=10;
      };

 if( key[KEY_MINUS_PAD])
      {
        if (UPeriod)
          {
            UPeriod--;
	    if (!UPeriod)
              osd_gfx_set_message(MESSAGE[language][all_frame]);
            else
	     {
	       sprintf(tmp_buf,MESSAGE[language][frame_skip],UPeriod);
	       osd_gfx_set_message(tmp_buf);
              };
            message_delay=180;
          };
        key_delay=10;
      };

  if (key[KEY_1])
    {
	   SPONSwitch=!SPONSwitch;
           key_delay=10;
    }
  if (key[KEY_2])
    {
	   BGONSwitch=!BGONSwitch;
	   key_delay=10;
    }
  }



 if( key[KEY_F1] )
  {
   UInt32 sav_timer = timer_60;

   if (sound_driver == 1)
    set_volume(0,-1);

   searchbyte();

   if (sound_driver == 1)
    set_volume(gen_vol,-1);

   timer_60 = sav_timer;
   return 0;
  }

 if( key[KEY_F2] )
  {
   UInt32 sav_timer = timer_60;

   if (sound_driver == 1)
    set_volume(0,-1);

   pokebyte();

   if (sound_driver == 1)
    set_volume(gen_vol,-1);

   timer_60 = sav_timer;
   return 0;
  }

 if( key[KEY_F7] )
   {
     UInt32 sav_timer = timer_60;

     if (!loadgame())
       {
         osd_gfx_set_message(MESSAGE[language][game_load]);
         message_delay=180;
       }

     timer_60 = sav_timer;
   };


 if( key[KEY_F3] )
  {
    UInt32 sav_timer = timer_60;

    if ( sound_driver == 1 )
      set_volume(0,-1);

    freeze_value();

    if (sound_driver == 1)
      set_volume(gen_vol,-1);

    timer_60 = sav_timer;
    return 0;
   }

 if (key[KEY_TILDE])
    {
     char* tmp=(char*)alloca(100);
     sprintf(tmp,"FRAME DELTA = %d",frame - HCD_frame_at_beginning_of_track);
     osd_gfx_set_message(tmp);
     message_delay=240;
     }

  if (key[KEY_3])
    //cd_port_1800 = 0xD0;
    cd_port_1800 &= ~0x40;

  if (key[KEY_4])
    _Wr6502(0x2066,Rd6502(0x2066) | 32);

  if (key[KEY_5])
    cd_port_1800 = 0xD8;

  if (key[KEY_6])
    _Wr6502(0x22D6,1);

  if (key[KEY_8])
    _Wr6502(0x20D8,128);
    //_Wr6502(0x20D8,Rd6502(0x20D8) | 128);


 if (key[KEY_MINUS])
   {
   if (gen_vol)
     gen_vol--;
   if (sound_driver == 1)
     {
      set_volume(gen_vol,-1);
      sprintf(tmp_buf,MESSAGE[language][volume_set_to],gen_vol);
      osd_gfx_set_message(tmp_buf);
      message_delay = 60;
     }
   }

 if (key[KEY_EQUALS])
   {
   if (gen_vol<255)
     gen_vol++;
   if (sound_driver == 1)
     {
      set_volume(gen_vol,-1);
      sprintf(tmp_buf,MESSAGE[language][volume_set_to],gen_vol);
      osd_gfx_set_message(tmp_buf);
      message_delay = 60;
     }
   }

 if (key[KEY_0])
   {
    if (freq_int<44100)
      freq_int+=50;
    if (sound_driver == 1)
      {
       voice_set_frequency(PCM_stream->voice,freq_int);
       sprintf(tmp_buf,MESSAGE[language][freq_set_to],freq_int);
       osd_gfx_set_message(tmp_buf);
       message_delay = 60;
      }
    }

 if (key[KEY_9])
   {
    if (freq_int>11025)
      freq_int-=50;
    if (sound_driver == 1)
      {
       voice_set_frequency(PCM_stream->voice,freq_int);
       sprintf(tmp_buf,MESSAGE[language][freq_set_to],freq_int);
       osd_gfx_set_message(tmp_buf);
       message_delay = 60;
      }
    }

 io.JOY[0]=(*config[current_config].pad[0])();
 io.JOY[1]=(*config[current_config].pad[1])();
 io.JOY[2]=(*config[current_config].pad[2])();
 io.JOY[3]=(*config[current_config].pad[3])();
 io.JOY[4]=(*config[current_config].pad[4])();

 return 0;
}
