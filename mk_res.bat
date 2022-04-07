@echo off

echo creating directory

md %1


echo copying files

copy doc\file_id.diz %1

REM copy hu-go!.dat %1

copy hu-go!.ini %1
copy doc\hu-go!.txt %1
copy doc\hu-go!.fr %1
copy doc\hu-go!.es %1
copy doc\changes %1
copy doc\changemt.fr %1
copy doc\scancode.txt %1
copy hu-go!.exe %1
copy cwsdpmi.exe %1
copy extra\pce.ico %1
copy extra\hu-go!.pif %1
copy extra\hu-go!.ico %1
copy extra\pong.pce %1
copy doc\faq %1
copy extra\dracx.hcd %1
copy matrix.bmp %1
copy doc\cheat.txt %1

cd %1


echo compacting EXE

upx -9 hu-go!.exe


echo trying different compression

pkzip %temp%\HU-GO!.ZIP *.* -ex

REM acb u HU-GO!.ACB *.*

REM 777 a -mg -s HU-GO!.777 *.*

move %temp%\HU-GO!.ZIP .

DIR /OS
