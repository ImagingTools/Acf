@echo off
setlocal enabledelayedexpansion

REM --- Проверка параметра ---
if "%~1"=="" (
    echo Usage: %~nx0 path\to\template.xtrsvn
    exit /b 1
)

set "FILE=%~1"

if not exist "%FILE%" (
    echo File "%FILE%" does not exist.
    exit /b 1
)

REM --- Получаем ревизию ---
git fetch --prune --unshallow 2>nul

for /f "usebackq delims=" %%i in (`git rev-list --count origin/master 2^>nul`) do set REV=%%i
if not defined REV (
    for /f "usebackq delims=" %%i in (`git rev-list --count HEAD 2^>nul`) do set REV=%%i
)
if not defined REV (
    echo Failed to compute revision count.
    exit /b 1
)

REM --- Проверка грязного состояния ---
git diff-index --quiet HEAD --
if %errorlevel%==0 (
    set DIRTY=0
) else (
    set DIRTY=1
)

echo Git revision: %REV%, dirty: %DIRTY%
echo Processing file: %FILE%

REM --- Выходной файл (убираем .xtrsvn) ---
set "OUT=%FILE:.xtrsvn=%"

(for /f "usebackq delims=" %%L in ("%FILE%") do (
    set "line=%%L"
    set "line=!line:$WCREV$=%REV%!"
    set "line=!line:$WCMODS?1:0$=%DIRTY%!"
    echo(!line!
)) > "%OUT%"

echo Wrote %OUT% with WCREV=%REV% and WCMODS=%DIRTY%
endlocal