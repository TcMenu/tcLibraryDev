@echo off
set IDF_PATH=C:\Espressif\frameworks\esp-idf-v5.5.1

echo Starting CLion ESP-IDF env script > C:\Temp\clion-esp-idf-env.txt
echo Script path: %~f0 >> C:\Temp\clion-esp-idf-env.txt
echo Current dir: %CD% >> C:\Temp\clion-esp-idf-env.txt
echo. >> C:\Temp\clion-esp-idf-env.txt

echo Calling ESP-IDF export.bat... >> C:\Temp\clion-esp-idf-env.txt
call call C:\Espressif\idf_cmd_init.bat >> C:\Temp\clion-esp-idf-env.txt 2>&1
echo export.bat errorlevel=%ERRORLEVEL% >> C:\Temp\clion-esp-idf-env.txt
echo. >> C:\Temp\clion-esp-idf-env.txt

echo IDF_PATH=%IDF_PATH% >> C:\Temp\clion-esp-idf-env.txt
echo IDF_TOOLS_PATH=%IDF_TOOLS_PATH% >> C:\Temp\clion-esp-idf-env.txt
echo PATH=%PATH% >> C:\Temp\clion-esp-idf-env.txt
echo. >> C:\Temp\clion-esp-idf-env.txt

echo where idf.py: >> C:\Temp\clion-esp-idf-env.txt
where idf.py >> C:\Temp\clion-esp-idf-env.txt 2>&1

echo where python: >> C:\Temp\clion-esp-idf-env.txt
where python >> C:\Temp\clion-esp-idf-env.txt 2>&1

echo where cmake: >> C:\Temp\clion-esp-idf-env.txt
where cmake >> C:\Temp\clion-esp-idf-env.txt 2>&1

echo where ninja: >> C:\Temp\clion-esp-idf-env.txt
where ninja >> C:\Temp\clion-esp-idf-env.txt 2>&1

echo where xtensa-esp32s2-elf-gcc: >> C:\Temp\clion-esp-idf-env.txt
where xtensa-esp32s2-elf-gcc >> C:\Temp\clion-esp-idf-env.txt 2>&1