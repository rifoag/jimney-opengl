all: build run

build: 
	gcc -lstdc++ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -L/lib -Iinclude -I/usr/include -o main main.cpp glad.c

run:
	./main
