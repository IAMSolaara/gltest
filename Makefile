CC		:= g++
LDFLAGS := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
CFLAGS	:= -Iinclude

TARGETS	:= bin/main.o

all:	make_directories default

default:	$(TARGETS)
	$(CC) $(TARGETS) bin/glad.o $(LDFLAGS) $(CFLAGS) -o out/a.out

bin/glad.o:		include/glad/glad.c
	$(CC) $(CFLAGS) include/glad/glad.c -c -o bin/glad.o

bin/main.o:		main.cpp bin/glad.o
	$(CC) $(CFLAGS) main.cpp bin/glad.o -c -o bin/main.o

.PHONY:	clean
clean:
	rm -rf bin out

.PHONY:	make_directories
make_directories:	bin out out/res/shaders

bin:
	mkdir -p bin/

out:
	mkdir -p out/res/

out/res/shaders:
	cp -rf shaders out/res/shaders
