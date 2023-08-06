CXXFLAGS := -I./include

all: build run

build: ./src/*.cpp
	g++ $(CXXFLAGS) $^ -o ./build/main

run: 
	./build/main

debug: ./src/*.cpp
	g++ -g $(CXXFLAGS) $^ -o ./debug/main_dbg

gdb: debug
	gdb ./debug/main_dbg

clean:
	rm -rf build/* debug/*


.PHONY: all build run debug clean

