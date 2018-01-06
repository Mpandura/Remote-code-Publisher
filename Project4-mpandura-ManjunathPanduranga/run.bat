cd Client_wpf\bin\Debug\
start Client1.exe 8080 8082
cd..
cd..
cd..
cd x64\Debug\
start Publisher_Online.exe ./files *.h *.cpp  /f /r
start TestExecutive_project4.exe
pause

