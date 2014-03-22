echo Creation of technical documentation...

del /q /s Docs\TechnicalDoc\*

cd Install\TempExcl

doxygen

cd ..\..

qhelpgenerator .\Docs\TechnicalDoc\index.qhp

echo Technical documentation created
