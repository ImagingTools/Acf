@echo off
setlocal

rem %1 = start directory (optional, defaults to current dir)
set "startDir=%~1"
if "%startDir%"=="" set "startDir=%CD%"

rem %2 = optional format (keeps your original idea: -o -,<format>)
set "format="
if not "%~2"=="" set "format=-o -,%~2"

for /r "%startDir%" %%i in (*Test.exe) do (
  echo Running: "%%i" %format%
  "%%i" %format%
  if errorlevel 1 exit /b %errorlevel%
)

exit /b 0

