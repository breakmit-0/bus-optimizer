libs = lib/chemin.o lib/graph.o lib/opti.o lib/display.o lib/gui.o lib/input.o
opts = -std=c++17
wall = -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter
dashl = -lallegro -lallegro_font -lallegro_image  -lallegro_audio -lallegro_acodec -lallegro_primitives

# activate to recompile everything
# MAKEFLAGS += --always-make

main: $(libs)
	gcc -o .build/main.exe $(libs) main.cpp $(opts) $(wall) $(dashl)

lib/input.o: lib/input.h lib/input.cpp
	gcc -c -o lib/input.o lib/input.cpp $(opts) $(wall)

lib/gui.o: lib/gui.cpp lib/gui.h
	gcc -c -o lib/gui.o lib/gui.cpp $(opts) $(wall) $(dashl)

lib/display.o: lib/display.cpp lib/display.h
	gcc -c -o lib/display.o lib/display.cpp $(opts) $(wall)

lib/graph.o: lib/graph.cpp lib/graph.h
	gcc -c -o lib/graph.o lib/graph.cpp $(opts) $(wall)

lib/chemin.o: lib/chemin.cpp lib/chemin.h
	gcc -c -o lib/chemin.o lib/chemin.cpp $(opts) $(wall)

lib/opti.o: lib/opti.cpp lib/opti.h
	gcc -c -o lib/opti.o lib/opti.cpp $(opts) $(wall)

run:
	./.build/main.exe