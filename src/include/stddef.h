#ifndef __STDDEF_H__
#define __STDDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__cplusplus)
#define NULL			(0)
#else
#define NULL			((void *)0)
#endif

#if (defined(__GNUC__) && (__GNUC__ >= 4))
#define offsetof(type, member)	__builtin_offsetof(type, member)
#define clamp(v, a, b)	min(max(a, v), b)

#else
#define offsetof(type, field)	((size_t)(&((type *)0)->field))
#endif

#if (defined(__GNUC__) && (__GNUC__ >= 3))
#define likely(expr)	(__builtin_expect(!!(expr), 1))
#define unlikely(expr)	(__builtin_expect(!!(expr), 0))
#else
#define likely(expr)	(!!(expr))
#define unlikely(expr)	(!!(expr))
#endif

#if !defined(__cplusplus)
#define min(a, b)		({typeof(a) _amin = (a); typeof(b) _bmin = (b); (void)(&_amin == &_bmin); _amin < _bmin ? _amin : _bmin;})
#define max(a, b)		({typeof(a) _amax = (a); typeof(b) _bmax = (b); (void)(&_amax == &_bmax); _amax > _bmax ? _amax : _bmax;})
#else
#define min(a, b)		(a < b ? a : b)
#define max(a, b)		(a > b ? a : b)
#endif
#define clamp(v, a, b)	min(max(a, v), b)

#define ifloor(x)		((x) > 0 ? (int)(x) : (int)((x) - 0.9999999999))
#define iround(x)		((x) > 0 ? (int)((x) + 0.5) : (int)((x) - 0.5))
#define iceil(x)		((x) > 0 ? (int)((x) + 0.9999999999) : (int)(x))
#define idiv255(x)		((((int)(x) + 1) * 257) >> 16)

#define X(...)			("" #__VA_ARGS__ "")

enum {
	FALSE				= 0,
	TRUE				= 1,
};

#ifdef __cplusplus
}
#endif

#endif /* __STDDEF_H__ */
