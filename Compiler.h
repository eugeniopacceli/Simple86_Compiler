#ifndef SIMULA_COMPILER
#define SIMULA_COMPILER 1

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <iomanip>
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
        bool verboseEnabled;

    public:
        Compiler(ifstream* input, ofstream* output, bool verboseEnabled){
            this->input = input;
            this->output = output;
            this->program.clear();
            this->programSizeInBytes = 0;
            this->verboseEnabled = verboseEnabled;
        }

        void readProgram(ifstream* input){
            string str;
            int split;

            if(this->verboseEnabled){
                cout << "The following commands were read:" << endl;
                cout << left << setw(15) << setfill(' ') << "Pred. Address";
                cout << left << setw(30) << setfill(' ') << "Command" << endl;
            }
            while(getline(*input, str)){
                if(str.at(0) == '_'){
                    split = str.find( ":", 0);
                    // Label decode
                    this->program.push_back(Instruction(str.substr(0,split)));
                    this->program.back().address = programSizeInBytes;
                    if(this->verboseEnabled){
                        cout << left << setw(15) << setfill(' ') << this->program.back().address;
                        cout << left << setw(30) << setfill(' ') << program.back().fullText << endl;
                    }
                    str = str.substr(split+1,str.size());
                    if(str.empty()){
                        continue;
                    }
                    if(str.at(0) == ' '){
                        str = str.substr(1,str.size());
                    }
                }
                // Instruction decode
                this->program.push_back(Instruction(str));
                // Address computation
                this->program.back().address = programSizeInBytes;
                this->programSizeInBytes += this->bitSpaceToBytes(program.back().size);
                // ---
                if(this->verboseEnabled){
                    cout << left << setw(15) << setfill(' ') << this->program.back().address;
                    cout << left << setw(30) << setfill(' ') << program.back().fullText << endl;
                }
            }
            if(this->verboseEnabled){
                cout << endl;
            }
        }

        void resolveLabels(vector<Instruction>& instructions){
            if(this->verboseEnabled){
                cout << left << "Table of names " << setw(15) << setfill('=') << '=' << endl;
                cout << left << setw(15) << setfill(' ') << "Name";
                cout << left << setw(15) << setfill(' ') << "Address" << endl;                
            }

            for(Instruction& i : instructions){
                if(i.type == InstructionType::LABEL || i.type == InstructionType::VAR){
                    if(i.type == InstructionType::VAR){
                        i.address = this->programSizeInBytes;
                        this->programSizeInBytes += 2;
                    }

                    if(this->verboseEnabled){
                        cout << left << setw(15) << setfill(' ') << i.id;
                        cout << left << setw(15) << setfill(' ') << i.address << endl;
                    }

                    for(Instruction& j : instructions){
                        if(j.type == InstructionType::INSTRUCTION){
                            if(j.opA == i.id){
                                j.opA = to_string(i.address);
                            }
                            if(j.opB == i.id){
                                j.opB = to_string(i.address);
                            }
                        }
                    }
                }
            }

            if(this->verboseEnabled){
                cout << left << setw(30) << setfill('=') << '=' << endl << endl;
            }
        }

        int compile(){
            this->readProgram(this->input);
            this->resolveLabels(this->program);
            
            if(this->verboseEnabled){
                this->writeTextOutput(this->program);
            }

            this->writeBin(this->program);

            input->close();
            output->close();
            return 1;
        }

        void writeTextOutput(vector<Instruction> program){
            cout << "The following program will be written in binary:" << endl;
            cout << left << setw(15) << setfill(' ') << "Address";
            cout << left << setw(30) << setfill(' ') << "Command";
            cout << left << setw(10) << setfill(' ') << "Size (bytes)" << endl;
            for(Instruction& ins : program){
                if(ins.type != InstructionType::VAR && ins.type != InstructionType::LABEL){
                    cout << left << setw(15) << setfill(' ') << ins.address;
                    cout << left << setw(30) << setfill(' ') << ins.debugInstruction();
                    cout << left << setw(10) << setfill(' ') << this->bitSpaceToBytes(ins.size) << endl;
                }
            }
        }

        int16_t bitSpaceToBytes(int16_t bits){
            return bits/8;
        }


        void writeBin(vector<Instruction> toWrite){
            
            this->output->put(0);
            this->output->put(0);
            for(Instruction& i : toWrite){
                if(i.type == INSTRUCTION){
                    
                    //this->output->put(0);
                    this->output->put((char)i.type);
                    //this->output->put(0);
                    this->output->put((char)i.code);
                    
                    //opA
                    if(i.opType==OperandType::R || i.opType==OperandType::RR || i.opType==OperandType::RM || i.opType==OperandType::RI){
                        this->output->put((char)i.getRegisterCode(i.opA));
                        this->output->put(0);
                    } else if(i.opType==OperandType::I){
                        this->output->put((char)std::stoul(i.opA, nullptr, 16));
                        this->output->put((char)(std::stoul(i.opA, nullptr, 16) >> 8));
                    } else if(i.opType==OperandType::M || i.opType==OperandType::MI || i.opType==OperandType::MR){
                        this->output->put((char)(stoi(i.opA) >> 8));
                        this->output->put((char)stoi(i.opA));
                    }
                    
                    //opB
                    if(i.opType==OperandType::RR || i.opType==OperandType::MR){
                        this->output->put((char)i.getRegisterCode(i.opB));
                        this->output->put(0);
                    } else if(i.opType==OperandType::MI || i.opType==OperandType::RI){
                        this->output->put((char)std::stoul(i.opB, nullptr, 16));
                        this->output->put((char)(std::stoul(i.opA, nullptr, 16) >> 8));                        
                    }
                    else if(i.opType==OperandType::RM){
                        this->output->put((char)(stoi(i.opB) >> 8));
                        this->output->put((char)stoi(i.opB));
                    }
                }
            }
        }

        ostream& write_word(ostream& out, int16_t value){
            for (uint size = sizeof(int16_t); size > 0; size--)
                out.put(static_cast<char>(value & 0xFF));
                value >>= 8;
            return out;
        }
};

#endif
