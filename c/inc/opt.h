#pragma once

#ifdef __GNUC__

#define opt_likely(x) 	__builtin_expect(!!(x), 1)
#define opt_unlikely(x)	__builtin_expect(!!(x), 0)

#define opt_noreturn __attribute__((noreturn))

#define opt_pure __attribute__((pure))

#define opt_constructor __attribute__((constructor))
#define opt_destructor __attribute__((destructor))
#define opt_construct(num) __attribute__((constructor(num)))
#define opt_destruct(num)  __attribute__((destructor(num)))

#define opt_const __attribute__((const))

#else 

#define opt_likely(x)	x
#define opt_unlikely(x)	x

#define opt_noreturn

#define opt_pure

#define opt_constructor 
#define opt_destructor
#define opt_construct(num)
#define opt_destruct(num)

#define opt_const

#endif
