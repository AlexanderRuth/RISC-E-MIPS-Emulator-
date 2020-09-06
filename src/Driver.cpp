//
//	Driver.cpp
//
//	Written by Alex Ruth for the 2020 OLC Code Jam
//
//	Driver for MIPS Emulator
//

#include "CPU.h"

#include <iostream>

int main(int argc, char* argv[])
{
	mips::CPU myCPU;

	std::cout << std::endl;
	std::cout << "##############################################" << std::endl;
	std::cout << "        RISC-E MIPS Interpreter (v1.0)        " << std::endl;
	std::cout << "Written by Alex Ruth for the 2020 OLC Code Jam" << std::endl;
	std::cout << "##############################################" << std::endl;
	std::cout << std::endl;

	if (argc != 2)
	{
		std::cerr << "Expected one argument. Exiting..." << std::endl;
		std::cerr << "To run: MIPSEmulator.exe <input_file>" << std::endl;
	}

	if(myCPU.loadProgram(std::string(argv[1])))
	{
		myCPU.run();
		return 0;
	}

	return 1;
}
