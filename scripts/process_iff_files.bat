@echo off
setlocal enabledelayedexpansion

echo ===============================================
echo   Floigan Bros Dialogue Extractor
echo ===============================================
echo.

REM Check if there are any IFF files
set IFF_COUNT=0
for %%f in (*.IFF) do (
    set /a IFF_COUNT+=1
)

if %IFF_COUNT%==0 (
    echo ERROR: No IFF files found in this directory!
    echo Please place your .IFF files in the scripts folder.
    pause
    exit /b 1
)

echo Found %IFF_COUNT% IFF files in this directory.
echo.
echo Starting extraction process...
echo.

REM Run the complete pipeline using process_all_iff.py
python process_all_iff.py .
if errorlevel 1 (
    echo ERROR: Extraction failed!
    pause
    exit /b 1
)

echo.
echo ===============================================
echo   EXTRACTION COMPLETE!
echo ===============================================
echo.
echo Your clean dialogue has been saved to: floigan_bros_all_dialogues.txt
echo.
echo Cleaning up temporary files...
del all_files.strings.txt 2>nul
del all_files.strings.txt.data.txt 2>nul

echo.
echo Done! You can now open floigan_bros_all_dialogues.txt to see the extracted dialogue.
pause
