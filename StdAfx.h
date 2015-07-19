// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently


#if !defined(AFX_STDAFX_H__3DFA5876_1256_46DC_85D9_92B52CA812AF__INCLUDED_)
#define AFX_STDAFX_H__3DFA5876_1256_46DC_85D9_92B52CA812AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WIN32_WINNT 
#define _WIN32_WINNT 0x0502 
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#pragma warning(disable:4996)

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

#import "C:\Program Files\Common Files\system\ado\msadox.dll" 
#import "C:\Program Files\Common Files\system\ado\msado15.dll" rename("EOF","adoEOF")

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
//检查内存泄露
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include<stdlib.h>
#include<crtdbg.h>
#endif 


#include "DB_table.h"
//增加崩溃日志记录，正常结束时会自动删除
#include <fstream>
#define LOG(s) {ofstream out("Log\\detail-log.txt",ios::out|ios::app);out<<s;out.close();}
#define LOGT(s) {ofstream out("Log\\time-log.txt",ios::out|ios::app);out<<s;out.close();}

#endif // !defined(AFX_STDAFX_H__3DFA5876_1256_46DC_85D9_92B52CA812AF__INCLUDED_)
