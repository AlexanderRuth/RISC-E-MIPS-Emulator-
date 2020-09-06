### RISC-E (Reduced Instruction Set Computer Emulator) MIPS Emulator

Created for the OLC Code Jam 2020 (The Great Machine)

What greater machine is there than the modern CPU? (Probably robots or something...)

## Building

The emulator has been tested on Ubuntu and Windows (Visual Studio 2019)

The build uses CMake, on both Linux and Windows.

To build:

Create a build directory in the repository:

```
mkdir build
``` 

Enter the build directory:

```
cd build/
```

Run cmake from within the directory:

```
cmake ..
```

On Linux, run make install:

```
make install
```

On Windows, open the generated .sln file in Visual Studio, and build the solution. Then, build the INSTALL project.


In either case, a directory named RISC-E will be created with the following contents:
- MIPSEmulator[.exe]:	The executable
- examples/:	A directory containing example MIPS programs

## Running

To use the emulator, run the generated MIPSEmulator exectuble, passing it a filepath to the program to run:

On Linux:
```
./MIPSEmulator examples/primes.txt
```

On Windows:
```
MIPSEmulator.exe examples/primes.txt
```

## Supported Operations

All Arithmetic, Logical, and Branching Instructions are supported.

add, addi, addiu, addu, and, andi, beq, bgtz, blez, bne, div, divu, j, jal, jalr, jr, lb, lbu, lh, lhu, lui, lw, mfhi, mflo, mthi, mtlo, mult, multu, nor, or, ori, sb, sh, sll, sllv, slt, slti, sltiu
, sltu, sra, srav, srl, srlv, sub, subu, sw, syscall, xor, xori

## System Calls

Additionally, the following system calls are provided:

The system call number is provided via $v0:

1:    print integer contained in register $a0

2:    read integer and place result into $a0

3:    print string whose memory address is at $a0

4:    read string of size $a1 and place memory at $a0

5:    read character and store result in $v0

6:    print character in $a0

7:    generate random integer and store in $v0

8:    set RNG seed based on value in $a0

9:    flush input buffer
