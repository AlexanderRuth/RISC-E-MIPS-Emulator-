//
//	CPU.h
//
//	Written by Alex Ruth for the 2020 OLC Code Jam
//
//	Header file for MIPS CPU
//

#ifndef MIPS_CPU_H
#define MIPS_CPU_h

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <functional>

#define MIPS_BYTE 1
#define MIPS_HALFWORD 2
#define MIPS_WORD 4
#define MEM_SIZE 2048	//2048 bytes of memory

namespace mips
{

	class CPU
	{
	protected:

		typedef std::function<void()> instruction;

		uint8_t mem[MEM_SIZE];	//2K byte-addressable memory
		uint32_t reg[32];	//32 bit registers

		// Vector of instructions in the program, ordered by location in memory
		// e.g., the 3rd instruction corresponds to index 2 (PC == 2)
		std::vector<instruction> instructions;

		uint32_t hi;		//HI register
		uint32_t lo;		//LO register
		uint32_t pc;		//Program counter

		int cycles;			//Count of cycles to execute program

		std::map<std::string, uint32_t> _labels;		//Labels to instruction locations
		std::map<std::string, uint32_t> _variables;		//Variables defined in .data

		bool _increment_pc;								//Increment the program counter this cycle?

		// Arithmetic Instructions
		void add(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void sub(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void addi(uint32_t& rs, uint32_t& rt, uint16_t imm);
		void addu(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void subu(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void addiu(uint32_t& rs, uint32_t& rt, uint16_t imm);
		void div(uint32_t& rs, uint32_t& rt);
		void mult(uint32_t& rs, uint32_t& rt);

		// Move to/from HI and LO
		void mfhi(uint32_t& rd);
		void mthi(uint32_t& rs);
		void mflo(uint32_t& rd);
		void mtlo(uint32_t& rs);

		// Logical instructions
		void _and(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void _or(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void _xor(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void _nor(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void slt(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void sltu(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void ori(uint32_t& rs, uint32_t& rt, uint16_t imm);
		void andi(uint32_t& rs, uint32_t& rt, uint16_t imm);
		void xori(uint32_t& rs, uint32_t& rt, uint16_t imm);
		void slti(uint32_t& rs, uint32_t& rt, uint16_t imm);
		void lui(uint32_t& rt, uint16_t imm);

		// Shift instructions
		void sllv(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void srlv(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void srav(uint32_t& rs, uint32_t& rt, uint32_t& rd);
		void sll(uint32_t& rd, uint32_t rt, uint16_t shamt);
		void srl(uint32_t& rd, uint32_t rt, uint16_t shamt);
		void sra(uint32_t& rd, uint32_t rt, uint16_t shamt);

		// Control instructions
		void blez(uint32_t& rs, uint16_t imm);
		void bgtz(uint32_t& rs, uint16_t imm);
		void bne(uint32_t& rs, uint32_t& rt, uint16_t imm);
		void beq(uint32_t& rs, uint32_t& rt, uint16_t imm);
		void jalr_1(uint32_t& rs);
		void jalr_2(uint32_t& rd, uint32_t& rs);
		void jr(uint32_t& rs);

		// Read/Write memory instructions
		void lw(uint32_t& rs, uint32_t& rt, uint32_t addr);
		void sw(uint32_t& rs, uint32_t& rt, uint32_t addr);
		void lh(uint32_t& rs, uint32_t& rt, uint32_t addr);
		void lhu(uint32_t& rs, uint32_t& rt, uint32_t addr);
		void sh(uint32_t& rs, uint32_t& rt, uint32_t addr);
		void lb(uint32_t& rs, uint32_t& rt, uint32_t addr);
		void lbu(uint32_t& rs, uint32_t& rt, uint32_t addr);
		void sb(uint32_t& rs, uint32_t& rt, uint32_t addr);

		// Handle a system call
		void syscall();

		//Parse a line in the .data section
		void parseDataLine(const std::string& label, std::istringstream& iss, int& mem_offset);

		// Handle all different types of instructions while parsing
		void handle3RegInstruction(const std::string& op, std::istringstream& iss);
		void handle2RegInstruction(const std::string& op, std::istringstream& iss);
		void handle2RegShamtInstruction(const std::string& op, std::istringstream& iss);
		void handle1RegInstruction(const std::string& op, std::istringstream& iss);
		void handle1RegImmediateInstruction(const std::string& op, std::istringstream& iss);
		void handle2RegImmediateInstruction(const std::string& op, std::istringstream& iss);
		void handleMemInstruction(const std::string& op, std::istringstream& iss);
		void handleSyscall();
		void handleJump(const std::string& op, std::istringstream& iss);
		void handleJalr(const std::string& op, std::istringstream& iss);

		// Split a line by commas. Used to separate arguments to an operation. Returns a vector of strings
		std::vector<std::string> splitByCommas(std::istringstream& iss);

		// System call handlers
		void readInteger();
		void printInteger(const uint32_t& val);
		void readString(const uint32_t& addr, const uint32_t& size);
		void printString(const uint32_t& addr);
		void readChar();
		void printChar(const uint32_t& c);
		void randomInteger();
		void setSeed(const uint32_t& s);
		void flush();

		// Sign extend a 16 bit value
		uint32_t signExtend(uint16_t imm);

		// Get reference to a register
		uint32_t& getRegister(std::string r);

		// Get immediate value (either a literal, or a label/variable)
		uint16_t getImmediate(std::string i);

		// Get the immediate and offset from an address of the form "imm(register)"
		std::pair<uint16_t, uint32_t&> getAddress(std::string a);

		// Write "size" bytes to memory. Size should be <= 4
		void writeMem(uint32_t word, uint32_t addr, unsigned int size);

		// read "size" bytes from memory. Size should be <= 4
		uint32_t readMem(uint32_t addr, unsigned int size);

	public:

		// Load a program located at the provided filepath
		bool loadProgram(const std::string& filepath);

		// Run to completion, or until the max number of cyles is reached (if set)
		void run(int maxCycles = -1);

		// Dump contents of registers
		void dumpRegisters();

		// Dump contents of memory
		void dumpMemory();

	protected:

		//Maps from an opcode to the corresponding C++ function

		//R-type instructions
		static std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint32_t&, uint32_t&)>> RTYPE_3REG_INSTRUCTIONS;
		static std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint32_t&)>> RTYPE_2REG_INSTRUCTIONS;
		static std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint32_t&, uint16_t)>> RTYPE_2REG_SHAMT_INSTRUCTIONS;
		static std::map<std::string, std::function<void(mips::CPU*, uint32_t&)>> RTYPE_1REG_INSTRUCTIONS;

		//I-type instructions
		static std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint16_t)>> ITYPE_1REG_INSTRUCTIONS;
		static std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint32_t&, uint16_t)>> ITYPE_2REG_INSTRUCTIONS;
		static std::map<std::string, std::function<void(mips::CPU*, uint32_t&, uint32_t&, uint32_t)>> ITYPE_MEM_INSTRUCTIONS;
	};
}

#endif
