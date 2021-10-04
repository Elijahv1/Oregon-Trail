# Oregon-Trail

This is a project written in C that is similar to the Oregon Trail game. The user must start from the X character and traverse to the Z character with only a limited amout of food. The files are compiled using the GNU compiler, the commands to compile are listed below where the executable main.exe will be created.

Compile Steps:
g++ -c map.c main.c
g++ -o main.exe map.o main.o

To run with a map file use:
./main.exe test.txt
