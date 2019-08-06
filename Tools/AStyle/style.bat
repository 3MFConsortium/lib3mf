set STYLE=--recursive --style=mozilla --indent=tab=2 -n
AStyle.exe %STYLE% ..\Source\*.cpp
AStyle.exe %STYLE% ..\Include\*.hpp
AStyle.exe %STYLE% ..\Include\*.h
AStyle.exe %STYLE% ..\Tests\CPP_Bindings\*.cpp
AStyle.exe %STYLE% ..\Tests\CPP_Bindings\*.hpp
AStyle.exe %STYLE% ..\Tests\CPP_Bindings\*.h
