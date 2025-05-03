# Tetris

![tetris2](https://github.com/user-attachments/assets/354eaddd-030c-460d-9d19-c21a482b7482)

## Installation (Linux)
You'll need SDL2 libraries to compile:
```bash
sudo apt install libsdl2-dev
sudo apt install libsdl2-image-dev
sudo apt install libsdl2-ttf-dev
```

Then just run:
```bash
make clean # cleans root directory of build files
make       # builds tetris executable

# optional
make debug # runs with valgrind enabled (requires valgrind)
```
