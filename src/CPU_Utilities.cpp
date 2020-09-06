//
//	CPU_Utilities.cpp
//
//	Written by Alex Ruth for the 2020 OLC Code Jam
//
//	Defines general CPU utility methods
//

#include "CPU.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <limits>
#include <cstring>

// Dump contents of all registers
void mips::CPU::dumpRegisters()
{
	for (int i = 0; i < 32; i++)
	{
		std::cout << "[$" << std::dec << i << "]:\t";
		std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << reg[i];
		std::cout << std::dec << " (u: " << reg[i] << ", s: " << (int32_t)reg[i] << ")" << std::endl;
	}

	std::cout << "[hi]:\t0x" << std::setfill('0') << std::setw(8) << std::hex << hi << std::endl;
	std::cout << "[lo]:\t0x" << std::setfill('0') << std::setw(8) << std::hex << lo << std::endl;
}

// Dump contents of memory
void mips::CPU::dumpMemory()
{
	for (int i = 0; i < MEM_SIZE; i++)
	{
		std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << (int)mem[i];
		std::cout << " (c: " << (char)mem[i] << ")" << std::endl;

	}

	std::cout << (const char*)&mem[4] << std::endl;
}

// Sign extend a 16 bit value
uint32_t mips::CPU::signExtend(uint16_t imm)
{
	int sign_bit = imm & 0x8000;

	if (sign_bit)
		return (uint32_t)(imm | 0xFFFF0000);

	return imm;
}

// Run to completion, or until the max number of cyles is reached (if set)
void mips::CPU::run(int maxCycles)
{
	//Seed based on time. Can be overwritten via system calls
	srand(static_cast<unsigned int>(time(nullptr)));
	memset(reg, 0, 32 * sizeof(uint32_t));

	if (maxCycles < 0)
		maxCycles = std::numeric_limits<int>::max();

	cycles = 0;

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	while (pc < instructions.size() && cycles <= maxCycles)
	{
		_increment_pc = true;

		reg[0] = 0;

		cycles++;
		instructions[pc]();

		if (_increment_pc)
			pc++;
	}

	if (cycles >= maxCycles)
	{
		std::cout << "Maximum number of cycles exceeded. Terminating..." << std::endl;
	}

	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << "##############################################" << std::endl;
	std::cout << "Finished in " << std::dec << cycles << " cycles" << std::endl;
	std::cout << "Excecuted in " << std::dec << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9 << " seconds" << std::endl;
	std::cout << "##############################################" << std::endl;
}
