rem @echo off
set NSYS="C:\Program Files (x86)\NSIS\Unicode\makensis.exe"
set BUILD_OK=1
set PWD=%CD%
set MAKE=mingw32-make -j4

set NSYS_FILE=salon.nsi
set NSYS_PATH=%PWD%\%NSYS_FILE%
set SRC_DIR=%PWD%\..\src
set TMP_DIR=%PWD%\temp
set BUILD_DIR=%TMP_DIR%\release
set DEPLOY_DIR=%TMP_DIR%\deploy

rem создаем новые каталоги для сборки
rmdir /S /Q %TMP_DIR%
mkdir %TMP_DIR%
mkdir %BUILD_DIR%
mkdir %DEPLOY_DIR%

rem выдергиваем версии
set VER_MAJOR=0
set VER_MINOR=0
set VER_PATCH=0
set VER_BUILD=1

del tmp.file
set VERSION=%VER_MAJOR%.%VER_MINOR%.%VER_PATCH%-%VER_BUILD%
@echo on
	@echo Building version %VERSION%...
@echo off

rem прогоняем release сборку
cd %BUILD_DIR%
@echo on
	@echo Building RELEASE...
@echo off

rem первый прогон для установки переменных
cmd /C cmake -G "MinGW Makefiles"  %SRC_DIR%
cmd /C cmake -DCMAKE_BUILD_TYPE=Release -Dsalon_VERSION_MAJOR=%VER_MAJOR% -Dsalon_VERSION_MINOR=%VER_MINOR% -Dsalon_VERSION_PATCH=%VER_PATCH% -Dsalon_VERSION_BUILD=%VER_BUILD% %SRC_DIR%
IF not %ERRORLEVEL% ==0 goto cmake_error
@echo on
    @echo CMake RELEASE OK
	@echo Compile RELEASE...
@echo off

cmd /C "%MAKE% all"
IF not %ERRORLEVEL% ==0 goto build_error
@echo on
    @echo Compile RELEASE OK
@echo off

rem собираем файлы в дистрибутив
@echo on
	@echo Prepare files to deploy...
@echo off
	
mkdir %DEPLOY_DIR%
mkdir %DEPLOY_DIR%\pics
mkdir %DEPLOY_DIR%\sqldrivers

copy %BUILD_DIR%\salon.exe %DEPLOY_DIR%\salon.exe
copy %BUILD_DIR%\salon.ini %DEPLOY_DIR%\salon.ini
copy %BUILD_DIR%\birthday %DEPLOY_DIR%\birthday
copy %PWD%\..\pics\salon.ico %TMP_DIR%\
xcopy /s /y %PWD%\..\pics\*.png %DEPLOY_DIR%\pics\
xcopy /s /y %PWD%\..\pics\*.ico %DEPLOY_DIR%\pics\
xcopy /s /y %PWD%\..\pics\*.jpg %DEPLOY_DIR%\pics\
xcopy /s /y %PWD%\win_dist\sqldrivers\* %DEPLOY_DIR%\sqldrivers\
xcopy /s /y %PWD%\win_dist\*.dll %DEPLOY_DIR%\

rem сборка nsys
cd %TMP_DIR%

@echo on
	@echo Build package...
@echo off
copy %NSYS_PATH% .
%NSYS% /Dversion=%VERSION% %NSYS_FILE%
IF not %ERRORLEVEL% ==0 goto deploy_error

rem копирование собранного дистрибутива	
@echo on
	@echo Copy package...
@echo off
move *.exe %PWD%\
IF not %ERRORLEVEL% ==0 goto deploy_error
			
@echo on
	@echo Building version %VERSION% finish. Distributive in %PWD%
@echo off

goto build_ok

:cmake_error
	@echo on
		@echo Cmake FAILED
	@echo off
	cd %PWD%
 	#exit 1

:build_error
	@echo on
		@echo Compile FAILED
	@echo off
	cd %PWD%
	#exit 1

:deploy_error
	@echo on
		@echo %PWD% error
	@echo off
	cd %PWD%
 	#exit 1

:build_ok
	set BUILD_OK=0
	cd %PWD%
	rmdir /S /Q %TMP_DIR%
