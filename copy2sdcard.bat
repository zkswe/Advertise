@echo off
cd /d %~dp0
xcopy sdcard G:  /y /e /h /v /d
pause 