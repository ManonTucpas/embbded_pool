#include "../include/hexa.h"

uint8_t hex_to_n(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return 10 + (c - 'A');
    if (c >= 'a' && c <= 'f')
        return 10 + (c - 'a');
    return (0);
}

uint8_t pair_to_byte(char a, char b)
{
    // 4 bits de poids faible da a vers les 4 bits de poids fort de l'octet
    // 4 bits de b dans les 4 bits de poids faible de l'octet
    // hex_to_n('F') << 4   = 0b11110000
    // hex_to_n('F')        = 0b00001111
    // OR                   = 0b11111111 = 255
    return (hex_to_n(a) << 4) | (hex_to_n(b));
}