//
//	CPU_Arithmetic.cpp
//
//	Written by Alex Ruth for the 2020 OLC Code Jam
//
//	Defines arithmetic and shifting operations
//

#include "CPU.h"

void mips::CPU::add(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	addu(rs, rt, rd);

	//Exception on signed overflow
}

void mips::CPU::sub(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	subu(rs, rt, rd);

	//Exception on signed overflow
}

void mips::CPU::addi(uint32_t& rs, uint32_t& rt, uint16_t imm)
{
	addiu(rs, rt, imm);

	//Exception on signed overflow
}

void mips::CPU::addu(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	rd = rt + rs;
}

void mips::CPU::subu(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	rd = rs - rt;
}

void mips::CPU::addiu(uint32_t& rs, uint32_t& rt, uint16_t imm)
{
	rt = rs + signExtend(imm);
}

void mips::CPU::div(uint32_t& rs, uint32_t& rt)
{
	lo = rs / rt;
	hi = rs % rt;
}

void mips::CPU::mult(uint32_t& rs, uint32_t& rt)
{
	uint64_t res = (uint64_t) rs * (uint64_t) rt;
	hi = static_cast<uint32_t>(res >> 32);
	lo = res & 0x00000000FFFFFFFF;
}

void mips::CPU::sll(uint32_t& rd, uint32_t rt, uint16_t shamt)
{
	rd = rt << shamt;
}

void mips::CPU::srl(uint32_t& rd, uint32_t rt, uint16_t shamt)
{
	rd = rt >> shamt;
}

void mips::CPU::sra(uint32_t& rd, uint32_t rt, uint16_t shamt)
{
	//Get the sign bit
	uint32_t sign = rt & 0x80000000;
	uint32_t mask = 0;

	//Logical shift right
	srl(rd, rt, shamt);

	for (int i = 0; i < shamt; i++)
	{
		mask += sign >> i;
	}

	//Fill in the leftmost zeros with the sign bit
	rd |= mask;
}

void mips::CPU::sllv(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	rd = rt << rs;
}

void mips::CPU::srlv(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	rd = rt >> rs;
}

void mips::CPU::srav(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	//Get the sign bit
	uint32_t sign = rt & 0x80000000;
	uint32_t mask = 0;

	//Logical shift right
	rd = rt >> rs;

	for (unsigned int i = 0; i < rs; i++)
	{
		mask += sign >> i;
	}

	//Fill in the leftmost zeros with the sign bit
	rd |= mask;
}

void mips::CPU::mfhi(uint32_t& rd)
{
	rd = hi;
}

void mips::CPU::mthi(uint32_t& rs)
{
	hi = rs;
}

void mips::CPU::mflo(uint32_t& rd)
{
	rd = lo;
}

void mips::CPU::mtlo(uint32_t& rs)
{
	lo = rs;
}