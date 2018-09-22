cmake_minimum_required (VERSION 2.6)
project (mini_lpp)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_BUILD_TYPE Debug)

add_custom_command (
	OUTPUT parser.hpp parser.cpp parser.output
	COMMAND bison -v -o parser.cpp ${PROJECT_SOURCE_DIR}/parser/parser.y
	MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/parser/parser.y
)

add_custom_command (
	OUTPUT lexer.cpp
	COMMAND flex -o lexer.cpp ${PROJECT_SOURCE_DIR}/lexer/lexer.l
	MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/lexer/lexer.l
)

include_directories (${PROJECT_SOURCE_DIR}/ast)
include_directories (${PROJECT_SOURCE_DIR}/parser)
include_directories (${PROJECT_SOURCE_DIR}/lexer)
include_directories (${CMAKE_CURRENT_BINARY_DIR})

add_executable (
	mini_lpp.elf
	ast/nodes_to_string.cpp
	ast/nodes_semantic_analysis.cpp
	parser.cpp
	lexer.cpp
	main.cpp
)
