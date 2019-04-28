all: build run

build: 
	gcc -lstdc++ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -L/lib -Iinclude -I/usr/include -o main main.cpp include/glad.c

run:
	./main

rifo:
	g++ -I/usr/inlcude -Iinclude -o main main.cpp glad.c -lglfw3 -lGL -lGLU -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -lpthread -ldl -lm

remove:
	rm main
