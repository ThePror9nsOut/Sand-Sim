all: compile link run

compile:
	g++ -Isrc/include -c src/main.cpp

link:
	g++ main.o -o main -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system

run:
	.\main.exe