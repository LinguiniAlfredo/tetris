OBJS = main.cpp ./build/timer.o ./build/texture.o ./build/hud.o ./build/player.o

CC = g++
OPTIONS = -std=c++11 -Wall -g
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf 
EXE = main

all : $(OBJS)
	$(CC) $(OPTIONS) $(OBJS) $(LINKER_FLAGS) -o $(EXE)

build/texture.o : components/texture.cpp
	$(CC) components/texture.cpp -c -w $(LINKER_FLAGS) -o ./build/texture.o

build/timer.o : utils/timer.cpp
	$(CC) utils/timer.cpp -c -w $(LINKER_FLAGS) -o ./build/timer.o

build/hud.o : ui/hud.cpp
	$(CC) ui/hud.cpp -c -w $(LINKER_FLAGS) -o ./build/hud.o

build/player.o : entities/player.cpp
	$(CC) entities/player.cpp -c -w $(LINKER_FLAGS) -o ./build/player.o

clean : $(OBJS)
	rm -r ./build/
	rm ./main

$(shell mkdir ./build)