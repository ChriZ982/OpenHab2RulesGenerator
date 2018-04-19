CC := clang++
EXE := /linux/OH2RulesGenerator-linux-
FLAGS := -std=c++14 -Wall -Wextra
SOURCE := OpenHab2RulesGenerator/src/*.cpp

all:
	mkdir -p bin/Release/linux
	mkdir -p bin/Debug/linux
	$(CC) $(FLAGS) -target x86_64-linux $(SOURCE) -o bin/Debug$(EXE)Debug-x64
	$(CC) $(FLAGS) -target x86_64-linux -Ofast $(SOURCE) -o bin/Release$(EXE)Release-x64
	$(CC) $(FLAGS) -target i386-linux $(SOURCE) -o bin/Debug$(EXE)Debug-x86
	$(CC) $(FLAGS) -target i386-linux -Ofast $(SOURCE) -o bin/Release$(EXE)Release-x86

clean:
	rm -f bin/Debug$(EXE)Debug-x64
	rm -f bin/Release$(EXE)Release-x64
	rm -f bin/Debug$(EXE)Debug-x86
	rm -f bin/Release$(EXE)Release-x86
