/* Simple86_Emulator main
 *
 * Entry point for a program that implements the Simple86 machine.
 * Specification of that machine is defined in "TP1 - Software Básico.pdf"
 *
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <inttypes.h>
#include "Memory.h"
#include "Execute.h"
#include "FetchAndDecode.h"

/* ------------------------------------------------------------------------
 * Memory *populateMemory(char* file)
 * Reads a binary input file, containing a Simple86 program, and populates
 * the machine memory with it.
 * ------------------------------------------------------------------------ */
Memory* populateMemory(char* file) {
    Memory* memory = new Memory();
    int16_t i;
    int16_t numInst;
    int16_t bufferIn[MEMORY_LIMIT] = { 0 };
    int16_t ip;

    FILE* fIn = fopen(file, "r");
    fread(&ip, 2, 1, fIn);
    memory->setRegister(Memory::Register::IP, ip);
    numInst = (int16_t)fread((void*)bufferIn, 2, MEMORY_LIMIT, fIn);
    fclose(fIn);


    for (i = 0; i < numInst; i++) {
        memory->writeMemory(i, bufferIn[i]);
    }

    return memory;
}

/* ------------------------------------------------------------------------
* int main(int argc, char* argv[])
* Entry point. Receives the address of the file, containing the program to be
* executed in the emulator, as a argument. If no argument is specified, returns,
* else mounts the machine, loads the program and begins it's execution.
* ------------------------------------------------------------------------ */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Needs at least one argument: name of the program file" << std::endl;
        return 0;
    }

    // Machine is instantiated.
    Memory* memory = populateMemory(argv[1]);
    Execute* execute = new Execute(memory);
    FetchAndDecode* fetchAndDecode = new FetchAndDecode(memory, execute);

    // Machine execution started.
    fetchAndDecode->initMachine();

    delete execute;
    delete fetchAndDecode;
    delete memory;

    return 0;
}
