//
//	CPU_Memory.cpp
//
//	Written by Alex Ruth for the 2020 OLC Code Jam
//
//	Defines memory I/O operations
//

#include "CPU.h"

void mips::CPU::lw(uint32_t& rt, uint32_t& rs, uint32_t offset)
{
	rt = readMem(rs + offset, MIPS_WORD);
}

void mips::CPU::sw(uint32_t& rt, uint32_t& rs, uint32_t offset)
{
	writeMem(rt, rs + offset, MIPS_WORD);
}

void mips::CPU::lh(uint32_t& rt, uint32_t& rs, uint32_t offset)
{
	rt = signExtend(readMem(rs + offset, MIPS_HALFWORD));
}

void mips::CPU::lhu(uint32_t& rt, uint32_t& rs, uint32_t offset)
{
	rt = readMem(rs + offset, MIPS_HALFWORD);
}


void mips::CPU::sh(uint32_t& rt, uint32_t& rs, uint32_t offset)
{
	writeMem(rt, rs + offset, MIPS_HALFWORD);
}

void mips::CPU::lb(uint32_t& rt, uint32_t& rs, uint32_t offset)
{
	rt = signExtend(readMem(rs + offset, MIPS_BYTE));
}

void mips::CPU::lbu(uint32_t& rt, uint32_t& rs, uint32_t offset)
{
	rt = readMem(rs + offset, MIPS_BYTE);
}

void mips::CPU::sb(uint32_t& rt, uint32_t& rs, uint32_t offset)
{
	writeMem(rt, rs + offset, MIPS_BYTE);
}

void mips::CPU::writeMem(uint32_t word, uint32_t addr, unsigned int size)
{
	//Store word little endian
	for (unsigned int i = 0; i < size; i++)
	{
		if (addr + i >= MEM_SIZE)
		{
			std::stringstream ss;
			ss << "Attempted to access memory address beyond " << MEM_SIZE << ". Exiting...";
			throw std::out_of_range(ss.str());
		}

		mem[addr + i] = (uint8_t)word;
		word = word >> 8;
	}
}

uint32_t mips::CPU::readMem(uint32_t addr, unsigned int size)
{
	uint32_t result = 0;

	//Read word little endian
	for (unsigned int i = 0; i < size; i++)
	{
		if (addr + i >= MEM_SIZE)
		{
			std::stringstream ss;
			ss << "Attempted to access memory address beyond " << MEM_SIZE << ". Exiting...";
			throw std::out_of_range(ss.str());
		}

		result += mem[addr + i] << (8 * i);
	}

	return result;
}