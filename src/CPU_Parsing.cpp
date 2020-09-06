//
//	CPU_Parsing.cpp
//
//	Written by Alex Ruth for the 2020 OLC Code Jam
//
//	Defines methods for parsing the program
//

#include "CPU.h"

#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>

bool mips::CPU::loadProgram(const std::string& filepath)
{
	std::cout << "Loading Program (" << filepath << ")..." << std::endl;

	std::ifstream program(filepath);
	std::string l;
	std::string op;

	int cur_ins = 0;
	int cur_line = -1;

	int data_section_start = -1, data_section_end = -1;
	int text_section_start = -1, text_section_end = -1;

	bool in_text_section = false;

	//Reset mem
	memset(mem, 0, MEM_SIZE);

	try
	{
		while (std::getline(program, l))
		{
			cur_line++;

			//Ignore comments
			l = l.substr(0, l.find_first_of('#'));

			std::istringstream line(l);

			if (!(line >> op))
			{
				//Empty line, skip
				continue;
			}

			//Label
			if (op.back() == ':')
			{
				if (in_text_section)
					_labels[op.substr(0, op.size() - 1)] = cur_ins;

				//See if there is still an operation on this line
				if (!(line >> op))
				{
					//Empty line, skip
					continue;
				}
			}

			if (op == ".data")
			{
				if (data_section_start != -1)
					throw std::logic_error("Cannot have two .data sections");

				data_section_start = cur_line;
			}

			if (op == ".text")
			{
				data_section_end = cur_line;

				if (text_section_start != -1)
					throw std::logic_error("Cannot have two .data sections");
				text_section_start = cur_line;

				in_text_section = true;
			}

			if (in_text_section && op != ".text")
			{
				cur_ins++;
			}
		}

		cur_line = -1;
		cur_ins = 0;
		int mem_offset = 0;

		program.clear();
		program.seekg(0, std::ios::beg);

		while (std::getline(program, l))
		{
			cur_line++;

			//Ignore comments
			l = l.substr(0, l.find_first_of('#'));

			std::istringstream line(l);

			if (!(line >> op))
			{
				//Empty line, skip
				continue;
			}

			//Label
			if (op.back() == ':')
			{
				//If this is the data section, parse the data entry
				if (data_section_start != -1 && cur_line < data_section_end && cur_line >= data_section_start)
				{
					parseDataLine(op.substr(0, op.size() - 1), line, mem_offset);
					continue;
				}

				//Otherwise, in the text section. See if there is still an operation on this line
				if (!(line >> op))
				{
					//Empty line, skip
					continue;
				}
			}

			//Jalr has overloads, so handle it separately
			if (op == "jalr")
			{
				handleJalr(op, line);
			}
			else if (RTYPE_3REG_INSTRUCTIONS.find(op) != RTYPE_3REG_INSTRUCTIONS.end())
			{
				handle3RegInstruction(op, line);
			}
			else if (RTYPE_2REG_INSTRUCTIONS.find(op) != RTYPE_2REG_INSTRUCTIONS.end())
			{
				handle2RegInstruction(op, line);
			}
			else if (RTYPE_2REG_SHAMT_INSTRUCTIONS.find(op) != RTYPE_2REG_SHAMT_INSTRUCTIONS.end())
			{
				handle2RegShamtInstruction(op, line);
			}
			else if (RTYPE_1REG_INSTRUCTIONS.find(op) != RTYPE_1REG_INSTRUCTIONS.end())
			{
				handle1RegInstruction(op, line);
			}
			else if (ITYPE_1REG_INSTRUCTIONS.find(op) != ITYPE_1REG_INSTRUCTIONS.end())
			{
				handle1RegImmediateInstruction(op, line);
			}
			else if (ITYPE_2REG_INSTRUCTIONS.find(op) != ITYPE_2REG_INSTRUCTIONS.end())
			{
				handle2RegImmediateInstruction(op, line);
			}
			else if (ITYPE_MEM_INSTRUCTIONS.find(op) != ITYPE_MEM_INSTRUCTIONS.end())
			{
				handleMemInstruction(op, line);
			}
			else if (op == "syscall")
			{
				handleSyscall();
			}
			else if (op == "j" || op == "jal")
			{
				handleJump(op, line);
			}
			else
			{
				continue;
			}

			cur_ins++;
		}
	}
	catch (std::logic_error err)
	{
		std::cerr << "Parsing error on line " << cur_line+1 << ": " << err.what() << std::endl;
		return false;
	}

	std::cout << "Loaded " << instructions.size() << " instructions" << std::endl;
	std::cout << std::endl;
	pc = 0;

	return true;
}


