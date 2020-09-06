//
//	CPU_Control.cpp
//
//	Written by Alex Ruth for the 2020 OLC Code Jam
//
//	Defines operations that modify control flow
//

#include "CPU.h"

#include <iostream>

void mips::CPU::bgtz(uint32_t& rs, uint16_t imm)
{
	if ((int32_t)rs > 0)
	{
		pc = imm >> 2;
		_increment_pc = false;
	}
}

void mips::CPU::blez(uint32_t& rs, uint16_t imm)
{
	if ((int32_t)rs <= 0)
	{
		pc = imm >> 2;
		_increment_pc = false;
	}
}

void mips::CPU::beq(uint32_t& rs, uint32_t& rt, uint16_t imm)
{
	if (rs == rt)
	{
		pc = imm >> 2;
		_increment_pc = false;
	}
}

void mips::CPU::bne(uint32_t& rs, uint32_t& rt, uint16_t imm)
{
	if (rs != rt)
	{
		pc = imm >> 2;
		_increment_pc = false;
	}
}

void mips::CPU::jr(uint32_t& rs)
{
	//Set pc
	pc = (rs >> 2);

	//Don't increment the program counter
	_increment_pc = false;
}

void mips::CPU::jalr_2(uint32_t& rd, uint32_t& rs)
{
	rd = (pc + 1) << 2;

	//Set PC (shift right by 2, since instructions are at multiples of 4)
	pc = (rs >> 2);

	//Don't increment the program counter
	_increment_pc = false;
}

void mips::CPU::jalr_1(uint32_t& rs)
{
	jalr_2(getRegister("$ra"), rs);
}

//
//	System Calls:
//		$v0 == 1:	print integer in $a0
//			   2:	read integer and place result into $v0
//             3:   print string whose memory address is $a0
//             4:   read string of size $a1 and place memory at $a0
//             5:   read character and store result in $v0
//             6:   print character in $a0
//             7:   generate random integer and store in $v0
//             8:   set RNG seed based on value in $a0
//             9:   flush input buffer
//
void mips::CPU::syscall()
{
	uint32_t syscall_num = getRegister("$v0");

	switch (syscall_num)
	{
	case 1:
		printInteger(getRegister("$a0"));
		break;
	case 2:
		readInteger();
		break;
	case 3:
		printString(getRegister("$a0"));
		break;
	case 4:
		readString(getRegister("$a0"), getRegister("$a1"));
		break;
	case 5:
		readChar();
		break;
	case 6:
		printChar(getRegister("$a0"));
		break;
	case 7:
		randomInteger();
		break;
	case 8:
		setSeed(getRegister("$a0"));
		break;
	case 9:
		flush();
		break;
	default:
		//Unknown system call
		break;
	}
}


void mips::CPU::printInteger(const uint32_t& val)
{
	std::cout << (int32_t)val << std::endl;
}

void mips::CPU::printString(const uint32_t& addr)
{
	std::cout << (char*)&mem[addr];
}

void mips::CPU::readInteger()
{
	std::cin >> getRegister("$v0");
}

void mips::CPU::readString(const uint32_t& addr, const uint32_t& size)
{
	std::string input;

	std::getline(std::cin, input);
	if (input.size() > size - 1)
	{
		input = input.substr(0, size - 1);
	}

	//Write the result to memory
	for (unsigned int i = 0; i < input.size(); i++)
	{
		writeMem(input[i], addr + i, MIPS_BYTE);
	}

	//Null terminate
	writeMem('\0', addr + input.size(), MIPS_BYTE);
}

void mips::CPU::readChar()
{
	getRegister("$v0") = getchar();
}

void mips::CPU::printChar(const uint32_t& c)
{
	std::cout << (char)c;
}

void mips::CPU::randomInteger()
{
	getRegister("$v0") = rand();
}

void mips::CPU::setSeed(const uint32_t& s)
{
	srand(s);
}

void mips::CPU::flush()
{
	std::cin.clear();
	fflush(stdin);
}
