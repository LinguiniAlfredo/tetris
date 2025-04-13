# Tetris

## Installation
You'll need SDL2 libraries to compile:
```bash
sudo apt install libsdl2-dev
sudo apt install libsdl2-image-dev
sudo apt install libsdl2-ttf-dev
```

Then simply run:
```bash
make clean # cleans root directory of build files
make       # builds tetris executable

# optional
make debug # runs with valgrind enabled (requires valgrind)
```
