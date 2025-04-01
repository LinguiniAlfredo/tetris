OBJS = main.cpp ./build/timer.o ./build/texture.o ./build/hud.o ./build/tetromino.o ./build/board.o ./build/vec2.o ./build/collision.o

CC = g++
OPTIONS = -Wall -g
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf 
EXE = main

all : $(OBJS)
	$(CC) $(OPTIONS) $(OBJS) $(LINKER_FLAGS) -o $(EXE)

build/texture.o : components/texture.cpp
	$(CC) components/texture.cpp -c -w $(LINKER_FLAGS) -o ./build/texture.o

build/collision.o : components/collision.cpp
	$(CC) components/collision.cpp -c -w $(LINKER_FLAGS) -o ./build/collision.o

build/timer.o : utils/timer.cpp
	$(CC) utils/timer.cpp -c -w $(LINKER_FLAGS) -o ./build/timer.o

build/vec2.o : utils/vec2.cpp
	$(CC) utils/vec2.cpp -c -w $(LINKER_FLAGS) -o ./build/vec2.o

build/hud.o : ui/hud.cpp
	$(CC) ui/hud.cpp -c -w $(LINKER_FLAGS) -o ./build/hud.o
	
build/tetromino.o : entities/tetromino.cpp
	$(CC) entities/tetromino.cpp -c -w $(LINKER_FLAGS) -o ./build/tetromino.o

build/board.o : board.cpp
	$(CC) board.cpp -c -w $(LINKER_FLAGS) -o ./build/board.o

clean :
	rm -r ./build/
	rm ./main

debug :
	valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=debug.txt \
		./main

$(shell mkdir -p ./build)
