
SET FLEX=C:/Games/GnuWin32/bin/
SET CURDIR=%CD%
SET "FILENAME=Cool"

COPY "%FILENAME%.y" "%FLEX%/%FILENAME%.y"

cd %FLEX%
flex.exe -d "%FILENAME%.l"

MOVE "lex.yy.c" "%CURDIR%/lex.yy.c"
MOVE "lex.yy.c" "%CURDIR%/lex.yy.c"