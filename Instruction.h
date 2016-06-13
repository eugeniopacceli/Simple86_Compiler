#ifndef SIMULA_INSTRUCTION
#define SIMULA_INSTRUCTION 1

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdint>

using namespace std;

enum InstructionType{
    LABEL,
    INSTRUCTION,
    VAR
};

enum InstructionCode{
    MOV = 1,
    ADD = 2,
    SUB = 3,
    AND = 6,
    OR = 8,
    CMP = 9,
    MUL = 4,
    DIV = 5,
    NOT = 7,
    JUMP = 10,
    JZ = 11,
    JS = 12,
    CALL = 13,
    PUSH = 15,
    POP = 16,
    READ =18,
    WRITE = 19,
    RET = 14,
    DUMP = 17,
    HALT = 20,
    NOPE = 99
};

enum RegisterCode{
    AL = 0,
    AH = 1,
    AX = 2,
    BH = 3,
    BL = 4,
    BX = 5,
    CL = 6,
    CH = 7,
    CX = 8,
    ZF = 9
};

class Instruction{
    public:
        string fullText;
        string id;
        string opA, opB;
        InstructionType type;
        InstructionCode code;
        int16_t address;
        int16_t size;

        InstructionCode getInstructionCode(string id){
            if(id == "mov") return InstructionCode::MOV;
            if(id == "add") return InstructionCode::ADD;
            if(id == "sub") return InstructionCode::SUB;
            if(id == "and") return InstructionCode::AND;
            if(id == "or") return InstructionCode::OR;
            if(id == "cmp") return InstructionCode::CMP;
            if(id == "mul") return InstructionCode::MUL;
            if(id == "div") return InstructionCode::DIV;
            if(id == "not") return InstructionCode::NOT;
            if(id == "jmp") return InstructionCode::JUMP;
            if(id == "jz") return InstructionCode::JZ;
            if(id == "js") return InstructionCode::JS;
            if(id == "call") return InstructionCode::CALL;
            if(id == "push") return InstructionCode::PUSH;
            if(id == "pop") return InstructionCode::POP;
            if(id == "read") return InstructionCode::READ;
            if(id == "write") return InstructionCode::WRITE;
            if(id == "ret") return InstructionCode::RET;
            if(id == "dump") return InstructionCode::DUMP;
            if(id == "halt") return InstructionCode::HALT;
            return InstructionCode::NOPE;
        }

        int16_t getInstructionSize(InstructionCode code){
            switch(code){ // without breaks, a switch case falls to the cases bellow.
                case InstructionCode::MOV:
                case InstructionCode::ADD:
                case InstructionCode::SUB:
                case InstructionCode::AND:
                case InstructionCode::OR:
                case InstructionCode::CMP: return 48;
                case InstructionCode::MUL:
                case InstructionCode::DIV:
                case InstructionCode::NOT:
                case InstructionCode::JUMP:
                case InstructionCode::JZ:
                case InstructionCode::JS:
                case InstructionCode::CALL:
                case InstructionCode::PUSH:
                case InstructionCode::POP:
                case InstructionCode::READ:
                case InstructionCode::WRITE: return 32;
                case InstructionCode::RET:
                case InstructionCode::DUMP:
                case InstructionCode::HALT: return 16;
                default: return 0;
            }
        }

        Instruction(string full){
            // Removes comments from string
            istringstream untilComment(full);
            getline(untilComment,this->fullText,';');

            // All chars to lower case
            transform(this->fullText.begin(), this->fullText.end(), this->fullText.begin(), ::tolower);
            
            istringstream stringNav(this->fullText);
            getline(stringNav,this->id,' '); // Gets the first word. istream is pointing to the word after ' '
            getline(stringNav,this->opA,','); // Gets opA. istream is pointing to the word after ',' (or eof)
            getline(stringNav,this->opB); // Gets opB or nothing.

            // Remove remaining ' '
            this->id.erase(remove_if(this->id.begin(), this->id.end(), ::isspace), this->id.end());
            this->opA.erase(remove_if(this->opA.begin(), this->opA.end(), ::isspace), this->opA.end());
            this->opB.erase(remove_if(this->opB.begin(), this->opB.end(), ::isspace), this->opB.end());

            if(id.at(0) == '_'){
                this->type = InstructionType::LABEL;
                this->code = InstructionCode::NOPE;
            }else if(id == "dw"){
                this->type = InstructionType::VAR;
                this->code = InstructionCode::NOPE;
            }else{
                this->type = InstructionType::INSTRUCTION;
                this->code = getInstructionCode(this->id);
            }

            this->size = getInstructionSize(this->code);
            this->address = 0;
        }

        void debugInstruction(){
            cout << this->fullText << endl;
            cout << "Id: " << this->id << endl << "OpA: " << this->opA << endl << "OpB: " << this->opB << endl;
            cout << "Size: " << this->size << endl << "Address: " << this->address << endl;
        }
};

#endif