del /f/s/q .\vending.exe
cl /Fevending.exe /source-charset:utf-8 /execution-charset:utf-8 .\src\*.c
del .\*.obj
.\vending.exe