void mips::CPU::parseDataLine(
	const std::string& label,
	std::istringstream& iss,
	int& mem_offset)
{
	std::string type, value, skip;
	iss >> type;

	_variables[label] = mem_offset;

	//Reading quoted strings retrieved from GitHub: https://stackoverflow.com/questions/33072293/reading-quoted-string-in-c
	if (type == ".asciiz")
	{
		std::getline(std::getline(iss, skip, '"'), value, '"');

		for (unsigned int i = 0; i < value.size() + 1; i++)
		{
			writeMem(value.c_str()[i], mem_offset, MIPS_BYTE);
			mem_offset++;
		}
	}
	else if (type == ".ascii")
	{
		std::getline(std::getline(iss, skip, '"'), value, '"');

		for (unsigned int i = 0; i < value.size(); i++)
		{
			writeMem(value.c_str()[i], mem_offset, MIPS_BYTE);
			mem_offset++;
		}
	}
	else if (type == ".word")
	{
		iss >> value;
		writeMem(std::stoi(value), mem_offset, MIPS_WORD);
		mem_offset += 4;
	}
	else if (type == ".hword")
	{
		iss >> value;
		writeMem(std::stoi(value), mem_offset, MIPS_HALFWORD);
		mem_offset+=2;
	}
	else if (type == ".byte")
	{
		iss >> value;
		writeMem(std::stoi(value), mem_offset, MIPS_BYTE);
		mem_offset++;
	}
	else if (type == ".space")
	{
		iss >> value;
		int padding = std::stoi(value);

		if (padding < 0)
		{
			throw std::logic_error("Cannot have a negative .space");
		}

		mem_offset += padding;
	}
	else
	{
		throw std::logic_error("Unknown .data entry: " + type);
	}
}

void mips::CPU::handle3RegInstruction(
	const std::string& op,
	std::istringstream& iss)
{
	std::vector<std::string> tokens = splitByCommas(iss);

	if (tokens.size() != 3)
	{
		throw std::logic_error("Expected instruction of the format  '" + op + " $rd, $rs, $rt'" + " - Received '" + iss.str() + "'");
	}

	uint32_t& rd = getRegister(tokens[0]);
	uint32_t& rs = getRegister(tokens[1]);
	uint32_t& rt = getRegister(tokens[2]);

	auto opfunc = RTYPE_3REG_INSTRUCTIONS[op];

	instructions.push_back(
		[this, &rs, &rt, &rd, opfunc] { opfunc(this, rs, rt, rd); }
	);
}

void mips::CPU::handle2RegInstruction(
	const std::string& op,
	std::istringstream& iss)
{
	std::vector<std::string> tokens = splitByCommas(iss);

	if (tokens.size() != 2)
	{
		throw std::logic_error("Expected instruction of the format  '" + op + " $rs, $rt'" + " - Received '" + iss.str() + "'");
	}

	uint32_t& rs = getRegister(tokens[0]);
	uint32_t& rt = getRegister(tokens[1]);

	auto opfunc = RTYPE_2REG_INSTRUCTIONS[op];

	instructions.push_back(
		[this, &rs, &rt, opfunc] { opfunc(this, rs, rt); }
	);
}

void mips::CPU::handle2RegShamtInstruction(
	const std::string& op,
	std::istringstream& iss)
{
	std::vector<std::string> tokens = splitByCommas(iss);

	if (tokens.size() != 3)
	{
		throw std::logic_error("Expected instruction of the format  '" + op + " $rd, $rt, shamt'" + " - Received '" + iss.str() + "'");
	}

	uint32_t& rd = getRegister(tokens[0]);
	uint32_t& rt = getRegister(tokens[1]);
	uint16_t shamt = getImmediate(tokens[2]);

	auto opfunc = RTYPE_2REG_SHAMT_INSTRUCTIONS[op];

	instructions.push_back(
		[this, &rt, &rd, shamt, opfunc] { opfunc(this, rd, rt, shamt); }
	);
}

