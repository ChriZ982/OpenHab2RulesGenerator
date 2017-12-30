CC := clang++
EXE := bin/linux/OpenHab2RulesGenerator
FLAGS := -std=c++14 -Wall -Wextra

all:
	$(CC) $(FLAGS) OpenHab2RulesGenerator/src/*.cpp -o $(EXE)

clean:
	rm -f $(EXE)