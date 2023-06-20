Estou com uns problemas de classes e essas cenas então não vai compilar :)

Compilar
```bash
clang++ *.cpp -o main -Wall -Wextra -Wpedantic -Wno-c++17-attribute-extensions -O0 -g3 $(pkg-config --cflags --libs sdl2) -lSDL2_ttf
```

![](https://github.com/JGSVb/Cubo-bonito/blob/main/video.gif)
