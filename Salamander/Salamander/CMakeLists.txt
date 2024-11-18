# CMakeList.txt : CMake project for Salamander, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Salamander")

# Add Preprocessor directives
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Add source to this project's executable.
add_executable (Salamander "Compilers.h" "Compilers.c"
	"Reader.h" "Reader.c" "MainReader.c"
	"Scanner.h"		"Scanner.c"		"MainScanner.c"
	# TO_DO: Add A32 (Parser)
	#"Parser.h"		"Parser.c"		"MainParser.c"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Salamander PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
