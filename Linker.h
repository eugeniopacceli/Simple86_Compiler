/* Simple86_Linker Instruction
 *
 * Implements the linkr that translates modules objects into
 * an executable Simple86 machine code, outputs binary.
 *
 */

#ifndef SIMULA_LINKER
#define SIMULA_LINKER 1

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <iomanip>
#include "Instruction.h"

using namespace std;

// Linker module for Simple86
class Linker{

    private:
        vector<string> inputs; // Input modules object files
        ofstream* output; // outputfile
        vector<Instruction> program; // Program is an array of Instructions
        int16_t programSizeInBytes; // Total number of bytes this program has
        bool verboseEnabled; // -v flag

    public:

       /* ------------------------------------------------------------------------
        * Linker(ifstream* input, ofstream* output, bool verboseEnabled)
        * Instantializes a Linker object that knows it's IO files
        * ------------------------------------------------------------------------ */
        Linker(vector<string> inputs, ofstream* output, bool verboseEnabled){
            this->inputs = inputs;
            this->output = output;
            this->program.clear();
            this->programSizeInBytes = 0;
            this->verboseEnabled = verboseEnabled;            
        }

       /* ------------------------------------------------------------------------
        * void readProgram(ifstream* input)
        * Reads all the object files received by parameter and populates the program
        * vector.
        * ------------------------------------------------------------------------ */
        void readProgram(vector<string> inputStrings){
            this->program.clear();
            for(string& i : inputStrings){
                linkModule(i);
            }
        }

        /* ------------------------------------------------------------------------
        * int linkModule(string inputName)
        * Receives an name of a module object file. It appends its binary code
        * at the end of the current program and fixes the addresses. The module
        * file must be produced by the Mounter provided in this project, it's output
        * is formatted and accepted by this linker.
        * ------------------------------------------------------------------------ */
       void linkModule(string inputName){
           ifstream* input = new ifstream(inputName.c_str(),ios::in|ios::binary);
           
           if(!input->is_open()){
           		cerr << "File " << inputName << " could not be read. Ignoring this module, this may produce unwanted results and errors." << endl;
           		return;
           }

           while(!input->eof()){
                Instruction temp;
                char fullText[128] = { 0 };
                char id[128] = { 0 };
                char opA[128] = { 0 };
                char opB[128] = { 0 };
                input->read((char *)fullText, sizeof(char)*128);
                input->read((char *)id, sizeof(char)*128);
                input->read((char *)opA, sizeof(char)*128);
                input->read((char *)opB, sizeof(char)*128);
                temp.fullText = string(fullText);
                temp.id = string(id);
                temp.opA = string(opA);
                temp.opB = string(opB);
                input->read((char *)(&temp.type), sizeof(InstructionType));
                input->read((char *)(&temp.code), sizeof(InstructionCode));
                input->read((char *)(&temp.opType), sizeof(OperandType));
                input->read((char *)(&temp.address),sizeof(int16_t));
                input->read((char *)(&temp.size), sizeof(int16_t));
                temp.address = programSizeInBytes / 2;
                if(!temp.fullText.empty()){
                	this->programSizeInBytes += this->bitSpaceToBytes(temp.size);
                	this->program.push_back(temp);
            	}
            }    
            input->close();
        }

       /* ------------------------------------------------------------------------
        * void resolveLabels(vector<Instruction>& instructions)
        * The second pass, resolves all labels used in the program by searching
        * the vector program for labels and dws, then for each one of those found
        * search the vector again for instructions that used those, and replaces
        * the operands refeering to them to the actual memory address they represent.
        * ------------------------------------------------------------------------ */
        void resolveLabels(vector<Instruction>& instructions){
            if(this->verboseEnabled){
                cout << left << "Table of names " << setw(15) << setfill('=') << '=' << endl;
                cout << left << setw(15) << setfill(' ') << "Name";
                cout << left << setw(15) << setfill(' ') << "Address" << endl;                
            }

            // Searches for labels and dws, passing by each instruction
            for(Instruction& i : instructions){
                if(i.type == InstructionType::LABEL || i.type == InstructionType::VAR){
                    if(i.type == InstructionType::VAR){
                        // Variables are stored after the program. The program is stored at 0
                        i.address = this->programSizeInBytes / 2;
                        i.id = i.opA; // dw's pseudo instruction id is now it's name
                        this->programSizeInBytes += 2;
                    }

                    // Searches for instructions using current label or reserved word
                    for(Instruction& j : instructions){
                        // Replaces the labels and memory references by their actual address
                        if(j.type == InstructionType::INSTRUCTION){
                            if(j.opA == i.id){
                                j.opA = to_string(i.address);
                            }
                            if(j.opB == i.id){
                                j.opB = to_string(i.address);
                            }
                        }
                    }

                    if(this->verboseEnabled){
                        cout << left << setw(15) << setfill(' ') << i.id;
                        cout << left << setw(15) << setfill(' ') << i.address << endl;
                    }
                }
            }

            if(this->verboseEnabled){
                cout << left << setw(30) << setfill('=') << '=' << endl << endl;
            }


        }
       
