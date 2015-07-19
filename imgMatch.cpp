// imgMatch.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "imgMatch.h"

#include "MainFrm.h"
#include "imgMatchDoc.h"
#include "imgMatchView.h"
#include "direct.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool b_useDB = 1;
/////////////////////////////////////////////////////////////////////////////
// CImgMatchApp

BEGIN_MESSAGE_MAP(CImgMatchApp, CWinApp)
	//{{AFX_MSG_MAP(CImgMatchApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgMatchApp construction

CImgMatchApp::CImgMatchApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CImgMatchApp object

CImgMatchApp theApp;

void sortDBData(DB_table& db_t){
	ADODB::_ConnectionPtr m_pConnection;
	_variant_t RecordsAffected;
	ADODB::_RecordsetPtr m_pRecordset;

	try{
		m_pConnection.CreateInstance(__uuidof(ADODB::Connection));
		m_pConnection->Open(db_t.DBName, "", "", ADODB::adModeUnknown);
	}
	catch (_com_error e){
		CString errormessage;
		errormessage.Format("连接数据库失败!\r错误信息:%s", e.ErrorMessage());
		MessageBox(0,errormessage, "error:连接数据库失败",0);
		if (m_pRecordset)
			m_pRecordset->Close();
		if (m_pConnection && m_pConnection->State)
			m_pConnection->Close();
		return ;//exit(-1);
	}

	try{
		_bstr_t strCmd = "select * from " + db_t.table[0].tableName;
		m_pRecordset = m_pConnection->Execute(strCmd, &RecordsAffected, ADODB::adCmdText);
		CString fileNm, src = db_t.source;
		CreateDirectory("ImgCopy", NULL);	//创建临时用于复制的文件夹
		while ( !m_pRecordset->adoEOF ){
			CString newNm = "ImgCopy\\";
			fileNm = (LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_variant_t((long)2));
			if (PathFileExists((LPCTSTR)(_bstr_t)m_pRecordset->GetCollect(_variant_t((long)1)))		//srcName所指文件存在
				&& PathFileExists(db_t.source +("\\"+ fileNm)))	//fileName所指文件存在
				CopyFile(src + "\\" + fileNm, newNm + fileNm, 0);
			else{
				_bstr_t strCmd = "delete * from " + db_t.table[0].tableName + " where " + db_t.table[0].tvalueName[2] + "='" + fileNm + "'";
				m_pConnection->Execute(strCmd, &RecordsAffected, ADODB::adCmdText);
			}
			m_pRecordset->MoveNext();
		}
	}
	catch (_com_error &e){
		MessageBox(0,e.Description(), "error",0);
		return ;//exit(-1);
	}
	SHFILEOPSTRUCT Op;
	ZeroMemory((void*)&Op, sizeof(SHFILEOPSTRUCT));
	Op.pFrom = db_t.source;
	Op.wFunc = FO_DELETE;
	Op.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
	if (SHFileOperation(&Op) != 0){		//该方法删除文件夹失败后调用其他方法删除文件夹
		//MessageBox(0, "失败", db_t.source, 0);
		RemoveDirectory(db_t.source);
		if(GetFileAttributes(db_t.source)==FILE_ATTRIBUTE_DIRECTORY)
		{
			CString cmd = "del ";
			system(cmd + db_t.source + "\\*.* /f /q");
		}
		rmdir(db_t.source);
		Sleep(1);
	}
	rename("ImgCopy", db_t.source);
}

void initDB(){
	b_useDB = 1;
	DB_table db_t;
	ADOX::_CatalogPtr m_pCatalog = NULL;
	//如果该数据表不存在就创建
	if (!PathFileExists(db_t.m_dbName)){
		try{
			m_pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
			m_pCatalog->Create(db_t.DBName);
		}catch (_com_error &e){
			MessageBox(0, e.ErrorMessage(), "error:创建实例失败（使用数据库功能将无法工作）", 0);
			b_useDB = 0;
		}
	}

	const int tableNum = 1;
	const CString m_tableName[tableNum] = { db_t.table[0].tableName };
	bool IsExist[tableNum] = { 0 };
	ADOX::_TablePtr m_pTable = NULL;
	ADODB::_ConnectionPtr m_pConnection;
	_variant_t RecordsAffected;
	try{
		m_pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
		m_pCatalog->PutActiveConnection(db_t.DBName);
		int tableCount = m_pCatalog->Tables->Count;

		for (int i = 0; i<tableCount; i++){
			m_pTable = (ADOX::_TablePtr)m_pCatalog->Tables->GetItem((long)i);
			CString tableName = (CString)((BSTR)m_pTable->Name);
			for (int j = 0; j<tableNum; j++)
			if (tableName == m_tableName[j])
				IsExist[j] = true;
		}
		for (int i = 0; i<tableNum; i++){
			//如果此表不存在就建表
			if (!IsExist[i]){
				try{
					m_pConnection.CreateInstance(__uuidof(ADODB::Connection));
					m_pConnection->Open(db_t.DBName, "", "", ADODB::adModeUnknown);
				}
				catch (_com_error e){
					CString errormessage;
					errormessage.Format("连接数据库失败!\r错误信息:%s", e.ErrorMessage());
					MessageBox(0, errormessage, "连接数据库失败（使用数据库功能将无法工作）", 0);
					b_useDB = 0;
				}
				try{
					_bstr_t strCommand = "CREATE TABLE " + m_tableName[i] + "(" + db_t.table[0].tvalueName[0] +
						" varchar(33) primary key," + db_t.table[0].tvalueName[1] + " varchar(255)," + db_t.table[0].tvalueName[2] + " varchar(255));";
					m_pConnection->Execute(strCommand, &RecordsAffected, ADODB::adCmdText);

					strCommand = "INSERT INTO " + m_tableName[i] + "(" + db_t.table[0].tvalueName[0] + "," + db_t.table[0].tvalueName[1] + ","
						+ db_t.table[0].tvalueName[2] + ") VALUES('1','2','3')";
					m_pConnection->Execute(strCommand, &RecordsAffected, ADODB::adCmdText);
					if (m_pConnection && m_pConnection->State)
						m_pConnection->Close();
				}
				catch (_com_error &e){
					MessageBox(0, e.Description(), "error：创建表失败（使用数据库功能将无法工作）", 0);
					b_useDB = 0;
				}
			}
		}
	}
	catch (_com_error &e){
		MessageBox(0,e.Description(), "error:使用数据库功能将无法工作",0);
		//b_useDB = 0;
	}
	if(b_useDB)
		sortDBData(db_t);
}
/////////////////////////////////////////////////////////////////////////////
// CImgMatchApp initialization

BOOL CImgMatchApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Image Process and Handwriting identification, Developped by wenbo"));

	//LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CImgMatchDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CImgMatchView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	SetForegroundWindow((HWND)AfxGetMainWnd());	//将窗口置顶显示
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CImgMatchApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CImgMatchApp message handlers

