CC         = clang++
CFLAGS     = -Werror -g -ferror-limit=1 -O0
LIBS       = -lpthread
SOURCES    = main.cpp
EXECUTABLE = opengl-test-dvbris


UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS += -lSDL2 -lGLEW -lGL -lGLU
endif
ifeq ($(UNAME_S),Darwin)
	CFLAGS +=  -F/Library/Frameworks -framework SDL2 -framework OpenGL
endif


all:
	$(CC) $(CFLAGS) $(SOURCES) $(LIBS) -o $(EXECUTABLE)


clean:
	find . -name '*.o' -type f -delete
	rm $(EXECUTABLE)
