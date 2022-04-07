@echo off

echo directory creation

md %1
md %1\machine
md %1\machine\pc
md %1\machine\linux
md %1\dos
md %1\linux
md %1\debug
md %1\win

echo copying sources files

copy pce.c %1
copy pce.h %1
copy cd.c %1
copy cd.h %1
copy sound.c %1
copy sound.h %1
copy m6502.c %1
copy m6502.h %1
copy codes.h %1
copy gui.c %1
copy gui.h %1
copy gfx.c %1
copy gfx.h %1
copy crc_ctl.h %1
copy list_rom.c %1
copy list_rom.h %1
copy hutables.h %1
copy hucodes.h %1
copy trans_fx.h %1
copy trans_fx.c %1
copy mix.h %1
copy mix.c %1
copy cheat.h %1
copy cheat.c %1
copy sprite.h %1
copy sprite.c %1
copy config.h %1
copy config.c %1
copy debug\dis.c %1\debug
copy debug\dis.h %1\debug
copy debug\format.c %1\debug
copy debug\format.h %1\debug
copy debug\debug.c %1\debug
copy debug\debug.h %1\debug

rem copy keyboard.c %1
rem copy keyboard.h %1

copy bp.h %1
copy debug\edit_ram.c %1\debug
copy debug\edit_ram.h %1\debug
copy info_dat.h %1
copy lang.h %1
copy lang.c %1
copy cleantyp.h %1
copy libunzip.h %1
copy lsmp3.c %1
copy lsmp3.h %1
copy interf.c %1
copy interf.h %1
copy debug\followop.h %1\debug
copy debug\followop.c %1\debug
copy debug\optable.h %1\debug
copy debug\optable.c %1\debug
copy cdemu.h %1
copy debug\view_zp.h %1\debug
copy debug\view_zp.c %1\debug
copy debug\view_inf.c %1\debug
copy debug\view_inf.h %1\debug
copy iso_ent.h %1
copy data.h %1
copy specific_interface.c %1
copy specific_interface.h %1
copy lang_interf.h %1
rem copy data.s %1

copy sys_dep.h %1
copy sys_cd.h %1
copy sys_gfx.h %1
copy sys_inp.h %1
copy sys_misc.h %1

copy dos\osd_cd.c %1\dos
copy dos\osd_cd.h %1\dos
copy dos\osd_keyboard.c %1\dos
copy dos\osd_keyboard.h %1\dos
copy dos\aspi.h %1\dos
copy dos\aspi_int.h %1\dos
copy dos\scsidefs.h %1\dos
copy dos\osd_gfx.c %1\dos
copy dos\osd_gfx.h %1\dos
copy dos\osd_machine.h %1\dos
copy dos\osd_machine.c %1\dos

copy win\osd_cd.c %1\win
copy win\osd_cd.h %1\win
copy win\osd_keyboard.c %1\win
copy win\osd_keyboard.h %1\win
copy win\aspi.h %1\win
copy win\aspi_int.h %1\win
copy win\scsidefs.h %1\win
copy win\osd_gfx.c %1\win
copy win\osd_gfx.h %1\win
copy win\osd_machine.h %1\win
copy win\osd_machine.c %1\win

copy linux\osd_cd.c %1\linux
copy linux\osd_cd.h %1\linux
copy linux\osd_keyboard.c %1\linux
copy linux\osd_keyboard.h %1\linux
copy linux\osd_gfx.c %1\linux
copy linux\osd_gfx.h %1\linux
copy linux\osd_machine.h %1\linux
copy linux\osd_machine.c %1\linux

copy dos\cleantyp.h %1\dos
copy win\cleantyp.h %1\win
copy linux\cleantyp.h %1\linux

copy liballeg.a %1
copy hcd.c %1
copy hcd.h %1
copy eagle.h %1
copy ..\eagle.o %1
copy eagle.txt %1
copy ..\eagle.asm %1

copy makefile %1
copy makefile.dos %1
copy makedos.bat %1
copy makefile.mgw %1
copy makewin.bat %1
copy install.sh %1
copy mk_res.bat %1
copy mk_src.bat %1
copy install_src.bat %1

copy dos\libamp.a %1\dos
copy dos\libamp.h %1\dos
copy linux\libamp.a %1\linux
copy linux\libamp.h %1\linux

copy hugo_dos.gdt %1
copy hugo_dos.gpr %1
copy hugo_win.gdt %1
copy hugo_win.gpr %1

echo copying data files

copy file_id.diz %1
copy hu-go!.dat %1
copy hu-go!.ini %1
copy compile.doc %1
copy hu-go!.txt %1
copy hu-go!.fr %1
copy changes %1
copy changemt.fr %1
copy hu-go!.gdt %1
copy hu-go!.gpr %1
copy scancode.txt %1
copy pce.ico %1
copy hu-go!.pif %1
copy hu-go!.ico %1
copy faq %1

cd %1

echo entering compression mode

rem pkzip %temp%\SOURCES.ZIP *.* -ex
rem acb u %temp%\SOURCES.ACB *.*
rem 777 a -mg -s SOURCES.777 *.*
rem move %temp%\SOURCES.ZIP .
rem move %temp%\SOURCES.ACB .

echo displaying results

DIR /OS

