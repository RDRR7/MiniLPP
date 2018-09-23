#include "runtime.hpp"
#include <ncurses.h>
#include <unordered_map>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <chrono>
#include <thread>

int current_background_color;
int current_text_color;

std::unordered_map<int, int> colors =
	{{0, COLOR_BLACK},
	 {1, COLOR_BLUE},
	 {2, COLOR_GREEN},
	 {3, COLOR_CYAN},
	 {4, COLOR_RED},
	 {5, COLOR_MAGENTA},
	 {6, COLOR_YELLOW},
	 {7, COLOR_WHITE},
	 {8, COLOR_WHITE},
	 {9, COLOR_BLUE},
	 {10, COLOR_GREEN},
	 {11, COLOR_BLUE},
	 {12, COLOR_RED},
	 {13, COLOR_MAGENTA},
	 {14, COLOR_YELLOW},
	 {15, COLOR_WHITE}};

void init_screen()
{
	initscr();
	current_background_color = COLOR_BLACK;
	current_text_color = COLOR_WHITE;
}

void end_screen()
{
	endwin();
}

void limpiar_pantalla()
{
	clear();
}

void posicionar_cursor(int line, int column)
{
	move(line, column);
}

void color_fondo(int color)
{
	while (color >= 16)
	{
		color -= 16;
	}
	current_background_color = color;
	init_pair(1,
			  colors[current_text_color],
			  colors[current_background_color]);
}

void color_texto(int color)
{
	while (color >= 16)
	{
		color -= 16;
	}
	current_text_color = color;
	init_pair(1,
			  colors[current_text_color],
			  colors[current_background_color]);
}

char caracter_ascii(int ascii_value)
{
	return static_cast<char>(ascii_value);
}

int valor_ascii(char character)
{
	return static_cast<int>(character);
}

char obtener_caracter()
{
	return getch();
}

int obtener_tecla()
{
	return static_cast<int>(getch());
}

void nueva_linea()
{
	printw("\n");
}

int aleatorio()
{
	return std::rand();
}

void inicializar_aleatorio()
{
	std::srand(std::time(0));
}

bool tecla_presionada()
{
	return true;
}

void pausa(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int read_int()
{
	int line, column;
	getyx(stdscr, line, column);

	char *number = new char[11];
	getnstr(number, 10);

	int len = strlen(number);
	move(line, column + len);

	return atoi(number);
}

char read_char()
{
	return getch();
}

bool read_bool()
{
	int line, column;
	getyx(stdscr, line, column);

	char *boolean = new char[13];
	getnstr(boolean, 12);

	int len = strlen(boolean);
	move(line, column + len);

	return atoi(boolean) == 1;
}

void print(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	int index = 0;
	while (fmt[index] != '\0')
	{
		if (fmt[index] == '%' && fmt[index + 1] == 'd')
		{
			int i = va_arg(args, int);
			printw("%d", i);
			index++;
		}
		else if (fmt[index] == '%' && fmt[index + 1] == 'c')
		{
			int c = va_arg(args, int);
			printw("%c", (char)c);
			index++;
		}
		else if (fmt[index] == '%' && fmt[index + 1] == 'd')
		{
			int b = va_arg(args, int);
			printw("%d", (bool)b);
			index++;
		}
		else
		{
			printw("%c", fmt[index]);
		}
		index++;
	}

	va_end(args);
}
