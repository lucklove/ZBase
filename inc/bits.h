#pragma once

#define BITS_PER_BYTE 8

void bits_set(void *, unsigned int);
void bits_clr(void *, unsigned int);
void bits_dump(void *, unsigned int);
int bits_test(void *, unsigned int);
