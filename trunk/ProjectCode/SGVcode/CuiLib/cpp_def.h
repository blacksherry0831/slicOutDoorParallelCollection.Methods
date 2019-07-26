#ifndef CPP_DEF_H
#define CPP_DEF_H
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
#ifndef  C_STD_LIB_ASSERT
#include <assert.h>
#define  C_STD_LIB_ASSERT(Exp) assert(Exp)
#endif
/*-----------------------------------------*/
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
/*-----------------------------------------*/
#ifndef TRUE
#define TRUE 1
#endif
/*-----------------------------------------*/
#ifndef FALSE
#define FALSE 0
#endif
/*-----------------------------------------*/
#if linux||__linux||__linux__||__GNUC__
#ifndef nullptr
#define nullptr 0
#endif
#endif
/*-----------------------------------------*/
#ifndef DWORD
typedef unsigned long       DWORD;
#endif
/*-----------------------------------------*/
#ifndef BOOL
typedef int                 BOOL;
#endif
/*-----------------------------------------*/
#ifndef BYTE
typedef unsigned char       BYTE;
#endif
/*-----------------------------------------*/
#ifndef UINT32
typedef unsigned int UINT32;
#endif
/*-----------------------------------------*/
#ifndef BOOL
typedef int BOOL;
#endif
/*-----------------------------------------*/
#ifndef INT32
typedef signed int INT32;
#endif
/*-----------------------------------------*/
#ifndef ULONGLONG
typedef unsigned long long ULONGLONG;
#endif
/*-----------------------------------------*/
#ifndef LPVOID

#if __GNUC__
#define LPVOID  void *
#endif

#endif
/*-----------------------------------------*/
#ifndef PVOID
typedef void *PVOID;
#endif
/*-----------------------------------------*/
#ifndef HANDLE
typedef PVOID HANDLE;
#endif
/*-----------------------------------------*/
#ifndef UINT
typedef unsigned int UINT;
#endif // !UINT
/*-----------------------------------------*/
#ifndef PUINT
typedef unsigned int        *PUINT;
#endif // !1
/*-----------------------------------------*/
#ifndef boolean
typedef unsigned char boolean;
#endif // !boolean
/*-----------------------------------------*/

/*-----------------------------------------*/

/*-----------------------------------------*/
#endif









