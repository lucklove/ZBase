#include "ref.h"

ref_t
makeRef(void (*releaseFunc)(ref_t *))
{
	return (ref_t) { 1, releaseFunc };
}

int
refGet(ref_t *ref)
{
	atomic_fetch_add(&ref->count, 1);
	return ref->count;
}

int
refPut(ref_t *ref)
{
	atomic_fetch_sub(&ref->count, 1);
	if(ref->count == 0 && ref->releaseFunc != ((void *)0)) 
		ref->releaseFunc(ref);
	return ref->count;
}
