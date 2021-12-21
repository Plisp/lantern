CXX = clang++
CXXFLAGS = -Wall -g -fsanitize=undefined

all:
	$(CXX) $(CFLAGS) main.cpp map.cpp display_sdl.c libfov.c -lSDL2

clean:
	rm a.out
