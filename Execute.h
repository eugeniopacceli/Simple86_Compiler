/* Simple86_Emulator Execution
*
* Implements the Execution module for a Simple86 machine.
* Responsible for executing each instruction.
*
*/
#ifndef SIMULA_EXECUTE
#define SIMULA_EXECUTE 1

#include<cstdint>
#include<iostream>
#include<iomanip>
#include"Memory.h"

// Instruction argument types
#define opN 0 // Empty
#define opR 1 // Register
#define opM 2 // Memory
#define opRM 3 // Register-Memory
#define opMR 4 // Memory-Register
#define opRR 5 // Register-Register
#define opMI 6 // Memory-Absolute
#define opRI 7 // Register-Absolute
#define opI 8 // Absolute

using namespace std;

class Execute {
private:
    // Memory module
    Memory* memory;

    /* ------------------------------------------------------------------------
    * template<typename T> void writeToOutput(T t)
    * Prints a given object or type to screen, according to the
    * format specified at the docs.
    * ------------------------------------------------------------------------ */
    template<typename T>
    void writeToOutput(T t) {
        cout << left << setw(6) << setfill(' ') << t;
    }

    /* ------------------------------------------------------------------------
    * void writeHexToOutput(int16_t value)
    * Prints a given int16_t value, formatted, and in hexadecimal base.
    * ------------------------------------------------------------------------ */
    void writeHexToOutput(int16_t value) {
        cout << right << setw(4) << setfill('0') << hex << value << "  ";
    }

public:
    // Instantiates a Execute object with a pointer to a valid Memory module object.
    Execute(Memory* mem) {
        this->memory = mem;
    }

    /* -----------------------------------------------------------------------
    Most of the instructions implementations bellow have, as results, modifications
    that affect the Memory module. Registers and memory positions are modified,
    by each method, according to the equivalent instructions's specification.
    The Memory pointer passed to the constructor points to the Memory object that
    is going to be manipulated.
    The arguments are, generally:
    destiny : refers to a register or memory that is going to be written with
              the method's results.
    source : refers to a register or memory containing a value needed to the
             computation of the instruction, is only going to be read.
    operandType : represents one of the instructions argument types.
    --------------------------------------------------------------------------*/

    /* ------------------------------------------------------------------------
    * void mov(int16_t destiny, int16_t source, int16_t operandType)
    * Implements the Simple86's MOV instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void mov(int16_t destiny, int16_t source, int16_t operandType) {
        switch (operandType) {
        case opRM:
            memory->setRegister(memory->getRegName(destiny), memory->readMemory(source));
            break;
        case opMR:
            memory->writeMemory(destiny, memory->getRegister(memory->getRegName(source)));
            break;
        case opRR:
            memory->setRegister(memory->getRegName(destiny), memory->getRegister(memory->getRegName(source)));
            break;
        case opMI:
            memory->writeMemory(destiny, source);
            break;
        case opRI:
            memory->setRegister(memory->getRegName(destiny), source);
        }
    }

    /* ------------------------------------------------------------------------
    * void updateZFandSF(int16_t value)
    * Given value, sets Memory's ZF register to 1, if value = 0, or 0 if otherwise,
    * and SF to 1 if value is a positive number, or 0, if value is negative.
    * ------------------------------------------------------------------------ */
    void updateZFandSF(int16_t value) {
        memory->setRegister(memory->Register::ZF, value == 0 ? 1 : 0);
        memory->setRegister(memory->Register::SF, value >= 0 ? 0 : 1);
    }

