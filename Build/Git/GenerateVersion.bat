@echo off
cd /d "%~dp0"
call UpdateVersion.bat ..\..\Config\QMake\AcfVersion.pri.xtrsvn
call UpdateVersion.bat ..\..\Partitura\AcfVoce.arp\VersionInfo.acc.xtrsvn
call UpdateVersion.bat ..\..\Include\istd\AcfVersion.h.xtrsvn