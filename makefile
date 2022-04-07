MACHINE = linux
DEBUG_DIR = debug

INCLUDE = -I. -I$(DEBUG_DIR) -I$(MACHINE) -Igfx -Ikernel~1 -Isound -Imisc -Igui

OBJ = pce.o gfx/sprite.o kernel~1/h6280.o $(DEBUG_DIR)/view_zp.o $(DEBUG_DIR)/edit_ram.o \
$(DEBUG_DIR)/debug.o $(DEBUG_DIR)/format.o \
gfx/subs_eagle.o misc/cd.o misc/hcd.o misc/lsmp3.o sound/sound.o sound/mix.o \
$(DEBUG_DIR)/followop.o $(DEBUG_DIR)/dis.o $(DEBUG_DIR)/optable.o \
$(DEBUG_DIR)/view_inf.o misc/lang.o misc/list_rom.o misc/config.o gfx/trans_fx.o misc/cheat.o gui/gui.o \
gui/interf.o $(MACHINE)/osd_cd.o $(MACHINE)/osd_keyboard.o \
$(MACHINE)/osd_machine.o $(MACHINE)/osd_gfx.o gfx/gfx.o kernel~1/bp.o

DEF = -DLINUX -DALLEGRO -fomit-frame-pointer -O6 -DFINAL_RELEASE -DEXTERNAL_DAT -DKERNEL_DS

CC := egcs
RM := rm
CP := cp

ARCHIVE = tar cvfz
ARCHIVE_EXT = tgz

hugo : $(OBJ)
	#$(CC) -o hugo $(OBJ) $(DEF) -L/usr/lib -lalleg -lamp
	$(CC) -o hugo $(OBJ) $(DEF) `allegro-config --libs --static` -lamp

%.o : %.c %.h
	$(CC) $(INCLUDE) -o $@ -c $< $(DEF)

.PHONY : clean

clean :
	rm $(OBJ)

kernel/m6502.o : kernel/m6502.c kernel/m6502.h kernel/cdemu.h

RDIR := release_linux

mk_release : hugo
	mkdir $(RDIR)
	upx -9 hugo
	$(CP) file_id.diz $(RDIR)
	$(CP) hu-go!.ini $(RDIR)
	$(CP) hu-go!.txt $(RDIR)/hugo.txt
	$(CP) hu-go!.fr $(RDIR)/hugo.fr
	$(CP) hu-go!.es $(RDIR)/hugo.es
	$(CP) changes $(RDIR)
	$(CP) changemt.fr $(RDIR)
	$(CP) scancode.txt $(RDIR)
	$(CP) hugo $(RDIR)
	$(CP) pong.pce $(RDIR)
	$(CP) faq $(RDIR)
	$(CP) dracx.hcd $(RDIR)
	$(CP) hu-go!.dat $(RDIR)
	$(CP) install.sh $(RDIR)
	$(CP) fagemul.bmp $(RDIR)
	$(CP) cheat.txt $(RDIR)
	$(ARCHIVE) $(RDIR)/release.$(ARCHIVE_EXT) $(RDIR)/*
