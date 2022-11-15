copy ..\OBJ\T5L51.hex ..\Download
cd ..\Download
srec_cat.exe T5L51.hex -Intel -o T5L51.bin -Binary 
copy T5L51.bin ..\..\HAC02M3ToolingUI\DWIN_SET
echo.