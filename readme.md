# Tetris

<img src="https://github.com/user-attachments/assets/681a9f5d-9dfb-4666-adec-fbef31b95a1b" width=30% height=30% />

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
