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

extern int UPeriod;

extern unsigned char joy_mapping[5][10];

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

/* For joypad */
 struct js_event e;
 js_status joypad[4];


/* for keyboard */
UInt16 keyboard1()
{
 UInt16 tmp=0;

        /* TEST
     if (key[KEY_V])
       {
        already_fired_A[0] = !already_fired_A[0];
        if (already_fired_A[0])
          tmp|= JOY_A;
        else
          tmp|= JOY_B;
       } */

     if (key[config[current_config].joy_mapping[0][J_DOWN]])
		 tmp|=JOY_DOWN;
     else
	    if (key[config[current_config].joy_mapping[0][J_UP]])
		   tmp|=JOY_UP;

     if (key[config[current_config].joy_mapping[0][J_LEFT]])
		 tmp|=JOY_LEFT;
     else
       if (key[config[current_config].joy_mapping[0][J_RIGHT]])
		   tmp|=JOY_RIGHT;

     if (key[config[current_config].joy_mapping[0][J_II]])
		 if (!auto_fire_A[0])
		   tmp|=JOY_A;
       else
			{
 			  already_fired_A[0]=!already_fired_A[0];
			  if (!already_fired_A[0])
				 tmp|=JOY_A;
			 }

     if (key[config[current_config].joy_mapping[0][J_I]])
		 if (!auto_fire_B[0])
		   tmp|=JOY_B;
       else
			{
 			  already_fired_B[0]=!already_fired_B[0];
			  if (!already_fired_B[0])
				 tmp|=JOY_B;
			 }

     if (key[config[current_config].joy_mapping[0][J_SELECT]])
		 tmp|=JOY_SELECT;

     if (key[config[current_config].joy_mapping[0][J_START]])
		 tmp|=JOY_START;

     if (key[config[current_config].joy_mapping[0][J_AUTOI]])
	 if (!key_delay)
         {
		     auto_fire_A[0]=!auto_fire_A[0];
			  key_delay=10;
         }

     if (key[config[current_config].joy_mapping[0][J_AUTOII]])
	 if (!key_delay)
	 {
		     auto_fire_B[0]=!auto_fire_B[0];
			  key_delay=10;
         }

 return tmp;
 }

UInt16 keyboard2()
{
 UInt16 tmp=0;

	  if (key[config[current_config].joy_mapping[1][J_DOWN]])
		 tmp|=JOY_DOWN;
     else
	    if (key[config[current_config].joy_mapping[1][J_UP]])
		   tmp|=JOY_UP;

     if (key[config[current_config].joy_mapping[1][J_LEFT]])
		 tmp|=JOY_LEFT;
     else
       if (key[config[current_config].joy_mapping[1][J_RIGHT]])
		   tmp|=JOY_RIGHT;

     if (key[config[current_config].joy_mapping[1][J_II]])
		 if (!auto_fire_A[1])
		   tmp|=JOY_A;
       else
			{
 			  already_fired_A[1]=!already_fired_A[1];
			  if (!already_fired_A[1])
				 tmp|=JOY_A;
			 }

     if (key[config[current_config].joy_mapping[1][J_I]])
		 if (!auto_fire_B[1])
		   tmp|=JOY_B;
       else
			{
 			  already_fired_B[1]=!already_fired_B[1];
			  if (!already_fired_B[1])
				 tmp|=JOY_B;
			 }

     if (key[config[current_config].joy_mapping[1][J_SELECT]])
		 tmp|=JOY_SELECT;

     if (key[config[current_config].joy_mapping[1][J_START]])
		 tmp|=JOY_START;

     if (key[config[current_config].joy_mapping[1][J_AUTOI]])
		 if (!key_delay)
			{
		     auto_fire_A[1]=!auto_fire_A[1];
			  key_delay=10;
         }

     if (key[config[current_config].joy_mapping[1][J_AUTOII]])
		 if (!key_delay)
			{
		     auto_fire_B[1]=!auto_fire_B[1];
			  key_delay=10;
         }

 return tmp;
 }

