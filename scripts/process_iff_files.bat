@echo off
setlocal enabledelayedexpansion

echo Processing IFF files...

REM Path to main.exe
set MAIN_EXE="..\main.exe"

for %%f in (*.IFF) do (
    echo Processing %%f...
    %MAIN_EXE% "%%f" "%%f.strings.txt"
    if errorlevel 1 (
        echo Error processing %%f
    ) else (
        echo Successfully processed %%f
    )
)

echo Done processing all IFF files.
pause
