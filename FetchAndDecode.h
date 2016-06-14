/* Simple86_Emulator FetchAndDecode
*
* Implements the fetch, decode and general execution steps, for a
* Simple86 machine.
*
*/
#ifndef SIMULA_FETCHDECODE
#define SIMULA_FETCHDECODE 1

#include<cstdint>
#include"Memory.h"
#include"Execute.h"

// FetchAndDecode for Simple86
class FetchAndDecode {
private:
    // Other required machine modules, necessary for this module to work.
    Memory* memory;
    Execute* exec;

public:
    // Receives the other machines components at the object's creation.
    // The Memory pointer points to a Memory object that is going to be constantly
    // operated and changed, and the Execute pointer points to a Execute instance
    // that is going to be used to execute each instruction.
    FetchAndDecode(Memory* mem, Execute* alu) {
        this->memory = mem;
        this->exec = alu;
    }

    /* ------------------------------------------------------------------------
    * bool is16bitsInstruction(int8_t opCode)
    * Returns true if the passed instruction's opCode means that the instruction
    * lenght is equal to 16 bits.
    * ------------------------------------------------------------------------ */
    bool is16bitsInstruction(int8_t opCode) {
        return opCode == 14 || opCode == 17 || opCode == 20;
    }

    /* ------------------------------------------------------------------------
    * bool is32bitsInstruction(int8_t opCode)
    * Returns true if the passed instruction's opCode means that the instruction
    * lenght is equal to 32 bits.
    * ------------------------------------------------------------------------ */
    bool is32bitsInstruction(int8_t opCode) {
        return opCode == 4 || opCode == 5 || opCode == 7 || (opCode >= 10 && opCode <= 13) || opCode == 15 || opCode == 16 || opCode == 18 || opCode == 19;
    }

    /* ------------------------------------------------------------------------
    * bool is48bitsInstruction(int8_t opCode)
    * Returns true if the passed instruction's opCode means that the instruction
    * lenght is equal to 48 bits.
    * ------------------------------------------------------------------------ */
    bool is48bitsInstruction(int8_t opCode) {
        return opCode == 1 || opCode == 2 || opCode == 3 || opCode == 6 || opCode == 8 || opCode == 9;
    }

    /* ------------------------------------------------------------------------
    * initMachine()
    * With a given memory containing a valid Simple86 program, this function
    * implements the machine's control flux, and executes the program, step-by-step,
    * coordinating, accordingly, with the other machine's modules.
    * ------------------------------------------------------------------------ */
    void initMachine() {
        // Flux control variables
        int16_t i = 0;
        int16_t op1, op2;
        int8_t opCode;
        int8_t operandType;


        i = memory->getRegister(memory->Register::IP);

        while (i < MEMORY_LIMIT) {

            // Reads the instruction's opCode and operandType
            opCode = (int8_t)(memory->readMemory(i) >> 8);
            operandType = (int8_t)memory->readMemory(i);
            
            // Reads the instruction's arguments
            op1 = i + 1 < MEMORY_LIMIT ? memory->readMemory(i + 1) : 0;
            op2 = i + 2 < MEMORY_LIMIT ? memory->readMemory(i + 2) : 0;

            // Verifies the instruction's lenght and points the IP register to the next instruction.
            if (this->is16bitsInstruction(opCode)) {
                memory->setRegister(memory->Register::IP, memory->getRegister(memory->Register::IP) + 1);
            } else if (this->is32bitsInstruction(opCode)) {
                memory->setRegister(memory->Register::IP, memory->getRegister(memory->Register::IP) + 2);
            } else if (this->is48bitsInstruction(opCode)) {
                memory->setRegister(memory->Register::IP, memory->getRegister(memory->Register::IP) + 3);
            }

            // Passes the instruction to the Execute module, according to it's opCode.
            switch (opCode) {
                // 32 bits
            case 4: exec->mul(op1, operandType); break;
            case 5: exec->div(op1, operandType); break;
            case 7: exec->binaryNot(op1, operandType); break;
            case 10: exec->jmp(op1); break;
            case 11: exec->jz(op1); break;
            case 12: exec->js(op1); break;
            case 13: exec->call(op1); break;
            case 15: exec->push(op1, operandType); break;
            case 16: exec->pop(op1, operandType); break;
            case 18: exec->read(op1, operandType); break;
            case 19: exec->write(op1, operandType); break;
                // 48 bits
            case 1: exec->mov(op1, op2, operandType); break;
            case 2: exec->add(op1, op2, operandType); break;
            case 3: exec->sub(op1, op2, operandType); break;
            case 6: exec->binaryAnd(op1, op2, operandType); break;
            case 8: exec->binaryOr(op1, op2, operandType); break;
            case 9: exec->cmp(op1, op2, operandType); break;
                // 16 bits
            case 14: exec->ret(); break;
            case 17: exec->dump(); break;
            case 20: exec->halt(); break;
            default: break;
            }
            // Goes to the next instruction pointed by the IP register, or halts if
            // IP > MEMORY_LIMIT.
            i = memory->getRegister(memory->Register::IP);

        }
    }
};

#endif