void mips::CPU::handle1RegInstruction(
	const std::string& op,
	std::istringstream& iss)
{
	std::vector<std::string> tokens = splitByCommas(iss);

	if (tokens.size() != 1)
	{
		throw std::logic_error("Expected instruction of the format  '" + op + " $rd'" + " - Received '" + iss.str() + "'");
	}

	uint32_t& rt = getRegister(tokens[0]);

	auto opfunc = RTYPE_1REG_INSTRUCTIONS[op];

	instructions.push_back(
		[this, &rt, opfunc] { opfunc(this, rt); }
	);
}

void mips::CPU::handle1RegImmediateInstruction(
	const std::string& op,
	std::istringstream& iss)
{
	std::vector<std::string> tokens = splitByCommas(iss);

	if (tokens.size() != 2)
	{
		throw std::logic_error("Expected instruction of the format  '" + op + " $rt, imm'" + " - Received '" + iss.str() + "'");
	}

	uint32_t& rt = getRegister(tokens[0]);
	uint16_t imm = getImmediate(tokens[1]);

	auto opfunc = ITYPE_1REG_INSTRUCTIONS[op];

	instructions.push_back(
		[this, &rt, imm, opfunc] { opfunc(this, rt, imm); }
	);

}

void mips::CPU::handle2RegImmediateInstruction(
	const std::string& op,
	std::istringstream& iss)
{
	std::vector<std::string> tokens = splitByCommas(iss);

	if (tokens.size() != 3)
	{
		throw std::logic_error("Expected instruction of the format  '" + op + " $rt, $rs, imm'" + " - Received '" + iss.str() + "'");
	}

	uint32_t& rt = getRegister(tokens[0]);
	uint32_t& rs = getRegister(tokens[1]);
	uint16_t imm = getImmediate(tokens[2]);

	auto opfunc = ITYPE_2REG_INSTRUCTIONS[op];

	instructions.push_back(
		[this, &rt, &rs, imm, opfunc] { opfunc(this, rs, rt, imm); }
	);
}

void mips::CPU::handleMemInstruction(
	const std::string& op, 
	std::istringstream& iss)
{
	std::vector<std::string> tokens = splitByCommas(iss);
	
	if (tokens.size() != 2)
	{
		throw std::logic_error("Expected instruction of the format  '" + op + " $rt, imm($rs)'" + " - Received '" + iss.str() + "'");
	}

	uint32_t& rt = getRegister(tokens[0]);

	auto addr = getAddress(tokens[1]);

	uint16_t imm = addr.first;
	uint32_t& rs = addr.second;

	auto opfunc = ITYPE_MEM_INSTRUCTIONS[op];

	instructions.push_back(
		[this, &rt, &rs, imm, opfunc] { opfunc(this, rt, rs, imm); }
	);
}


void mips::CPU::handleSyscall()
{
	instructions.push_back(
		[this] { this->syscall(); }
	);
}

void mips::CPU::handleJump(const std::string& op, std::istringstream& iss)
{
	std::vector<std::string> tokens = splitByCommas(iss);

	if (tokens.size() != 1)
	{
		throw std::logic_error("Expected instruction of the format  '" + op + " imm'" + " - Received '" + iss.str() + "'");
	}

	uint32_t imm = getImmediate(tokens[0]);

	uint32_t& p = pc;


	if (op == "j")
	{
		instructions.push_back(
			[this, &p, imm] {p = imm >> 2; _increment_pc = false; }
		);
	}
	else if (op == "jal")
	{
		instructions.push_back(
			[this, &p, imm] {getRegister("$ra") = (pc + 1) << 2; p = imm >> 2; _increment_pc = false; }
		);
	}
}

