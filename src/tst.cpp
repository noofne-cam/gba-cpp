#include <iostream>
#include <cstdint>
#include <array>
#include <memory>

#include "mode.h"

using std::size_t;

class Bus
{

};


class Registers
{
    public:
        std::uint32_t read(size_t rindex)
        {
            size_t gindex = index_general(rindex);
            return general.at(gindex); 
        }
        void write(size_t rindex, std::uint32_t value)
        {
            size_t gindex = index_general(rindex);
            general.at(gindex) = value;
        }

        std::uint32_t read_pc()
        {
            return pc;
        }
        void write_pc(std::uint32_t value)
        {
            pc = value;
        }

        // cpsr-style reads/writes

    private:
        // pc-last 
        std::array<uint32_t, 30> general;
        std::uint32_t pc;
        std::array<uint32_t, 5> spsr;
        std::uint32_t cpsr;

        size_t index_general(size_t rindex)
        {
            std::array<size_t, 15> active;

            switch(*mode)
            {
                case SYS: active = { 0, 1, 2, 3, 4, 5, 6, 7,  8,  9, 10, 11, 12, 13, 14 };
                case USR: active = { 0, 1, 2, 3, 4, 5, 6, 7,  8,  9, 10, 11, 12, 13, 14 };
                case FIQ: active = { 0, 1, 2, 3, 4, 5, 6, 7, 15, 16, 17, 18, 19, 20, 21 };
                case SVC: active = { 0, 1, 2, 3, 4, 5, 6, 7,  8,  9, 10, 11, 12, 22, 23 };
                case ABT: active = { 0, 1, 2, 3, 4, 5, 6, 7,  8,  9, 10, 11, 12, 24, 25 };
                case IRQ: active = { 0, 1, 2, 3, 4, 5, 6, 7,  8,  9, 10, 11, 12, 26, 27 };
                case UND: active = { 0, 1, 2, 3, 4, 5, 6, 7,  8,  9, 10, 11, 12, 28, 29 };
            }

            return active.at(rindex);
        }

        // latches to core, set in constructor
        std::shared_ptr<Mode> mode; 
};

class Core
{
    public:
        Bus bus;
        Registers reg;

        bool nMREQ;
        bool SEQ;

        void tick()
        {
            // cycle differentiation
            switch ((uint8_t)(nMREQ << 1) | SEQ)
            {
                case 0b00: break; // non-sequential 
                case 0b01: break; // sequential
                case 0b10: break; // internal
                case 0b11: break; // coprocessor reg transfer
            }
        }

        // cycle types

    private:

};