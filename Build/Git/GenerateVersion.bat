@echo off
cd /d "%~dp0"
call UpdateVersion.bat Config\QMake\AcfVersion.pri.xtrsvn "%~1"
call UpdateVersion.bat Partitura\AcfVoce.arp\VersionInfo.acc.xtrsvn "%~1"
call UpdateVersion.bat Include\istd\AcfVersion.h.xtrsvn "%~1"