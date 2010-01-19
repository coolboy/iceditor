\REM Argument 1 is type of build (release, or debug)
CLS
@ECHO OFF

set COMPILE_TYPE=release
REM Check for user input of compile type
IF x==%1x GOTO GET_USER_INPUT

REM compile type on command line 
set COMPILE_TYPE=%1
GOTO PROCESS

:GET_USER_INPUT
REM prompt user for compile type
echo Default build type is release
echo Select a build type (release or debug)
SET /p COMPILE_TYPE=

IF /I %COMPILE_TYPE% == release GOTO RELEASE
IF /I %COMPILE_TYPE% == debug GOTO DEBUG
IF /I x==%COMPILE_TYPE%x GOTO RELEASE

ECHO Invalid compile type
set COMPILE_TYPE=release
GOTO GET_USER_INPUT

:RELEASE
ECHO YOU HAVE SELECTED RELEASE
SET COMPILE_PAR=jar
rem echo %COMPILE_PAR%
GOTO PROCESS

:DEBUG
ECHO YOU HAVE SELECTED DEBUG
SET COMPILE_PAR=jar-dev
rem echo %COMPILE_PAR%
GOTO PROCESS



:PROCESS
echo in process
echo on
ant -v -f .\build.xml %COMPILE_PAR% > .\build.log
rem ant -v -f .\build.xml %COMPILE_PAR% 
GOTO END

:END
EXIT