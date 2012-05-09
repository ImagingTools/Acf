echo Update ACfPublic repository
svn update C:\Temp\AcfPublic --username acf --password acf --non-interactive --trust-server-cert --no-auth-cache
echo Update of ACfPublic repository finished

echo Remove old ACfPublic files
cd C:\Temp\AcfPublic\Acf
del /s /q /a-h *
cd C:\Temp\AcfPublic\AcfSln
del /s /q /a-h *
cd C:\Temp\AcfPublic\Iacf
del /s /q /a-h *

echo Create installation files

echo Create Acf
cd C:\Temp\Acf\Install
call _Create_VC9_OpenSource.bat
xcopy Temp C:\Temp\AcfPublic\Acf /E /Y

echo Create Acf-Solutions
cd C:\Temp\AcfSln\Install
call _Create_VC9_OpenSource.bat
xcopy Temp C:\Temp\AcfPublic\AcfSln /E /Y

echo Create IACF
cd C:\Temp\Iacf\Install
call _Create_VC9_OpenSource.bat
xcopy Temp C:\Temp\AcfPublic\Iacf /E /Y

cd C:\Temp\AcfPublic

echo Delete removed files
for /f "usebackq tokens=2*" %%i in (`svn status ^| findstr /r "^\!"`) do (

    svn delete "%%i %%j"

)


echo Add new files
for /f "usebackq tokens=2*" %%i in (`svn status ^| findstr /r "^\?"`) do (

    svn add "%%i %%j"

)



echo Commit changes to the repository
svn commit C:\Temp\AcfPublic --username acf --password acf --non-interactive --trust-server-cert --no-auth-cache -F C:\Temp\Iacf\Install\TempExcl\Commit.log

