rm ../AutomaticComponentToolkit/act.win32.exe 
rm ../AutomaticComponentToolkit/act.win64.exe
rm ../AutomaticComponentToolkit/act.linux32 
rm ../AutomaticComponentToolkit/act.linux64 
rm ../AutomaticComponentToolkit/act.linux.arm 
rm ../AutomaticComponentToolkit/act.linux64.arm 
rm ../AutomaticComponentToolkit/act.darwin

cd AutomaticComponentToolkit
Build/build.sh

cp act.win32.exe ../../AutomaticComponentToolkit/
cp act.win64.exe ../../AutomaticComponentToolkit/
cp act.linux32 ../../AutomaticComponentToolkit/
cp act.linux64 ../../AutomaticComponentToolkit/
cp act.linux32.arm ../../AutomaticComponentToolkit/
cp act.linux64.arm ../../AutomaticComponentToolkit/
cp act.darwin ../../AutomaticComponentToolkit/

git rev-parse --verify HEAD >"../../AutomaticComponentToolkit/_githash_act.txt"

cd ..

