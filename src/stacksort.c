static void
swap(void *a, void *b, unsigned int size)
{
	char t;

        do {
                t = *(char *)a;
                *(char *)a++ = *(char *)b;
                *(char *)b++ = t;
        } while (--size > 0);
}


void 
stackSort(void *base, unsigned int size, unsigned int num, int (*cmp_func)(const void *, const void *))
{
        /* pre-scale counters for performance */
        int i = (num/2 - 1) * size, n = num * size, c, r;

        /* heapify */
        for ( ; i >= 0; i -= size) {
                for (r = i; r * 2 + size < n; r  = c) {
                        c = r * 2 + size;
                        if (c < n - size &&
                                        cmp_func(base + c, base + c + size) < 0)
                                c += size;
                        if (cmp_func(base + r, base + c) >= 0)
                                break; 
                        swap(base + r, base + c, size);
                }
        }

        /* sort */ 
        for (i = n - size; i > 0; i -= size) {
                swap(base, base + i, size);
                for (r = 0; r * 2 + size < i; r = c) {
                        c = r * 2 + size;
                        if (c < i - size &&
                                        cmp_func(base + c, base + c + size) < 0)
                                c += size;
                        if (cmp_func(base + r, base + c) >= 0)
                                break; 
                        swap(base + r, base + c, size);
                }
        }
}