       /* ------------------------------------------------------------------------
        * int link()
        * Reads modules object files. Resolve addresses and writes an executable
        * binary to the output file.
        * ------------------------------------------------------------------------ */
        int link(){
            this->readProgram(this->inputs); // First step
            this->resolveLabels(this->program); // Second step
            
            //Writes the end results inside the vector<Instruction> program
            if(this->verboseEnabled){
                this->writeTextOutput(this->program);
            }

            // Transforms the vector<Instruction> program into a real program output
            // to the output file.
            this->writeBin(this->program);
            output->close();
            return 1;
        }

       /* ------------------------------------------------------------------------
        * void writeTextOutput(vector<Instruction> program)
        * Outputs all the Instructions inside the vector. Used after the second pass
        * to show what has been understood by the mounter, and to what the labels
        * were resolved to.
        * ------------------------------------------------------------------------ */
        void writeTextOutput(vector<Instruction> program){
            // Everything output as table
            cout << "The following program will be written in binary:" << endl;
            cout << left << setw(15) << setfill(' ') << "Address";
            cout << left << setw(30) << setfill(' ') << "Command";
            cout << left << setw(10) << setfill(' ') << "Size (bytes)" << endl;
            for(Instruction& ins : program){
                // VAR and LABEL have 0 bits as size, and won't be output to binary, only
                // the addresses those resolve to.
                if(ins.type != InstructionType::VAR && ins.type != InstructionType::LABEL){
                    cout << left << setw(15) << setfill(' ') << ins.address;
                    cout << left << setw(30) << setfill(' ') << ins.debugInstruction();
                    cout << left << setw(10) << setfill(' ') << this->bitSpaceToBytes(ins.size) << endl;
                }
            }
        }

       /* ------------------------------------------------------------------------
        * int16_t bitSpaceToBytes(int16_t bits)
        * Converts bits to bytes. Used generally to compute byte space used by a
        * instruction, by passing its Instruction->size.
        * ------------------------------------------------------------------------ */
        int16_t bitSpaceToBytes(int16_t bits){
            return bits/8;
        }

       /* ------------------------------------------------------------------------
        * void writeBin(vector<Instruction> toWrite)
        * Transforms the vector into a binary program output to the output file.
        * The vector is intact. This is the second compilation pass.
        * Uses little endian notation, as required by the Simple86 machine.
        * ------------------------------------------------------------------------ */
        void writeBin(vector<Instruction> toWrite){
            
            // The word representing the address where the first instruction of
            // the program is at.
            this->output->put(0);
            this->output->put(0);
            // for each instruction inside the vector
            for(Instruction& i : toWrite){
                if(i.type == INSTRUCTION){
                    // The ofstream->put(char) function writes in binary
                    this->output->put((char)i.opType); // Operand type
                    this->output->put((char)i.code); // Instruction code
                    
                    // Outputs opA according to the operand type
                    if(i.opType==OperandType::R || i.opType==OperandType::RR || i.opType==OperandType::RM || i.opType==OperandType::RI){
                        this->output->put((char)i.getRegisterCode(i.opA));
                        this->output->put(0);
                    } else if(i.opType==OperandType::I){
                        // Converts the string representing a hexa number to binary
                        this->output->put((char)std::stoul(i.opA, nullptr, 16));
                        this->output->put((char)(std::stoul(i.opA, nullptr, 16) >> 8));
                    } else if(i.opType==OperandType::M || i.opType==OperandType::MI || i.opType==OperandType::MR){
                        // Converts the string representing a int number to binary
                        this->output->put((char)stoi(i.opA));
                        this->output->put((char)(stoi(i.opA) >> 8));
                    }
                    
                    // Outputs opB according to the operand type
                    if(i.opType==OperandType::RR || i.opType==OperandType::MR){
                        this->output->put((char)i.getRegisterCode(i.opB));
                        this->output->put(0);
                    } else if(i.opType==OperandType::MI || i.opType==OperandType::RI){
                        // Converts the string representing a hexa number to binary
                        this->output->put((char)std::stoul(i.opB, nullptr, 16));
                        this->output->put((char)(std::stoul(i.opA, nullptr, 16) >> 8));                        
                    }
                    else if(i.opType==OperandType::RM){
                        // Converts the string representing a int number to binary
                        this->output->put((char)stoi(i.opB));
                        this->output->put((char)(stoi(i.opB) >> 8));
                    }
                }
            }
        }

        /* ------------------------------------------------------------------------
        * void debugReceivedInstruction(Instruction& i)
        * Prints (formatted) a Instruction and what is inside it so far.
        * Used at the first step if verbose is enabled.
        * ------------------------------------------------------------------------ */
        void debugReceivedInstruction(Instruction& i){
            string ops = i.id;
            if(!i.opA.empty()){
                ops += " " + i.opA;
            }
            if(!i.opB.empty()){
                ops += ", " + i.opB;
            }
            cout << left << setw(15) << setfill(' ') << i.address;
            cout << left << setw(10) << setfill(' ') << ops;
            cout << endl;
        }
};

#endif
