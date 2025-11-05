#ifndef _HEX_
# define _HEX_

#include <avr/io.h>

uint8_t is_hex(char c);

uint8_t hex_to_n(char c);

uint8_t pair_to_byte(char a, char b);

#endif