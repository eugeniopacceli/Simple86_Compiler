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

enum OperandType{
    N = 0,
    R = 1,
    M = 2,
    RM = 3,
    MR = 4,
    RR = 5,
    MI = 6,
    RI = 7,
    I = 8
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
    NORG = 99
};

class Instruction{
    public:
        string fullText;
        string id;
        string opA, opB;
        InstructionType type;
        InstructionCode code;
        OperandType opType;
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

        RegisterCode getRegisterCode(string id){
            if(id == "al") return RegisterCode::AL;
            if(id == "ah") return RegisterCode::AH;
            if(id == "ax") return RegisterCode::AX;
            if(id == "bh") return RegisterCode::BH;
            if(id == "bl") return RegisterCode::BL;
            if(id == "bx") return RegisterCode::BX;
            if(id == "cl") return RegisterCode::CL;
            if(id == "ch") return RegisterCode::CH;
            if(id == "cx") return RegisterCode::CX;
            return RegisterCode::NORG;
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

        OperandType determinOperandType(string a, string b){
            char opAType = '0';
            char opBType = '0';
            string typeStr;
            
            if(!(a.empty())){
                if(a.at(0)== '0'){
                    opAType = 'I';
                }else if(a.at(0) == '_'){
                    opAType = 'M';
                }else{
                    opAType = 'R';
                }
                
                if(!(b.empty())){
                    if(b.at(0)== '_'){
                        opBType = 'M';
                    }else if(b.at(0) == '0'){
                        opBType = 'I';
                    }else{
                        opBType = 'R';
                    }
                }
            }

            typeStr += opAType;
            typeStr += opBType;

            if(typeStr == "00"){
                return OperandType::N;
            }else if(typeStr == "I0"){
                return OperandType::I;
            }else if(typeStr == "M0"){
                return OperandType::M;
            }else if(typeStr == "R0"){
                return OperandType::R;
            }else if(typeStr == "RI"){
                return OperandType::RI;
            }else if(typeStr == "MI"){
                return OperandType::MI;
            }else if(typeStr == "MR"){
                return OperandType::MR;
            }else if(typeStr == "RM"){
                return OperandType::RM;
            }else if(typeStr == "RR"){
                return OperandType::RR;
            }

            return OperandType::N;
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
            // Remove ':'
            this->id.erase(std::remove(this->id.begin(), this->id.end(), ':'), this->id.end());

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

            this->opType = this->determinOperandType(this->opA,this->opB);
            this->size = getInstructionSize(this->code);
            this->address = 0;
        }

        string debugInstruction(){
            string str = this->id;
            if(!(this->opA.empty())){
                str += ' ' + this->opA;
            }
            if(!(this->opB.empty())){
                str += ", " + this->opB ;
            }
            transform(str.begin(), str.end(), str.begin(), ::toupper);

            return str;
        }
};

#endif
