OBJS = main.cpp ./build/timer.o ./build/texture.o ./build/hud.o

CC = g++
OPTIONS = -Wall
LINKER_FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image
EXE = main

all : $(OBJS)
	$(CC) $(OBJS) $(OPTIONS) $(LINKER_FLAGS) -o $(EXE)

build/texture.o : components/texture.cpp
	$(CC) components/texture.cpp -c -w -o ./build/texture.o

build/timer.o : utils/timer.cpp
	$(CC) utils/timer.cpp -c -w -o ./build/timer.o

build/hud.o : ui/hud.cpp
	$(CC) ui/hud.cpp -c -w -o ./build/hud.o

clean : $(OBJS)
	rm -r ./build/*.o

