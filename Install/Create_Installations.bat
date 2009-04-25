echo off

cd ../Docs/Doxygen

call createDoc.bat

pause

cd ../../Install

call Create_VC_LGPL.bat

pause

