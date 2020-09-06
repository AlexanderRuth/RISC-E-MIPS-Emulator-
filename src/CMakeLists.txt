add_library(
	mips_cpu STATIC
	CPU.cpp
	CPU_Arithmetic.cpp CPU_Control.cpp
	CPU_Logical.cpp CPU_Memory.cpp
	CPU_Parsing CPU_Utilities.cpp
)

target_include_directories(mips_cpu PUBLIC ../include)
