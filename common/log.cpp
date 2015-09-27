#ifdef _WIN32
	#include <conio.h>
	#include <windows.h>
#endif
#include <cstdio>
#include <stdarg.h>
#include "log.h"

// Basic colors
typedef enum {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY,
	DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
} COLORS;

// Our base colors
static int __BACKGROUND = BLACK;
static int __FOREGROUND = LIGHTGRAY;

// Change console text color
void textcolor(int color) {
#ifdef _WIN32
	__FOREGROUND = color;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
	                        color + (__BACKGROUND << 4));
#else
	(void) color;
	(void) __BACKGROUND;
	(void) __FOREGROUND;
#endif
}

// This function logs based on flags
void Log(enum msg_type flag, const char *Format, ...) {
	va_list ap;	      // For arguments
	va_start(ap, Format);

	switch (flag) {
		case msg_type::MSG_QUERY:
		case msg_type::MSG_SDEBUG:
			break; // QIX: these weren't handled to begin with.

		case msg_type::MSG_NONE: // direct printf replacement
			textcolor(WHITE);
			vprintf(Format, ap);
			break;

		case msg_type::MSG_STATUS:
			textcolor(GREEN);
			printf("[STATUS]: ");
			break;

		case msg_type::MSG_SQL:
			textcolor(CYAN);
			printf("[SQL]: ");
			break;

		case msg_type::MSG_INFO:
			textcolor(LIGHTGREEN);
			printf("[INFO]: ");
			break;

		case msg_type::MSG_NOTICE:
			textcolor(LIGHTCYAN);
			printf("[NOTICE]: ");
			break;

		case msg_type::MSG_WARNING:
			textcolor(YELLOW);
			printf("[WARNING]: ");
			break;

		case msg_type::MSG_DEBUG:
			textcolor(LIGHTBLUE);
			printf("[DEBUG]: ");
			break;

		case msg_type::MSG_ERROR:
			textcolor(RED);
			printf("[ERROR]: ");
			break;

		case msg_type::MSG_FATALERROR:
			textcolor(LIGHTRED);
			printf("[FATAL ERROR]: ");
			break;

		case msg_type::MSG_HACK:
			textcolor(LIGHTRED);
			printf("[HACK]: ");
			break;

		case msg_type::MSG_LOAD:
			textcolor(BROWN);
			printf("[LOADING]: ");
			break;

		case msg_type::MSG_GMACTION:
			textcolor(MAGENTA);
			printf("[GM ACTION]: ");
			break;

		case msg_type::MSG_START:
			textcolor(MAGENTA);
			vprintf(Format, ap);
			printf("\r\n");
			break;
	}

	textcolor(LIGHTGRAY);

	if (flag != msg_type::MSG_QUERY) {
		vprintf(Format, ap);
		printf((flag == msg_type::MSG_LOAD) ? "\r" : "\n");
	}

	FILE *fh;

	switch (LOG_THISSERVER) {
		case LOG_LOGIN_SERVER:
			fh = fopen(LOG_DIRECTORY LOG_FILENAME_LOGINFILE, "a+");
			break;

		case LOG_CHARACTER_SERVER:
			fh = fopen(LOG_DIRECTORY LOG_FILENAME_CHARFILE, "a+");
			break;

		case LOG_WORLD_SERVER:
			fh = fopen(LOG_DIRECTORY LOG_FILENAME_WORLDFILE, "a+");
			break;

		case LOG_SAME_FILE:
			fh = fopen(LOG_DIRECTORY LOG_DEFAULT_FILE, "a+");
			break;
	}

	if (flag == msg_type::MSG_QUERY) {
		if (fh != NULL) {
			fclose(fh);
		}

		fh = fopen(LOG_DIRECTORY "queries.txt", "a+");
	}

	if (fh != NULL) {
		vfprintf(fh, Format, ap);
		fputc('\n', fh);
		fclose(fh);
	}

	va_end(ap);
}
