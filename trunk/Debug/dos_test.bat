@echo off
set n=0
:abc
set /a n+=1
SpectralClusteringGndVSkyOutDoor -NotSave -NotDebugInfo >>DebugInfo.txt
if %n%==10 exit
goto abc


