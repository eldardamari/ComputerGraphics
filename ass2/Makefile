CC = g++
FILES = main.cpp
LIBS = -lglut -lGL -lGLU -lX11 -lXmu -lm
STD = -std=c++11
OUT_EXE = run

build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES) $(LIBS)

clean:
	rm -f *.o run

rebuild: clean build
