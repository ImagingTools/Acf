@echo off
setlocal enabledelayedexpansion

REM --- Validate input parameter ---
if "%~1"=="" (
    echo Usage: %~nx0 path\to\template.xtrsvn
    exit /b 1
)

set "FILE=%~1"

if not exist "%FILE%" (
    echo File "%FILE%" does not exist.
    exit /b 1
)

REM --- Compute revision ---
git fetch --prune --unshallow 2>nul

for /f "usebackq delims=" %%i in (`git rev-list --count origin/main 2^>nul`) do set REV=%%i
if not defined REV (
    for /f "usebackq delims=" %%i in (`git rev-list --count HEAD 2^>nul`) do set REV=%%i
)
if not defined REV (
    echo Failed to compute revision count.
    exit /b 1
)

REM --- Check dirty working tree state ---
git diff-index --quiet HEAD --
if %errorlevel%==0 (
    set DIRTY=0
) else (
    set DIRTY=1
)

echo Git revision: %REV%, dirty: %DIRTY%
echo Processing file: %FILE%

REM --- Output file path (strip .xtrsvn) ---
set "OUT=%FILE:.xtrsvn=%"
set "TMP=%OUT%.tmp"

(for /f "usebackq delims=" %%L in ("%FILE%") do (
    set "line=%%L"
    set "line=!line:$WCREV$=%REV%!"
    set "line=!line:$WCMODS?1:0$=%DIRTY%!"
    echo(!line!
)) > "%TMP%"

if exist "%OUT%" (
    fc /b "%TMP%" "%OUT%" >nul
    if errorlevel 1 (
        move /y "%TMP%" "%OUT%" >nul
        echo Wrote %OUT% with WCREV=%REV% and WCMODS=%DIRTY%
    ) else (
        del "%TMP%" >nul 2>&1
        echo No changes in %OUT%, file not rewritten
    )
) else (
    move /y "%TMP%" "%OUT%" >nul
    echo Wrote %OUT% with WCREV=%REV% and WCMODS=%DIRTY%
)
endlocal
exit /b 0