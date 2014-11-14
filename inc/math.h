#pragma once

static inline unsigned int
mathMinPow(unsigned int v)
{
	float f = (float)(v - 1);
	return v & ~1 ? (1 << ((*(unsigned int *)(&f) >> 23) - 126)) : 1;
}
