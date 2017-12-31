CC := clang++
EXE := OpenHab2RulesGenerator
FLAGS := -std=c++14 -Wall -Wextra

all:
	$(CC) $(FLAGS) OpenHab2RulesGenerator/src/*.cpp -o bin/linux/debug/$(EXE)
	$(CC) $(FLAGS) -O3 OpenHab2RulesGenerator/src/*.cpp -o bin/linux/release/$(EXE)

clean:
	rm -f bin/linux/Debug/$(EXE)
	rm -f bin/linux/Release/$(EXE)