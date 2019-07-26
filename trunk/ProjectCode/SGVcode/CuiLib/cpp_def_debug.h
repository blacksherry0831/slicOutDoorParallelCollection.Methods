#pragma once
/*----------------------------------------------------------------------------------*/
#ifndef CPP_DEF_DEBUG_H
#define CPP_DEF_DEBUG_H
/*-----------------------------------------*/
#ifndef ASSERT
#include <assert.h>
#define  ASSERT(Exp) assert(Exp)
#endif
/*-----------------------------------------*/
#ifndef TRACE_FUNC_ARGS
#define TRACE_FUNC_ARGS(fmt,...)  printf("%s(%d)-<%s>: "##fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__) 
#endif // 
/*-----------------------------------------*/
#ifndef TRACE_FUNC
#define TRACE_FUNC()  printf("\n>>>>>>CALL FUNC : %s(%d)\n", __FUNCTION__,__LINE__) 
#endif // 
/*-----------------------------------------*/
#endif









