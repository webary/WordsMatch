// imgMatch.h : main header file for the IMGMATCH application
//

#if !defined(AFX_IMGMATCH_H__F55B795D_39DF_40AA_AC0C_711FEB7E0046__INCLUDED_)
#define AFX_IMGMATCH_H__F55B795D_39DF_40AA_AC0C_711FEB7E0046__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
extern bool b_useDB;
#include <Shlwapi.h>
#pragma comment (lib,"Shlwapi.lib")	//PathFileExists
/////////////////////////////////////////////////////////////////////////////
// CImgMatchApp:
// See imgMatch.cpp for the implementation of this class
//

void initDB();

class CImgMatchApp : public CWinApp
{
public:
	CImgMatchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgMatchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CImgMatchApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGMATCH_H__F55B795D_39DF_40AA_AC0C_711FEB7E0046__INCLUDED_)