UInt16 keyboard3()
{
 UInt16 tmp=0;

	  if (key[config[current_config].joy_mapping[2][J_DOWN]])
		 tmp|=JOY_DOWN;
     else
	    if (key[config[current_config].joy_mapping[2][J_UP]])
		   tmp|=JOY_UP;

     if (key[config[current_config].joy_mapping[2][J_LEFT]])
		 tmp|=JOY_LEFT;
     else
       if (key[config[current_config].joy_mapping[2][J_RIGHT]])
		   tmp|=JOY_RIGHT;

     if (key[config[current_config].joy_mapping[2][J_II]])
		 if (!auto_fire_A[2])
		   tmp|=JOY_A;
       else
			{
 			  already_fired_A[2]=!already_fired_A[2];
			  if (!already_fired_A[2])
				 tmp|=JOY_A;
			 }

     if (key[config[current_config].joy_mapping[2][J_I]])
		 if (!auto_fire_B[2])
		   tmp|=JOY_B;
       else
			{
 			  already_fired_B[2]=!already_fired_B[2];
			  if (!already_fired_B[2])
				 tmp|=JOY_B;
			 }

     if (key[config[current_config].joy_mapping[2][J_SELECT]])
		 tmp|=JOY_SELECT;

     if (key[config[current_config].joy_mapping[2][J_START]])
		 tmp|=JOY_START;

     if (key[config[current_config].joy_mapping[2][J_AUTOI]])
		 if (!key_delay)
			{
		     auto_fire_A[2]=!auto_fire_A[2];
			  key_delay=10;
         }

     if (key[config[current_config].joy_mapping[2][J_AUTOII]])
		 if (!key_delay)
			{
		     auto_fire_B[2]=!auto_fire_B[2];
			  key_delay=10;
         }

 return tmp;
 }

UInt16 keyboard4()
{
 UInt16 tmp=0;

	  if (key[config[current_config].joy_mapping[3][J_DOWN]])
		 tmp|=JOY_DOWN;
     else
	    if (key[config[current_config].joy_mapping[3][J_UP]])
		   tmp|=JOY_UP;

     if (key[config[current_config].joy_mapping[3][J_LEFT]])
		 tmp|=JOY_LEFT;
     else
       if (key[config[current_config].joy_mapping[3][J_RIGHT]])
		   tmp|=JOY_RIGHT;

     if (key[config[current_config].joy_mapping[3][J_II]])
		 if (!auto_fire_A[3])
		   tmp|=JOY_A;
       else
			{
 			  already_fired_A[3]=!already_fired_A[3];
			  if (!already_fired_A[3])
				 tmp|=JOY_A;
			 }

     if (key[config[current_config].joy_mapping[3][J_I]])
		 if (!auto_fire_B[3])
		   tmp|=JOY_B;
       else
			{
 			  already_fired_B[3]=!already_fired_B[3];
			  if (!already_fired_B[3])
				 tmp|=JOY_B;
			 }

     if (key[config[current_config].joy_mapping[3][J_SELECT]])
		 tmp|=JOY_SELECT;

     if (key[config[current_config].joy_mapping[3][J_START]])
		 tmp|=JOY_START;

     if (key[config[current_config].joy_mapping[3][J_AUTOI]])
		 if (!key_delay)
			{
		     auto_fire_A[3]=!auto_fire_A[3];
			  key_delay=10;
         }

     if (key[config[current_config].joy_mapping[3][J_AUTOII]])
		 if (!key_delay)
			{
		     auto_fire_B[3]=!auto_fire_B[3];
			  key_delay=10;
         }

 return tmp;
 }

UInt16 keyboard5()
{
 UInt16 tmp=0;

	  if (key[config[current_config].joy_mapping[4][J_DOWN]])
		 tmp|=JOY_DOWN;
     else
	    if (key[config[current_config].joy_mapping[4][J_UP]])
		   tmp|=JOY_UP;

     if (key[config[current_config].joy_mapping[4][J_LEFT]])
		 tmp|=JOY_LEFT;
     else
       if (key[config[current_config].joy_mapping[4][J_RIGHT]])
		   tmp|=JOY_RIGHT;

     if (key[config[current_config].joy_mapping[4][J_II]])
		 if (!auto_fire_A[4])
		   tmp|=JOY_A;
       else
			{
 			  already_fired_A[4]=!already_fired_A[4];
			  if (!already_fired_A[4])
				 tmp|=JOY_A;
			 }

     if (key[config[current_config].joy_mapping[4][J_I]])
		 if (!auto_fire_B[4])
		   tmp|=JOY_B;
       else
			{
 			  already_fired_B[4]=!already_fired_B[4];
			  if (!already_fired_B[4])
				 tmp|=JOY_B;
			 }

     if (key[config[current_config].joy_mapping[4][J_SELECT]])
		 tmp|=JOY_SELECT;

     if (key[config[current_config].joy_mapping[4][J_START]])
		 tmp|=JOY_START;

     if (key[config[current_config].joy_mapping[4][J_AUTOI]])
		 if (!key_delay)
			{
		     auto_fire_A[4]=!auto_fire_A[4];
			  key_delay=10;
         }

     if (key[config[current_config].joy_mapping[4][J_AUTOII]])
		 if (!key_delay)
			{
		     auto_fire_B[4]=!auto_fire_B[4];
			  key_delay=10;
         }

 return tmp;
 }

