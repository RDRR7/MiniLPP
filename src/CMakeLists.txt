cmake_minimum_required (VERSION 2.6)
project (mini_lpp)

set (CMAKE_CXX_STANDARD 11)

add_custom_command (
	OUTPUT lexer.cpp
	COMMAND flex -o lexer.cpp ${PROJECT_SOURCE_DIR}/lexer/lexer.l
	MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/lexer/lexer.l
)

include_directories (${PROJECT_SOURCE_DIR})

add_executable (
	mini_lpp.elf
	lexer.cpp
	main.cpp
)
