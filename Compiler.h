#ifndef SIMULA_COMPILER
#define SIMULA_COMPILER 1

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include "Instruction.h"

using namespace std;

#define LOW_MASK  0000000011111111
#define HIGH_MASK 1111111100000000

// Compiler module for Simple86
class Compiler{

    private:
        ifstream* input;
        ofstream* output;
        vector<Instruction> program;

    public:
        Compiler(ifstream* input, ofstream* output, bool verboseEnabled){
            this->input = input;
            this->output = output;
            this->program.clear();
        }

        void readProgram(ifstream* input){
            string str;
            int i = 0;
            cout << "The following lines were read:" << endl;
            while(getline(*input, str)){
                this->program.push_back(Instruction(str));
                cout << "[" << i++ << "] : " << program.back().fullText << endl;
            }
        }

        int compile(){
            this->readProgram(this->input);
            for(uint i = 0; i < program.size(); i++){
                program.at(i).debugInstruction();
            }
            input->close();
            output->close();
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
