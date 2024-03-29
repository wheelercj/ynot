﻿#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <csignal>  // raise, SIGINT
#ifdef _WIN32
#include <windows.h>
#include <conio.h>  // _getch
#include <io.h>  // _setmode
#include <fcntl.h>  // _O_U16TEXT
#else
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <locale>
#include <codecvt>  // wstring_convert, codecvt_utf8
#endif

// Here are some resources that were helpful for writing this file:
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
// https://docs.microsoft.com/en-us/windows/console/reading-input-buffer-events
// https://docs.microsoft.com/en-us/windows/console/mouse-event-record-str
// https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#main
// http://xn--rpa.cc/irl/term.html
// https://docs.microsoft.com/en-us/windows/console/writeconsole

namespace ynot
{

	struct Coord
	{
		int x;
		int y;
		Coord()
		{
			this->x = 0;
			this->y = 0;
		}
		Coord(int new_x, int new_y)
		{
			this->x = new_x;
			this->y = new_y;
		}
		bool operator==(const Coord& other)
		{
			return this->x == other.x && this->y == other.y;
		}
		bool operator!=(const Coord& other)
		{
			return !operator==(other);
		}
	};

	/* Values to pass to a function to control the cursor's appearance. */
	enum class CursorStyle
	{
		blinking_block = 1,
		steady_block,
		blinking_underline,
		steady_underline,
		blinking_i_beam,
		steady_i_beam,
		blinking_default,
		steady_default,
		hidden,
		not_hidden
	};

	/* Values to pass to functions that control the terminal's text style.
	   Some styles don't work in all terminals. */
	enum class Style
	{
		reset_all,
		bold,
		dim,
		italic,
		underlined,
		slow_blink,
		fast_blink,
		reverse,
		hidden,
		strikethrough,
		double_underlined = 21,
		normal_intensity,
		not_italic,
		not_underlined,
		not_blinking,
		not_reversed = 27,
		not_hidden,
		not_strikethrough,
		black = 30,
		red,
		green,
		yellow,
		blue,
		magenta,
		cyan,
		reset_color = 39,
		bg_black,
		bg_red,
		bg_green,
		bg_yellow,
		bg_blue,
		bg_magenta,
		bg_cyan,
		bg_white,
		reset_bg_color = 49,
		framed = 51,
		encircled,
		overlined,
		not_framed_or_encircled,
		not_overlined,
		bright_gray = 90,
		bright_red,
		bright_green,
		bright_yellow,
		bright_blue,
		bright_magenta,
		bright_cyan,
		bright_white,
		bright_bg_gray = 100,
		bright_bg_red,
		bright_bg_green,
		bright_bg_yellow,
		bright_bg_blue,
		bright_bg_magenta,
		bright_bg_cyan,
		bright_bg_white,
	};

	/* Prints a string that can have any emoji/Unicode characters. */
	void print(std::string message);

	/* Changes the terminal cursor's appearance. */
	void set_cursor_style(CursorStyle style);

	/* Returns a string that changes the terminal cursor's appearance when printed. */
	std::string ret_set_cursor_style(CursorStyle style);

	/* Resets the terminal's cursor to the default style. */
	void reset_cursor_style();

	/* Returns a string that resets the terminal's cursor to the default style when printed. */
	std::string ret_reset_cursor_style();

	/* Changes the terminal window's title.
	   Throws std::invalid_argument if the title's size is >= 255. */
	void set_window_title(std::string title);

	/* Returns a string that changes the terminal window's title when printed.
	   Throws std::invalid_argument if the title's size is >= 255. */
	std::string ret_set_window_title(std::string title);

	/* Prints a message with style such as as color, italics, underline, etc. */
	void print_styled(std::string message, std::vector<Style> styles);

	/* Returns a string that displays a message with style such as as color, italics,
	   underline, etc. when printed. */
	std::string ret_print_styled(std::string message, std::vector<Style> styles);

	/* Sets future output to have a style such as as color, italics, underline, etc. */
	void set_style(std::vector<Style> styles);

	/* Returns a string that sets future output to have a style such as as color, italics,
	   underline, etc. when printed. */
	std::string ret_set_style(std::vector<Style> styles);

	/* Sets future output to NOT have any special styles. */
	void reset_style();

	/* Returns a string that sets future output to NOT have any special styles whe printed. */
	std::string ret_reset_style();

