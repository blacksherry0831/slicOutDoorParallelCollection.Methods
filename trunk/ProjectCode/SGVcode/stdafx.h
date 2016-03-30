
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS


//#include <vld.h> 
#ifdef _MSC_VER

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#endif 

#ifndef ASSERT
#if linux||__linux||__linux__||__GNUC__
#include <assert.h>
#define  ASSERT(Exp) assert(Exp)
#endif
#endif




#define DEBUG_CUI FALSE
#define GetHorLineFromFile  FALSE
//#include <vld.h> 
#include <../CuiLib/SGVcode/cfg/cui_confg_400_CUDA_ColorBar20150319.h>
#include <../CuiLib/modules.h>
//#include <../cudaLib/CUDA_Code.h>
//#include "vld.h"
//#include "../../gperftools-2.0/src/gperftools/heap-profiler.h"








