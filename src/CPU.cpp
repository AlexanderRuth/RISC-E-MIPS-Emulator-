//
//	CPU.cpp
//
//	Written by Alex Ruth for the 2020 OLC Code Jam
//
//	Defines static members
//

#include "CPU.h"

//
//	Map an instruction name to the corresponding method
//

// R-type instructions that involve 3 registers
std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint32_t&, uint32_t&)>> mips::CPU::RTYPE_3REG_INSTRUCTIONS = {
	{"add", &mips::CPU::add},
	{"sub", &mips::CPU::sub},
	{"addu", &mips::CPU::addu},
	{"subu", &mips::CPU::subu},
	{"and", &mips::CPU::_and},
	{"or", &mips::CPU::_or},
	{"xor", &mips::CPU::_xor},
	{"nor", &mips::CPU::_nor},
	{"slt", &mips::CPU::slt},
	{"sltu", &mips::CPU::sltu},
	{"sllv", &mips::CPU::sllv},
	{"srlv", &mips::CPU::srlv},
	{"srav", &mips::CPU::srav}
};

// R-type instructions that involve 2 registers and a shamt field
std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint32_t&, uint16_t)>> mips::CPU::RTYPE_2REG_SHAMT_INSTRUCTIONS = {
	{"sll", &mips::CPU::sll},
	{"srl", &mips::CPU::srl},
	{"sra", &mips::CPU::sra},
};

// R-type instructions that involve 2 registers
std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint32_t&)>> mips::CPU::RTYPE_2REG_INSTRUCTIONS = {
	{"div", &mips::CPU::div},
	{"divu", &mips::CPU::div},
	{"mult", &mips::CPU::mult},
	{"multu", &mips::CPU::mult},
	{"jalr", &mips::CPU::jalr_2}
};

// R-type instructions that involve 1 register
std::map<std::string, std::function<void(mips::CPU*, uint32_t&)>> mips::CPU::RTYPE_1REG_INSTRUCTIONS = {
	{"mfhi", &mips::CPU::mfhi},
	{"mthi", &mips::CPU::mthi},
	{"mflo", &mips::CPU::mflo},
	{"mtlo", &mips::CPU::mtlo},
	{"jalr", &mips::CPU::jalr_1},
	{"jr", &mips::CPU::jr}
};

// I-type instructions that involve 1 register and an immediate
std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint16_t)>> mips::CPU::ITYPE_1REG_INSTRUCTIONS = {
	{"lui", &mips::CPU::lui},
	{"blez", &mips::CPU::blez},
	{"bgtz", &mips::CPU::bgtz}
};

// I-type instructions that involve 2 registers
std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint32_t&, uint16_t)>> mips::CPU::ITYPE_2REG_INSTRUCTIONS = {
	{"addi", &mips::CPU::addi},
	{"addiu", &mips::CPU::addiu},
	{"ori", &mips::CPU::ori},
	{"andi", &mips::CPU::andi},
	{"xori", &mips::CPU::xori},
	{"beq", &mips::CPU::beq},
	{"bne", &mips::CPU::bne},
	{"slti", &mips::CPU::slti},
	{"sltiu", &mips::CPU::slti}
};

// I-type instructions that access memory
std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint32_t&, uint32_t)>> mips::CPU::ITYPE_MEM_INSTRUCTIONS = {
	{"lw", &mips::CPU::lw},
	{"sw", &mips::CPU::sw},
	{"lh", &mips::CPU::lh},
	{"lhu", &mips::CPU::lhu},
	{"sh", &mips::CPU::sh},
	{"lb", &mips::CPU::lb},
	{"lbu", &mips::CPU::lbu},
	{"sb", &mips::CPU::sb}
};