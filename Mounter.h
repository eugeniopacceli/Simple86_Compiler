/* Simple86_Mounter Instruction
 *
 * Implements the mounter that translates asm
 * int Simple86 machine code, outputs binary.
 *
 */

#ifndef SIMULA_MOUNTER
#define SIMULA_MOUNTER 1

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <iomanip>
#include "Instruction.h"

using namespace std;

// Mounter module for Simple86
class Mounter{

    private:
        ifstream* input; // input file
        ofstream* output; // outputfile
        vector<Instruction> program; // Program is an array of Instructions
        int16_t programSizeInBytes; // Total number of bytes this program has
        bool verboseEnabled; // -v flag

    public:

       /* ------------------------------------------------------------------------
        * Mounter(ifstream* input, ofstream* output, bool verboseEnabled)
        * Instantializes a Mounter object that knows it's IO files and the -v flag
        * ------------------------------------------------------------------------ */
        Mounter(ifstream* input, ofstream* output, bool verboseEnabled){
            this->input = input;
            this->output = output;
            this->program.clear();
            this->programSizeInBytes = 0;
            this->verboseEnabled = verboseEnabled;
        }

       /* ------------------------------------------------------------------------
        * void readProgram(ifstream* input)
        * Reads a program from a text file and populates the vector<Instruction> program
        * object. The first compilation pass. Ends with all instructions and commands
        * partially decoded, but labels still don't know the address they represent.
        * ------------------------------------------------------------------------ */
        void readProgram(ifstream* input){
            string str;
            int split;

            if(this->verboseEnabled){
                cout << "The following commands were read:" << endl;
                cout << left << setw(15) << setfill(' ') << "Pred. Address";
                cout << left << setw(10) << setfill(' ') << "Command" << endl;
            }
            // Reads peer line
            while(getline(*input, str)){
                // This if detects it there is a label at the current line
                // splits the label and instruction into two different
                // strings
                if(str.at(0) == '_'){
                    split = str.find( ":", 0);
                    // Label decode
                    this->program.push_back(Instruction(str.substr(0,split)));
                    this->program.back().address = programSizeInBytes / 2;
                    if(this->verboseEnabled){
                        this->debugReceivedInstruction(this->program.back());
                    }
                    // The instruction after the label.
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
                this->program.back().address = programSizeInBytes / 2;
                this->programSizeInBytes += this->bitSpaceToBytes(program.back().size);
                if(this->verboseEnabled){
                    this->debugReceivedInstruction(this->program.back());
                }
            }
            if(this->verboseEnabled){
                cout << endl;
            }
        }

       /* ------------------------------------------------------------------------
        * void resolveLabels(vector<Instruction>& instructions)
        * The second pass, resolves all labels used in the program by searching
        * the vector program for labels and dws, then for each one of those found
        * search the vector again for instructions that used those, and replaces
        * the operands refeering to them to the actual memory address they represent.
        * ------------------------------------------------------------------------ */
        void resolveLocalLabels(vector<Instruction>& instructions){
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
                        this->programSizeInBytes += 2;
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
        * int mount()
        * Applies the first and second pass to the program received, generating a
        * binary. This function coordinates the compilation process, which begins
        * with an input file, passes through an array of Instruction objects
        * representing what has been processed so far, and ends with a fully processed
        * vector which is transformed into a binary file equivalent to the program
        * initially received.
        * ------------------------------------------------------------------------ */
        int mount(){
            this->readProgram(this->input); // First step
            this->resolveLocalLabels(this->program); // Second step
            
            // Writes the end results inside the vector<Instruction> program
            if(this->verboseEnabled){
                this->writeTextOutput(this->program);
            }

            // Transforms the vector<Instruction> program into a real program output
            // to the output file.
            this->writeObject(this->program);

            input->close();
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
        void writeObject(vector<Instruction> toWrite){
            // for each instruction inside the vector
            for(Instruction& i : toWrite){
                char fullText[128] = { 0 };
                char id[128] = { 0 };
                char opA[128] = { 0 };
                char opB[128] = { 0 };
                i.fullText.copy(fullText,i.fullText.size(),0);
                i.id.copy(id,i.id.size(),0);
                i.opA.copy(opA,i.opA.size(),0);
                i.opB.copy(opB,i.opB.size(),0);
                this->output->write((char *)fullText, sizeof(char)*128);
                this->output->write((char *)id, sizeof(char)*128);
                this->output->write((char *)opA, sizeof(char)*128);
                this->output->write((char *)opB, sizeof(char)*128);
                this->output->write((char *)(&i.type), sizeof(InstructionType));
                this->output->write((char *)(&i.code), sizeof(InstructionCode));
                this->output->write((char *)(&i.opType), sizeof(OperandType));
                this->output->write((char *)(&i.address),sizeof(int16_t));
                this->output->write((char *)(&i.size), sizeof(int16_t));
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
