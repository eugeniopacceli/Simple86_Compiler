#ifndef SIMULA_COMPILER
#define SIMULA_COMPILER 1

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>

using namespace std;

#define LOW_MASK  0000000011111111
#define HIGH_MASK 1111111100000000
#define MEMORY_LIMIT 1000

// Compiler module for Simple86
class Compiler{

    public:
        Compiler(ifstream& input, ofstream& output, bool verboseEnabled){
            input.close();
            output.close();
        }

        int compile(){
            return 1;
        }

        ostream& write_word(ostream& out, int16_t value){
            for (uint size = sizeof(int16_t); size > 0; size--)
                out.put(static_cast<char>(value & 0xFF));
                value >>= 8;
            return out;
        }

        istream& read_word(istream& in, int16_t value){
            uint size = 0;
            for (value = 0; size < sizeof(int16_t); size++)
                value |= in.get() << (8 * size);
            return in;
        }
};

#endif