	/* Prints a message with a chosen color.
	   Throws std::invalid_argument if a color value is greater than 255.
	   The red, green, and blue variables must be in the range [0, 255]. */
	void print_rgb(unsigned red, unsigned green, unsigned blue, std::string message);

	/* Returns a string that displays a message with a chosen color when printed.
	   Throws std::invalid_argument if a color value is greater than 255.
	   The red, green, and blue variables must be in the range [0, 255]. */
	std::string ret_print_rgb(unsigned red, unsigned green, unsigned blue, std::string message);

	/* Prints a message with a chosen background color.
	   Throws std::invalid_argument if a color value is greater than 255. */
	void print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::string message);

	/* Returns a string that displays a message with a chosen background color when printed.
	   Throws std::invalid_argument if a color value is greater than 255. */
	std::string ret_print_bg_rgb(unsigned red, unsigned green, unsigned blue, std::string message);

	/* Sets future output to have a chosen color. */
	void set_rgb(unsigned red, unsigned green, unsigned blue);

	/* Returns a string that sets future output to have a chosen color when printed. */
	std::string ret_set_rgb(unsigned red, unsigned green, unsigned blue);

	/* Sets future output to have a chosen background color. */
	void set_bg_rgb(unsigned red, unsigned green, unsigned blue);

	/* Returns a string that sets future output to have a chosen background color when printed. */
	std::string ret_set_bg_rgb(unsigned red, unsigned green, unsigned blue);

	/* Prints a message at chosen coordinates. */
	void print_at(unsigned x, unsigned y, std::string message);

	/* Returns a string that displays a message at chosen coordinates when printed. */
	std::string ret_print_at(unsigned x, unsigned y, std::string message);

	/* Moves the terminal's cursor to chosen coordinates.
	   In this coordinate system, x increases to the right and y increases downwards. */
	void set_cursor_coords(unsigned x, unsigned y);

	/* Returns a string that moves the terminal's cursor to chosen coordinates when printed.
	   In this coordinate system, x increases to the right and y increases downwards. */
	std::string ret_set_cursor_coords(unsigned x, unsigned y);

	/* Moves the terminal's cursor to chosen coordinates.
	   In this coordinate system, x increases to the right
	   and y increases downwards. */
	void set_cursor_coords(Coord coord);

	/* Returns a string that moves the terminal's cursor to chosen coordinates when printed.
	   In this coordinate system, x increases to the right and y increases downwards. */
	std::string ret_set_cursor_coords(Coord coord);

	/* Detects the current coordinates of the terminal's cursor. */
	Coord get_cursor_coords();

	/* Saves the terminal cursor's current location to be restored later. */
	void save_cursor_location();

	/* Returns a string that saves the terminal cursor's current location (to be restored later)
	   when printed. */
	std::string ret_save_cursor_location();

	/* Restores the terminal cursor to a previously saved location. */
	void restore_cursor_location();

	/* Returns a string that restores the terminal cursor to a previously saved location
	   when printed. */
	std::string ret_restore_cursor_location();

	/* Moves the terminal cursor up a chosen number of lines.
	   If the cursor is at the top of the window, this function will have no effect. */
	void move_cursor_up(size_t lines = 1);

	/* Returns a string that moves the terminal cursor up a chosen number of lines when printed.
	   If the cursor is at the top of the window, printing the string will have no effect. */
	std::string ret_move_cursor_up(size_t lines = 1);

	/* Moves the terminal cursor down a chosen number of lines.
	   If the cursor is at the bottom of the window, this function will have no effect. */
	void move_cursor_down(size_t lines = 1);

	/* Returns a string that moves the terminal cursor down a chosen number of lines when printed.
	   If the cursor is at the bottom of the window, printing the string will have no effect. */
	std::string ret_move_cursor_down(size_t lines = 1);

	/* Moves the terminal cursor to the right a chosen number of columns.
	   If the cursor is at the right edge of the window, this function will
	   have no effect. */
	void move_cursor_right(size_t columns = 1);

	/* Returns a string that moves the terminal cursor to the right a chosen number
	   of columns when printed. If the cursor is at the right edge of the window,
	   printing the string will have no effect. */
	std::string ret_move_cursor_right(size_t columns = 1);

	/* Moves the terminal cursor to the left a chosen number of columns.
	   If the cursor is at the left edge of the window, this function will
	   have no effect. */
	void move_cursor_left(size_t columns = 1);

	/* Returns a string that moves the terminal cursor to the left a chosen number
	   of columns when printed. If the cursor is at the left edge of the window,
	   printing the string will have no effect. */
	std::string ret_move_cursor_left(size_t columns = 1);

	/* Detects the terminal window's current size in columns and rows. */
	Coord get_window_size();

	/* Gets a keypress of input without requiring an enter keypress.
	   Optional parameters:
	   * whether to wait for the keypress.

	   Some possible return values are entire words or multiple words, such as "enter"
	   or "left arrow". To see all possible return values, you can look through the
	   function definition or run the terminal tests (the first test is for the get_key function).
	   Here are some potentially unexpected return values:
	   ctrl+c -> (ends the program immediately)
	   ctrl+2 -> (ends the program immediately)
	   ctrl+h -> "backspace"
	   ctrl+i -> "tab"
	   ctrl+j -> "ctrl+enter"
	   ctrl+m -> "enter" */
	std::string get_key(bool wait = true);

	/* Waits for and gets a character input without requiring an Enter keypress.
	   See a list of many possible return values here: https://gist.github.com/wheelercj/3e6dd4f9c8b267145cbd746d8daccf80
	   Cross-platform, unlike _getch from conio.h. */
	char getch_();

	/* Detects whether a key was pressed.
	   Ignores modifier keys.
	   Cross-platform, unlike _kbhit from conio.h. */
	bool kbhit__();

	/* Pauses the program until the user presses a key.
	   Modifier keys are ignored. */
	void pause();

	/* Inserts text at the terminal cursor's current location.
	   Any text that is pushed out of the window to the right is deleted. */
	void insert(std::string text);

	/* Returns a string that inserts text at the terminal cursor's current location
	   when printed. Any text that is pushed out of the window to the right is deleted. */
	std::string ret_insert(std::string text);

	/* Deletes a chosen number of characters at the terminal cursor's current location.
	   All characters to the right of what is deleted move to the left. */
	void delete_chars(size_t count);

	/* Returns a string that deletes a chosen number of characters at the terminal
	   cursor's current location when printed. All characters to the right of what
	   is deleted move to the left. */
	std::string ret_delete_chars(size_t count);

	/* Backspaces a chosen number of characters at the terminal cursor's current location. */
	void backspace_chars(size_t count);

	/* Returns a string that backspaces a chosen number of characters at the terminal
	   cursor's current location when printed. */
	std::string ret_backspace_chars(size_t count);

	/* Inserts empty lines at the terminal cursor's current location.
	   Any text that gets pushed down off the window is deleted. */
	void insert_lines(size_t count);

	/* Returns a string that inserts empty lines at the terminal cursor's current location
	   when printed. Any text that gets pushed down off the window is deleted. */
	std::string ret_insert_lines(size_t count);

	/* Deletes a chosen number of lines at the terminal cursor's current location.
	   All characters below what is deleted move up. */
	void delete_lines(size_t count);

	/* Returns a string that deletes a chosen number of lines at the terminal cursor's
	   current location when printed. All characters below what is deleted move up. */
	std::string ret_delete_lines(size_t count);

	/* Deletes all characters in the terminal window.
	   Use of this function is not recommended without using an alternate screen buffer. */
	void clear_screen();

	/* Returns a string that deletes all characters in the terminal window when printed.
	   Printing of the string is not recommended without using an alternate screen buffer. */
	std::string ret_clear_screen();

	/* Switches the terminal app to a different "window" in the terminal.
	   Some terminals move the cursor to the top when the new "window" opens, and some
	   do not. This can be called multiple times consecutively, but only the original
	   buffer can be restored. */
	void alternate_screen_buffer();

	/* Returns a string that switches the terminal app to a different "window" in the terminal
	   when printed. Some terminals move the cursor to the top when the new "window" opens,
	   and some do not. The string can be printed multiple times consecutively, but only the
	   original buffer can be restored. */
	std::string ret_alternate_screen_buffer();

	/* Restores the terminal app's original "window". Does nothing if already on the
	   original buffer. */
	void restore_screen_buffer();

	/* Returns a string that restores the terminal app's original "window" when printed.
	   Does nothing if already on the original buffer. */
	std::string ret_restore_screen_buffer();

	/* Restores the screen buffer, the text style, and the terminal cursor style if
	   ctrl/cmd+c or ctrl/cmd+2 is pressed. */
	void reset_on_keyboard_interrupt();

	void set_window_width_to_132();

	std::string ret_set_window_width_to_132();

	void set_window_width_to_80();

	std::string ret_set_window_width_to_80();
}
