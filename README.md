###RISC-E (Reduced Instruction Set Computer Emulator) MIPS Emulator

Created for the OLC Code Jam 2020

##Building

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
- MIPSEmulator[.exe]
	The executable
- examples/
	A directory containing example MIPS programs

##Running

To use the emulator, run the generated MIPSEmulator exectuble, passing it a filepath to the program to run:

On Linux:
```
./MIPSEmulator examples/primes.txt
```

On Windows:
```
MIPSEmulator.exe examples/primes.txt
```
