CC = g++
FLAGS = -Wall -std=c++11

all: emulator mounter linker

emulator : Memory.h Execute.h FetchAndDecode.h
	$(CC) $(FLAGS) mainEmulator.cpp -o Simple86_Emulator

mounter : Instruction.h Mounter.h
	$(CC) $(FLAGS) mainMounter.cpp -o Simple86_Mounter

linker : Instruction.h Linker.h
	$(CC) $(FLAGS) mainLinker.cpp -o Simple86_Linker
