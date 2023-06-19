Compilar
```bash
clang++ *.cpp -o main -Wall -Wextra -Wpedantic -O0 -g3 $(pkg-config --cflags --libs sdl2) -lSDL2_ttf
```

![](https://github.com/JGSVb/Cubo-bonito/blob/main/video.gif)
