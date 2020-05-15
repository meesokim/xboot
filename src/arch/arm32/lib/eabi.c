/*
 * eabi.c
 */
#include <types.h>

int raise(int signal)
{
	return 0;
}

// multiple definition of `__aeabi_unwind_cpp_pr0'; .obj/arch/arm32/lib/eabi.o:/home/msx/xboot/src/arch/arm32/lib/eabi.c:13: first defined here
#if 0
void __aeabi_unwind_cpp_pr0(void)
{
}
#endif

void __aeabi_unwind_cpp_pr1(void)
{
}

void __aeabi_memcpy(void * dest, const void * src, size_t len)
{
	extern void * memcpy(void *, const void *, size_t);
	(void)memcpy(dest, src, len);
}

void __aeabi_memset(void * s, size_t n, int c)
{
	extern void * memset(void *, int, size_t);
	(void)memset(s, c, n);
}
