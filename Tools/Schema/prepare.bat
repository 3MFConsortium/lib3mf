ECHO OFF
for %%I in (*.xsd) do (
	SETLOCAL DISABLEDELAYEDEXPANSION
	IF EXIST test\%%~nxI (
		DEL test\%%~nxI
	)
	FOR /F "tokens=* delims=" %%L IN (%%~nxI) DO (
	   SET "line=%%L"
	   SETLOCAL ENABLEDELAYEDEXPANSION
	   ECHO(!line:"2147483647"="1073741824"!>>test\%%~nxI
	   ENDLOCAL
	)
	ENDLOCAL
)
