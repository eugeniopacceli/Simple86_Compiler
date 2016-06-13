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

#define LOW_MASK  255 //0000000011111111
#define HIGH_MASK -32678 // 1111111100000000

// Compiler module for Simple86
class Compiler{

    private:
        ifstream* input;
        ofstream* output;
        vector<Instruction> program;
        int16_t programSizeInBytes;

    public:
        Compiler(ifstream* input, ofstream* output, bool verboseEnabled){
            this->input = input;
            this->output = output;
            this->program.clear();
            this->programSizeInBytes = 0;
        }

        void readProgram(ifstream* input){
            string str;
            cout << "The following lines were read:" << endl;
            while(getline(*input, str)){
                // Instruction decode
                this->program.push_back(Instruction(str));
                // Address computation
                this->program.back().address = programSizeInBytes;
                this->programSizeInBytes += this->bitSpaceToBytes(program.back().size);
                // ---
                cout << "[" << this->program.back().address << "] : " << program.back().fullText << endl;
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

        int16_t bitSpaceToBytes(int16_t bits){
            return bits/8;
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
