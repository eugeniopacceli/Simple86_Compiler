/* Simple86_Emulator Memory
*
* Memory module for a Simple86 machine, implemented
* according to the specifications.
*
*/
#ifndef SIMULA_MEMORY
#define SIMULA_MEMORY 1

#include<cstdint>
#define LOW_MASK  0b0000000011111111
#define HIGH_MASK 0b1111111100000000
#define MEMORY_LIMIT 1000

// Memory module for Simple86
class Memory {

    // The machine registers, and memory.
    // Access is controlled by the class's public methods.
private:
    int16_t regAX;
    int16_t regBX;
    int16_t regCX;
    int16_t regBP;
    int16_t regSP;
    int16_t regIP;
    int16_t regZF;
    int16_t regSF;
    int16_t MEM[MEMORY_LIMIT];

public:
    // Keywords to access each one of the machine's registers.
    // Pass those to the public methods controlling the registers access.
    enum Register { AX, AL, AH, BX, BL, BH, CX, CL, CH, BP, SP, IP, ZF, SF };

    // Initializes a Memory object, with the initial state specified in the Simple86 description.
    Memory() {
        this->regBP = MEMORY_LIMIT;
        this->regSP = MEMORY_LIMIT;
        this->regIP = 0;
    }

    /* ------------------------------------------------------------------------
    * Register getRegName(int16_t address)
    * Given a binary address code refering to a register, returns the register's
    * keyword to be used with the public methods, so the caller can manipulate
    * the referenced register.
    * ------------------------------------------------------------------------ */
    Register getRegName(int16_t address) {
        switch (address) {
        case 0: return this->AL;
        case 1: return this->AH;
        case 2: return this->AX;
        case 3: return this->BH;
        case 4: return this->BL;
        case 5: return this->BX;
        case 6: return this->CL;
        case 7: return this->CH;
        case 8: return this->CX;
        default: return this->ZF;
        }
    }

    /* ------------------------------------------------------------------------
    * int16_t getRegister(Register reg)
    * Returns the value of the register mapped to the passed keyword.
    * ------------------------------------------------------------------------ */
    int16_t getRegister(Register reg) {
        switch (reg) {
        case Register::AX: return this->regAX;
        case Register::AL: return (this->regAX & LOW_MASK);
        case Register::AH: return (this->regAX >> 8);
        case Register::BX: return this->regBX;
        case Register::BL: return (this->regBX & LOW_MASK);
        case Register::BH: return (this->regBX >> 8);
        case Register::CX: return this->regCX;
        case Register::CL: return (this->regCX & LOW_MASK);
        case Register::CH: return (this->regCX >> 8);
        case Register::BP: return this->regBP;
        case Register::SP: return this->regSP;
        case Register::IP: return this->regIP;
        case Register::ZF: return this->regZF;
        case Register::SF: return this->regSF;
        }

        return 0;
    }

    /* ------------------------------------------------------------------------
    * int16_t setRegister(Register reg, int16_t newValue)
    * Modifies the value of the register mapped to the passed keyword, and
    * returns it's new value (return should be equal to newValue, or else, it is an
    * error).
    * ------------------------------------------------------------------------ */
    int16_t setRegister(Register reg, int16_t newValue) {
        switch (reg) {
        case Register::AX: this->regAX = newValue; return this->regAX;
        case Register::BX: this->regBX = newValue; return this->regBP;
        case Register::CX: this->regCX = newValue; return this->regCX;
        case Register::BP: this->regBP = newValue; return this->regBP;
        case Register::SP: this->regSP = newValue; return this->regSP;
        case Register::IP: this->regIP = newValue; return this->regIP;
        case Register::ZF: this->regZF = newValue; return this->regZF;
        case Register::SF: this->regSF = newValue; return this->regSF;

        case Register::AL: this->regAX = (this->regAX & HIGH_MASK) + newValue; return (this->regAX & LOW_MASK);
        case Register::BL: this->regBX = (this->regBX & HIGH_MASK) + newValue; return (this->regBX & LOW_MASK);
        case Register::CL: this->regCX = (this->regCX & HIGH_MASK) + newValue; return (this->regCX & LOW_MASK);

        case Register::AH: this->regAX = (this->regAX & LOW_MASK) + (newValue << 8); return this->regAX >> 8;
        case Register::BH: this->regBX = (this->regBX & LOW_MASK) + (newValue << 8); return this->regBX >> 8;
        case Register::CH: this->regCX = (this->regCX & LOW_MASK) + (newValue << 8); return this->regCX >> 8;
        }

        return 0;
    }

    /* ------------------------------------------------------------------------
    * int16_t readMemory(int16_t source)
    * Reads the memory position specified by the source parameter, and returns it's
    * value.
    * ------------------------------------------------------------------------ */
    int16_t readMemory(int16_t source) {
        return this->MEM[source];
    }

    /* ------------------------------------------------------------------------
    * int16_t writeMemory(int16_t destination, int16_t newValue)
    * Writes to the memory position specified by the destination parameter, and
    * returns it's new value (return should be equal to newValue).
    * ------------------------------------------------------------------------ */
    int16_t writeMemory(int16_t destination, int16_t newValue) {
        this->MEM[destination] = newValue;
        return this->MEM[destination];
    }
};

#endif
