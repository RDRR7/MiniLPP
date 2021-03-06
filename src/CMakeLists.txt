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
include_directories (${PROJECT_SOURCE_DIR}/code_handler)
include_directories (${CMAKE_CURRENT_BINARY_DIR})

add_executable (
	mini_lpp.elf
	ast/nodes_to_string.cpp
	ast/nodes_semantic_analysis.cpp
	ast/nodes_generate_code.cpp
	parser.cpp
	lexer.cpp
	code_handler/function_handler.cpp
	code_handler/code_handler.cpp
	main.cpp
)

add_custom_command (
	OUTPUT liblppf.so
	COMMAND g++ -std=c++11 -m32 -fPIC ${PROJECT_SOURCE_DIR}/runtime/runtime.cpp -shared -o liblppf.so -lncurses
	DEPENDS ${PROJECT_SOURCE_DIR}/runtime/runtime.hpp
	MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/runtime/runtime.cpp
)

add_custom_target(compile_runtime DEPENDS liblppf.so)
