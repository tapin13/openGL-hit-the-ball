LDLIBS=-lglut  -lGLEW -lGL -lm
all: pingpong
clean:
	rm -f *.o pingpong
.PHONY: all clean