/* for joypad */


poll_joypad()
 {
  
    while (read ((int)fd[0], &e, sizeof(e)) > 0) {
      if (e.type & JS_EVENT_BUTTON)
        joypad[0].button[e.number] = e.value;
      else if (e.type & JS_EVENT_AXIS)
        joypad[0].axis[e.number] = e.value;
    }
  }

UInt16 joypad1()
 {
  UInt16 tmp;

  poll_joypad();

  tmp=0;

    if (joypad[0].axis[0] == -32767)
                tmp|=JOY_LEFT;

    if (joypad[0].axis[0] == 32767)
		tmp|=JOY_RIGHT;

    if (joypad[0].axis[1] == -32767)
		tmp|=JOY_UP;

    if (joypad[0].axis[1] == 32767)
		tmp|=JOY_DOWN;

    if (joypad[0].button[0])
		tmp|=JOY_A;

    if (joypad[0].button[1])
		tmp|=JOY_B;

	if (joypad[0].button[8])
		tmp|=JOY_START;

	if (joypad[0].button[10])
		tmp|=JOY_SELECT;
	if (joypad[0].button[6])
		auto_fire_A[0]=1;
	else auto_fire_A[0]=0;
	
	if (joypad[0].button[7])
		auto_fire_B[0]=1;
	else auto_fire_B[0]=0;

	/*
	 if (key[config[current_config].joy_mapping[0][J_START]])
		tmp=JOY_START;

    if (key[config[current_config].joy_mapping[0][J_SELECT]])
		tmp=JOY_SELECT;
	*/

  return tmp;
  }

UInt16 joypad2()
 {
  UInt16 tmp;

  poll_joypad();

  tmp=0;

    if (joypad[0].axis[0] == -32767)
        tmp|=JOY_LEFT;
 
    if (joypad[0].axis[0] == 32767)
        tmp|=JOY_RIGHT;

    if (joypad[0].axis[1] == -32767)
        tmp|=JOY_UP;
 
    if (joypad[0].axis[1] == 32767)
        tmp|=JOY_DOWN;
 
    if (joypad[0].button[0])
        tmp|=JOY_A;

    if (joypad[0].button[1])
        tmp|=JOY_B;

    if (joypad[0].button[8])
        tmp|=JOY_START;

    if (joypad[0].button[9])
        tmp|=JOY_SELECT;

  return tmp;
  }

UInt16 joypad3()
 {
    UInt16 tmp;

  poll_joystick();

  tmp=0;

	 if (joy[2].stick[0].axis[0].d1)
		tmp|=JOY_LEFT;

    if (joy[2].stick[0].axis[0].d2)
		tmp|=JOY_RIGHT;

	 if (joy[2].stick[0].axis[1].d1)
		tmp|=JOY_UP;

    if (joy[2].stick[0].axis[1].d2)
		tmp|=JOY_DOWN;

    if (joy[2].button[0].b)
		tmp|=JOY_A;

    if (joy[2].button[1].b)
		tmp|=JOY_B;

	 if (key[config[current_config].joy_mapping[2][J_START]])
		tmp|=JOY_START;

    if (key[config[current_config].joy_mapping[2][J_SELECT]])
		tmp|=JOY_SELECT;

  return tmp;

  }

UInt16 joypad4()
 {
    UInt16 tmp;

  poll_joystick();

  tmp=0;

	 if (joy[3].stick[0].axis[0].d1)
		tmp|=JOY_LEFT;

    if (joy[3].stick[0].axis[0].d2)
		tmp|=JOY_RIGHT;

	 if (joy[3].stick[0].axis[1].d1)
		tmp|=JOY_UP;

    if (joy[3].stick[0].axis[1].d2)
		tmp|=JOY_DOWN;

    if (joy[3].button[0].b)
		tmp|=JOY_A;

    if (joy[3].button[1].b)
		tmp|=JOY_B;

	 if (key[config[current_config].joy_mapping[3][J_START]])
		tmp|=JOY_START;

    if (key[config[current_config].joy_mapping[3][J_SELECT]])
		tmp|=JOY_SELECT;

  return tmp;
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

 while (key[KEY_PAUSE]) pause();

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