void  mips::CPU::handleJalr(const std::string& op, std::istringstream& iss)
{
	std::vector<std::string> tokens = splitByCommas(iss);

	if (tokens.size() == 1)
	{
		uint32_t& rt = getRegister(tokens[0]);

		auto opfunc = RTYPE_1REG_INSTRUCTIONS[op];

		instructions.push_back(
			[this, &rt, opfunc] { opfunc(this, rt); }
		);
	}
	else if (tokens.size() == 2)
	{
		uint32_t& rs = getRegister(tokens[0]);
		uint32_t& rt = getRegister(tokens[1]);

		auto opfunc = RTYPE_2REG_INSTRUCTIONS[op];

		instructions.push_back(
			[this, &rs, &rt, opfunc] { opfunc(this, rs, rt); }
		);
	}
	else
	{
		throw std::logic_error("Expected instruction of the format  '" + op + " $rd, $rs' or '" + op  + " $rs'" + " - Received '" + iss.str() + "'");
	}

}

uint32_t& mips::CPU::getRegister(std::string r)
{
	if (r.front() != '$')
	{
		throw std::logic_error("Registers should be preceded by a $");
	}

	//Remove the preceding $
	r = r.substr(1);

	if (r == "zero") return reg[0];
	else if (r == "v0") return reg[2];
	else if (r == "v1") return reg[3];
	else if (r == "a0") return reg[4];
	else if (r == "a1") return reg[5];
	else if (r == "a2") return reg[6];
	else if (r == "a3") return reg[7];
	else if (r == "t0") return reg[8];
	else if (r == "t1") return reg[9];
	else if (r == "t2") return reg[10];
	else if (r == "t3") return reg[11];
	else if (r == "t4") return reg[12];
	else if (r == "t5") return reg[13];
	else if (r == "t6") return reg[14];
	else if (r == "t7") return reg[15];
	else if (r == "t8") return reg[24];
	else if (r == "t9") return reg[25];
	else if (r == "s0") return reg[16];
	else if (r == "s1") return reg[17];
	else if (r == "s2") return reg[18];
	else if (r == "s3") return reg[19];
	else if (r == "s4") return reg[20];
	else if (r == "s5") return reg[21];
	else if (r == "s6") return reg[22];
	else if (r == "s7") return reg[23];
	else if (r == "ra") return reg[31];

	int regnum = std::stoi(r);

	if (regnum > 31 || regnum < 0)
	{
		throw std::logic_error("Invalid Register: " + r);
	}

	return reg[regnum];
}

uint16_t mips::CPU::getImmediate(std::string i)
{
	if (_labels.find(i) != _labels.end())
	{
		return _labels[i] << 2;
	}

	if (_variables.find(i) != _variables.end())
	{
		return _variables[i];
	}

	//Use strol to see if valid number as well as convert to int16_t
	char* p;
	int16_t imm = static_cast<int16_t>(strtol(i.c_str(), &p, 10));

	if (*p)
	{
		throw std::logic_error("Immediate '" + i + "' could not be parsed as a number");
	}

	return (uint16_t)imm;
}

std::pair<uint16_t, uint32_t&> mips::CPU::getAddress(std::string a)
{
	//Regex to match imm($rt)
	std::regex addr_regex("\\s*(0x)?\\w+\\s*\\((\\s*\\$.+)\\)\\s*$",
		std::regex_constants::ECMAScript);

	if (!std::regex_match(a, addr_regex))
	{
		throw std::logic_error("Address is not of the form imm($rt)");
	}

	//Get the immediate (either a string or a number)
	std::string immediate = a.substr(0, a.find_first_of('('));

	//Get contents within the parentheses
	std::string reg = a.substr(a.find_first_of('(') + 1, a.find_last_of(')'));
	reg = reg.substr(0, reg.size() - 1);

	return std::pair<uint16_t, uint32_t&>(getImmediate(immediate), getRegister(reg));
}

std::vector<std::string> mips::CPU::splitByCommas(std::istringstream& iss)
{
	std::vector<std::string> tokens;
	std::string token;
	while (iss >> token)
	{
		if (!token.size() || token[0] == ',')
			continue;

		if (token.back() == ',')
			token = token.substr(0, token.size() - 1);

		tokens.push_back(token);
	}

	return tokens;
}
