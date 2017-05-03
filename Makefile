LDLIBS=-lglut  -lGLEW -lGL -lm
all: hittheball
clean:
	rm -f *.o hittheball
.PHONY: all clean
