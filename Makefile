LDLIBS=-lglut  -lGLEW -lGL -lm
all: bingbong
clean:
	rm -f *.o bingbong
.PHONY: all clean
