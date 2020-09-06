//
//	CPU_Logical.cpp
//
//	Written by Alex Ruth for the 2020 OLC Code Jam
//
//	Defines logical instructions
//


#include "CPU.h"

void  mips::CPU::_and(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	rd = rs & rt;
}

void  mips::CPU::_or(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	rd = rs | rt;
}

void mips::CPU::_xor(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	rd = rs ^ rt;
}

void mips::CPU::_nor(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	rd = ~ (rs | rt);
}

void mips::CPU::slt(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	rd = (int32_t)rs < (int32_t)rt ? 1 : 0;
}

void mips::CPU::sltu(uint32_t& rs, uint32_t& rt, uint32_t& rd)
{
	rd = rs < rt ? 1 : 0;
}

void mips::CPU::slti(uint32_t& rs, uint32_t& rt, uint16_t imm)
{
	rt = rs < signExtend(imm) ? 1 : 0;
}

void mips::CPU::lui(uint32_t& rt, uint16_t imm)
{
	rt = (uint32_t)imm << 16;
}

void mips::CPU::ori(uint32_t& rs, uint32_t& rt, uint16_t imm)
{
	rt = rs | imm;
}

void mips::CPU::andi(uint32_t& rs, uint32_t& rt, uint16_t imm)
{
	rt = rs & imm;
}

void mips::CPU::xori(uint32_t& rs, uint32_t& rt, uint16_t imm)
{
	rt = rs ^ imm;
}