    /* ------------------------------------------------------------------------
    * void add(int16_t destiny, int16_t source, int16_t operandType)
    * Implements the Simple86's ADD instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void add(int16_t destiny, int16_t source, int16_t operandType) {
        int16_t opA, opB;
        Memory::Register reg;
        switch (operandType) {
        case opRM:
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            opB = memory->readMemory(source);
            opB += opA;
            memory->setRegister(reg, opB);
            break;
        case opMR:
            opA = memory->readMemory(destiny);
            reg = memory->getRegName(source);
            opB = memory->getRegister(reg);
            memory->writeMemory(destiny, opA + opB);
            break;
        case opRR:
            reg = memory->getRegName(source);
            opB = memory->getRegister(reg);
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            memory->setRegister(reg, opA + opB);
            break;
        case opMI:
            opA = memory->readMemory(destiny);
            opB = memory->readMemory(opA);
            memory->writeMemory(opA, opB + source);
            break;
        case opRI:
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            opA += source;
            memory->setRegister(reg, opA);
            break;
        }

        this->updateZFandSF(opA);
    }

    /* ------------------------------------------------------------------------
    * void sub(int16_t destiny, int16_t source, int16_t operandType)
    * Implements the Simple86's SUB instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void sub(int16_t destiny, int16_t source, int16_t operandType) {
        int16_t opA, opB;
        Memory::Register reg;
        switch (operandType) {
        case opRM:
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            opB = memory->readMemory(source);
            opA -= opB;
            memory->setRegister(reg, opA);
            break;
        case opMR:
            opA = memory->readMemory(destiny);
            reg = memory->getRegName(source);
            opB = memory->getRegister(reg);
            opA -= opB;
            memory->writeMemory(destiny, opA);
            break;
        case opRR:
            reg = memory->getRegName(source);
            opB = memory->getRegister(reg);
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            opA -= opB;
            memory->setRegister(reg, opA);
            break;
        case opMI:
            opA = memory->readMemory(destiny);
            opB = memory->readMemory(opA);
            opB -= source;
            memory->writeMemory(opA, opB);
            opA = opB;
            break;
        case opRI:
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            opA -= source;
            memory->setRegister(reg, opA);
            break;
        }

        this->updateZFandSF(opA);
    }

    /* ------------------------------------------------------------------------
    * void mul(int16_t source, int16_t operandType)
    * Implements the Simple86's MUL instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void mul(int16_t source, int16_t operandType) {
        int16_t opA, opB;
        Memory::Register reg;
        switch (operandType) {
        case opR:
            reg = memory->getRegName(source);
            opA = memory->getRegister(reg);
            memory->setRegister(memory->Register::AX, memory->getRegister(memory->Register::AX)*opA);
            break;
        case opM:
            opA = memory->readMemory(source);
            opB = memory->getRegister(memory->Register::AL);
            opB *= opA;
            memory->setRegister(memory->Register::AX, opB);
            break;
        }
    }

    /* ------------------------------------------------------------------------
    * void div(int16_t source, int16_t operandType)
    * Implements the Simple86's DIV instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void div(int16_t source, int16_t operandType) {
        int16_t opA;
        Memory::Register reg;
        switch (operandType) {
        case opR:
            reg = memory->getRegName(source);
            opA = memory->getRegister(reg);
            memory->setRegister(memory->Register::AX, memory->Register::AX / opA);
            memory->setRegister(memory->Register::BX, memory->Register::AX%opA);
            break;
        case opM:
            opA = memory->readMemory(source);
            memory->setRegister(memory->Register::AL, memory->Register::AX / opA);
            memory->setRegister(memory->Register::AH, memory->Register::AX%opA);
            break;
        }
    }

    /* ------------------------------------------------------------------------
    * void binaryAnd(int16_t destiny, int16_t source, int16_t operandType)
    * Implements the Simple86's AND instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void binaryAnd(int16_t destiny, int16_t source, int16_t operandType) {
        int16_t opA, opB;
        Memory::Register reg;
        switch (operandType) {
        case opRM:
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            opB = memory->readMemory(source);
            opA &= opB;
            memory->setRegister(reg, opA);
            break;
        case opMR:
            opA = memory->readMemory(destiny);
            reg = memory->getRegName(source);
            opB = memory->getRegister(reg);
            opA &= opB;
            memory->writeMemory(destiny, opA);
            break;
        case opRR:
            reg = memory->getRegName(source);
            opB = memory->getRegister(reg);
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            opA &= opB;
            memory->setRegister(reg, opA);
            break;
        case opMI:
            opA = memory->readMemory(destiny);
            opB = memory->readMemory(opA);
            opB &= source;
            memory->writeMemory(opA, opB);
            opA = opB;
            break;
        case opRI:
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            opA &= source;
            memory->setRegister(reg, opA);
            break;
        }

        this->updateZFandSF(opA);
    }

    /* ------------------------------------------------------------------------
    * void binaryOr(int16_t destiny, int16_t source, int16_t operandType)
    * Implements the Simple86's OR instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void binaryOr(int16_t destiny, int16_t source, int16_t operandType) {
        int16_t opA, opB;
        Memory::Register reg;
        switch (operandType) {
        case opRM:
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            opB = memory->readMemory(source);
            opA |= opB;
            memory->setRegister(reg, opA);
            break;
        case opMR:
            opA = memory->readMemory(destiny);
            reg = memory->getRegName(source);
            opB = memory->getRegister(reg);
            opA |= opB;
            memory->writeMemory(destiny, opA);
            break;
        case opRR:
            reg = memory->getRegName(source);
            opB = memory->getRegister(reg);
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            opA |= opB;
            memory->setRegister(reg, opA);
            break;
        case opMI:
            opA = memory->readMemory(destiny);
            opB = memory->readMemory(opA);
            opB |= source;
            memory->writeMemory(opA, opB);
            opA = opB;
            break;
        case opRI:
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            opA |= source;
            memory->setRegister(reg, opA);
            break;
        }

        this->updateZFandSF(opA);
    }

    /* ------------------------------------------------------------------------
    * void binaryNot(int16_t destiny, int16_t operandType)
    * Implements the Simple86's NOT instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void binaryNot(int16_t destiny, int16_t operandType) {
        int16_t opA, opB;
        Memory::Register reg;
        switch (operandType) {
        case opR:
            reg = memory->getRegName(destiny);
            opA = memory->getRegister(reg);
            memory->setRegister(reg, ~opA);
            break;
        case opM:
            opB = memory->readMemory(destiny);
            opA = ~memory->readMemory(opB);
            memory->writeMemory(opB, opA);
            break;
        }

        this->updateZFandSF(opA);
    }

    /* ------------------------------------------------------------------------
    * void cmp(int16_t source1, int16_t source2, int16_t operandType)
    * Implements the Simple86's CMP instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void cmp(int16_t source1, int16_t source2, int16_t operandType) {
        int16_t opA, opB;
        Memory::Register reg;
        switch (operandType) {
        case opRM:
            reg = memory->getRegName(source1);
            opA = memory->getRegister(reg);
            opB = memory->readMemory(source2);
            opA -= opB;
            break;
        case opMR:
            opA = memory->readMemory(source1);
            reg = memory->getRegName(source2);
            opB = memory->getRegister(reg);
            opA -= opB;
            break;
        case opRR:
            reg = memory->getRegName(source2);
            opB = memory->getRegister(reg);
            reg = memory->getRegName(source1);
            opA = memory->getRegister(reg);
            opA -= opB;
            break;
        case opMI:
            opA = memory->readMemory(source1) - source2;
            break;
        case opRI:
            reg = memory->getRegName(source1);
            opA = memory->getRegister(reg);
            opA -= source2;
            break;
        }

        this->updateZFandSF(opA);
    }

    /* ------------------------------------------------------------------------
    * void jmp(int16_t destiny)
    * Implements the Simple86's JMP instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void jmp(int16_t destiny) {
        memory->setRegister(memory->Register::IP, destiny);
    }

    /* ------------------------------------------------------------------------
    * void jz(int16_t destiny)
    * Implements the Simple86's JZ instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void jz(int16_t destiny) {
        int16_t opA;
        opA = memory->getRegister(memory->Register::ZF);
        if (opA == 1) {
            memory->setRegister(memory->Register::IP, destiny);
        }
    }

    /* ------------------------------------------------------------------------
    * void js(int16_t destiny)
    * Implements the Simple86's JS instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void js(int16_t destiny) {
        int16_t opA;
        opA = memory->getRegister(memory->Register::SF);
        if (opA == 1) {
            memory->setRegister(memory->Register::IP, destiny);
        }
    }

    /* ------------------------------------------------------------------------
    * void call(int16_t destiny)
    * Implements the Simple86's CALL instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void call(int16_t destiny) {
        push(memory->getRegister(Memory::IP), opM);
        jmp(destiny);
    }

    /* ------------------------------------------------------------------------
    * void ret()
    * Implements the Simple86's RET instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void ret() {
        jmp(memory->readMemory(memory->getRegister(Memory::SP)));
        memory->setRegister(memory->Memory::SP, memory->getRegister(Memory::SP) + 1);
    }

    /* ------------------------------------------------------------------------
    * void push(int16_t source, int16_t operandType)
    * Implements the Simple86's PUSH instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void push(int16_t source, int16_t operandType) {
        int16_t opA;
        Memory::Register reg;
        switch (operandType) {
        case opR:
            reg = memory->getRegName(source);
            opA = memory->getRegister(reg);
            break;
        case opM:
            opA = memory->readMemory(source);
            break;
        case opI:
            opA = source;
            break;
        } 
        memory->setRegister(memory->Memory::SP, memory->getRegister(Memory::SP) - 1); //+1?
        memory->writeMemory(memory->getRegister(Memory::SP), opA);
    }

    /* ------------------------------------------------------------------------
    * void pop(int16_t destiny, int16_t operandType)
    * Implements the Simple86's POP instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void pop(int16_t destiny, int16_t operandType) {
        int16_t opA;
        Memory::Register reg;
        opA = memory->readMemory(memory->getRegister(memory->Memory::SP));
        switch (operandType) {
        case opR:
            reg = memory->getRegName(destiny);
            memory->setRegister(reg, opA);
            break;
        case opM:
            memory->writeMemory(destiny, opA);
            break;
        default:
            break;
        }
        memory->setRegister(memory->Memory::SP, memory->getRegister(Memory::SP) + 1);
    }

    /* ------------------------------------------------------------------------
    * void dump()
    * Implements the Simple86's DUMP instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void dump() {
        this->writeToOutput("AX");
        this->writeToOutput("BX");
        this->writeToOutput("CX");
        this->writeToOutput("SP");
        this->writeToOutput("BP");
        this->writeToOutput("IP");
        this->writeToOutput("ZF");
        this->writeToOutput("SF");
        cout << endl;
        this->writeHexToOutput(memory->getRegister(Memory::AX));
        this->writeHexToOutput(memory->getRegister(Memory::BX));
        this->writeHexToOutput(memory->getRegister(Memory::CX));
        this->writeHexToOutput(memory->getRegister(Memory::SP));
        this->writeHexToOutput(memory->getRegister(Memory::BP));
        this->writeHexToOutput(memory->getRegister(Memory::IP));
        this->writeHexToOutput(memory->getRegister(Memory::ZF));
        this->writeHexToOutput(memory->getRegister(Memory::SF));
        cout << endl;
    }

    /* ------------------------------------------------------------------------
    * void read(int16_t destiny, int16_t operandType)
    * Implements the Simple86's READ instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void read(int16_t destiny, int16_t operandType) {
        Memory::Register reg;
        int16_t input;

        cin >> hex >> input;
        if (operandType == opR) {
            reg = memory->getRegName(destiny);
            memory->setRegister(reg, input);
        } else if (operandType == opM) {
            memory->writeMemory(destiny, input);
        }

        this->updateZFandSF(input);
    }

    /* ------------------------------------------------------------------------
    * void write(int16_t source, int16_t operandType)
    * Implements the Simple86's WRITE instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void write(int16_t source, int16_t operandType) {
        Memory::Register reg;
        int16_t value = 0;

        if (operandType == opR) {
            reg = memory->getRegName(source);
            value = memory->getRegister(reg);
        } else if (operandType == opM) {
            value = memory->readMemory(source);
        }
        writeHexToOutput(value);
        cout << endl;
    }

    /* ------------------------------------------------------------------------
    * void halt()
    * Implements the Simple86's HALT instruction.
    * Arguments must be already decoded and passed correctly to this method's
    * parameters (done by the FetchAndDecode module).
    * ------------------------------------------------------------------------ */
    void halt() {
        // Causes FetchAndDecode to halt.
        memory->setRegister(memory->Register::IP, MEMORY_LIMIT + 1);
    }
};

#endif
