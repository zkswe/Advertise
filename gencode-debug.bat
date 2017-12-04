@echo off
cd /d %~dp0
rmdir sdcard
mkdir sdcard
cd sdcard
mkdir ui
mkdir lib
cd ..
xcopy resources sdcard\ui  /y /e /h /v /d
copy build\*.ftu sdcard\ui\
copy build\EasyUIdebug.cfg sdcard\EasyUI.cfg
copy libs\armeabi\libzkgui.so sdcard\lib\libzkgui.so
pause 