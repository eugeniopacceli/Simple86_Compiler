CC = g++
FLAGS = -Wall -std=c++11

all: emulator compiler

emulator : Memory.h Execute.h FetchAndDecode.h
	$(CC) $(FLAGS) mainEmulator.cpp -o Simple86_Emulator

compiler : Instruction.h Compiler.h
	$(CC) $(FLAGS) mainCompiler.cpp -o Simple86_Compiler