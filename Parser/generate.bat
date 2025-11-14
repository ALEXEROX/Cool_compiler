
SET BISON=C:/Games/GnuWin32/bin/
SET CURDIR=%CD%
SET "FILENAME=Cool_code"

COPY "%FILENAME%.y" "%BISON%/%FILENAME%.y"

cd %BISON%
bison.exe -d "%FILENAME%.y" --locations

MOVE "%FILENAME%.tab.c" "%CURDIR%/%FILENAME%.tab.c"
MOVE "%FILENAME%.tab.h" "%CURDIR%/%FILENAME%.tab.h"