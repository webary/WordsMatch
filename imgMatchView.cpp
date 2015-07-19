// imgMatchView.cpp : implementation of the CImgMatchView class
#include "stdafx.h"
#include "imgMatch.h"

#include "MainFrm.h"
#include "imgMatchDoc.h"
#include "imgMatchView.h"
#include "AdvancedSet.h"

#include "imgPro.h"
#include "targetMatch.h"
#include "adaMachineLearning.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
	void CImgMatchView::AssertValid() const
	{
		CFormView::AssertValid();
	}

	void CImgMatchView::Dump(CDumpContext& dc) const
	{
		CFormView::Dump(dc);
	}

	CImgMatchDoc* CImgMatchView::GetDocument() // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImgMatchDoc)));
		return (CImgMatchDoc*)m_pDocument;
	}
#endif //_DEBUG
	
/////////////////////////////////////////////////////////////////////////////
// CHelpDlg dialog used for App Help

class CHelpDlg : public CDialog
{
public:
	CHelpDlg() : CDialog(CHelpDlg::IDD){};
	enum { IDD = IDD_useHelp };
protected:
	virtual void DoDataExchange(CDataExchange* pDX){CDialog::DoDataExchange(pDX);};
};

/////////////////////////////////////////////////////////////////////////////
// CUpdateLogDlg dialog used for App UpdateLog

class CUpdateLogDlg : public CDialog
{
public:
	CUpdateLogDlg();
	enum { IDD = IDD_UpdateLog };
protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		DDX_Text(pDX, IDC_Update_log, m_updateLog);
	}
public:
	CString m_updateLog;
};

CUpdateLogDlg::CUpdateLogDlg() : CDialog(CUpdateLogDlg::IDD)
{
	m_updateLog = _T(
		"-->v2.0 正式版：\r\n"
		"1.修复数据库验证删除过程不彻底的bug\r\n"
		"2.拖入文件后自动将软件最前端显示\r\n"
		"3.小幅调整界面的布局\r\n"
		"\r————————————————————————————————\r\n"
		"\r\n"
		"-->v1.5 正式版：\r\n"
		"1.支持拖入文件到窗口完成载入,支持的格式有.jpg/.bmp/.png/.path/.epath\r\n"
		"2.加入数据库存储,对预处理后的图片进行保存，可加速载入同一图片\r\n"
		"3.修改首次打开检测方式\r\n"
		"\r————————————————————————————————\r\n"
		"\r\n"
		"-->v1.4.1 测试版：\r\n"
		"1.增加调整正负训练集所占权重的参数（正负平衡系数）\r\n"
		"\r————————————————————————————————\r\n"
		"\r\n"
		"-->v1.4 正式版：\r\n"
		"1.增加新特征 bias\r\n"
		"2.根据正负训练集的数目自动调整所占权重\r\n"
		"\r————————————————————————————————\r\n"
		"\r\n"
		"-->v1.3.3 测试版：\r\n"
		"1.修改部分bug,提高稳定性\r\n"
		"\r————————————————————————————————\r\n"
		"\r\n"
		"-->v1.3.2 测试版：\r\n"
		"1.增加新特征，提高识别正确率\r\n"
		"2.增加多线程并发处理，加快读取训练集和测试集的速度\r\n"
		"\r————————————————————————————————\r\n"
		"\r\n"
		"-->v1.3.1 测试版：\r\n"
		"1.增加进度显示功能\r\n"
		"\r————————————————————————————————\r\n"
		"\r\n"
		"-->v1.3 正式版：\r\n"
		"1.增加线程功能，询问是否停止时后台仍然继续演化\r\n"
		"2.优化数据量较大时界面出现的假死状态\r\n"
		"3.优化内存和CPU占用量\r\n"
		"4.修复首次读取时偶尔会出错的bug\r\n"
		"5.对用户操作做详细记录，方便意外崩溃情况下的问题分析\r\n"
		"6.过滤运算过程中用户对窗口移动的操作，防止窗口绘制异常\r\n"
		"————————————————————————————————\r\n"
		"\r\n"
		"-->v1.2.2 测试版：\r\n"
		"1.增加对匹配结果的偏向性的调整功能（高级设置）\r\n"
		"2.增加对选择模板演化时的系数调整功能（高级设置）\r\n"
		"3.优化训练集和测试集与模板的匹配方式\r\n"
		"————————————————————————————————\r\n"
		"\r\n"
		"-->v1.2.1 内测版：\r\n"
		"1.增加保存和读取演化路径功能，可在下次继续进行相似匹配时跳过演化过程\r\n"
		"2.增加对演化模板的列数修改（高级设置）\r\n"
		"3.增加更新日志查看功能\r\n"
		"4.优化测试结果显示方式：不同次载入的测试集分开显示，匹配过的不显示\r\n"
		"————————————————————————————————\r\n"
		"\r\n"
		"-->v1.2 正式版：\r\n"
		"1.增加载入多模板功能，降低了由于模板的特征局限性造成的系统误差\r\n"
		"2.增加保存匹配结果的功能\r\n"
		"3.增加高级设置选项，可以修改一些运行时内部参数，更加符合个性设置\r\n"
		"4.增加对演化询问等待间隔时间和演化允许误差的修改（高级设置）\r\n"
		"5.增加对历史操作中时间的记录，每一步操作都会记录操作时间\r\n"
		"6.增加首次运行检测，首次打开该软件时会自动弹出使用说明\r\n"
		"7.更改了界面背景图片，并把空间背景设置为透明，看起来更加自然\r\n"
		"————————————————————————————————\r\n"
		"\r\n"
		"-->v1.1 正式版：\r\n"
		"1.对界面布局进行大幅调整，将原本的菜单栏相应功能按先后顺序布局当窗口\r\n"
		"2.操作和显示界面设计更加人性化，更符合普通用户的操作习惯\r\n"
		"3.加入背景图片，界面做了美化\r\n"
		"4.操作历史记录改成了向上刷新的显示方式\r\n"
		"5.取消使用帮助中调用控制台的过程\r\n"
		"6.增加对训练集和测试集解释说明的模块\r\n"
		"————————————————————————————————\r\n"
		"\r\n"
		"-->v1.0 测试版：\r\n"
		"1.此原始版本实现了简单的框架和主要功能，界面未做任何美化\r\n"
		"2.功能全部整合到菜单栏\r\n"
		"————————————————————————————————");
}

//////////////////////////////////////////////////////////////////////////
// chooseType dialog used for App ChooseType when drag files to the dialog

class CChooseType : public CDialog
{
public:
	CChooseType() : CDialog(CChooseType::IDD), m_choosed(0){};
	enum { IDD = IDD_ChooseType };
protected:
	virtual void DoDataExchange(CDataExchange* pDX){
		CDialog::DoDataExchange(pDX);
		DDX_Radio(pDX, IDC_RADIO1, m_choosed);
	};
public:
	int m_choosed;
};

#pragma region Declare_MessageMap
IMPLEMENT_DYNCREATE(CImgMatchView, CFormView)

BEGIN_MESSAGE_MAP(CImgMatchView, CFormView)
	ON_COMMAND(ID_FILE_open1, &CImgMatchView::OnFileopen1)
	ON_COMMAND(ID_FILE_open2, &CImgMatchView::OnFileopen2)
	ON_COMMAND(ID_match_all, &CImgMatchView::OnMatchAll)
	ON_COMMAND(ID_Save_Match, &CImgMatchView::OnSaveMatch)
	ON_BN_CLICKED(IDC_Clear_log, &CImgMatchView::OnClearLog)
	ON_COMMAND(ID_match_surf, &CImgMatchView::OnMatchSurf)
	ON_COMMAND(ID_Clear, &CImgMatchView::OnClearLog)
	ON_COMMAND(ID_Clear_all, &CImgMatchView::OnClearAll)
	ON_COMMAND(ID_train_p, &CImgMatchView::OnTrainP)
	ON_COMMAND(ID_train_n, &CImgMatchView::OnTrainN)
	ON_COMMAND(ID_Show_Train_P, &CImgMatchView::OnShowTrainP)
	ON_COMMAND(ID_Show_Train_N, &CImgMatchView::OnShowTrainN)
	ON_COMMAND(ID_Clear_p, &CImgMatchView::OnClearP)
	ON_COMMAND(ID_Clear_N, &CImgMatchView::OnClearN)
	ON_COMMAND(ID_Show_Model, &CImgMatchView::OnShowModel)
	ON_COMMAND(ID_show_obj1, &CImgMatchView::OnShowObj1)
	ON_COMMAND(ID_show_obj2, &CImgMatchView::OnShowObj2)
	ON_COMMAND(ID_Show_Test, &CImgMatchView::OnShowTest)
	ON_COMMAND(ID_Start_Evolution, &CImgMatchView::OnStartEvolution)
	ON_COMMAND(ID_Add_Test, &CImgMatchView::OnAddTest)
	ON_COMMAND(ID_Clear_Test, &CImgMatchView::OnClearTest)
	ON_COMMAND(ID_Start_Match, &CImgMatchView::OnStartMatch)
	ON_COMMAND(ID_Clear_Train, &CImgMatchView::OnClearTrain)
	ON_COMMAND(ID_UseHelp, &CImgMatchView::OnUseHelp)
	ON_COMMAND(ID_Open_Model_many, &CImgMatchView::OnOpenModelMany)
	ON_BN_CLICKED(IDC_train_help, &CImgMatchView::OnTrainHelp)
	ON_BN_CLICKED(IDC_test_help, &CImgMatchView::OnTestHelp)
	ON_COMMAND(ID_AdvancedSet, &CImgMatchView::OnAdvancedSet)
	ON_BN_CLICKED(IDC_SaveMatch, &CImgMatchView::OnSavematch)
	ON_COMMAND(ID_saveEvoPath, &CImgMatchView::OnSaveEvoPath)
	ON_COMMAND(ID_readEvoPath, &CImgMatchView::OnReadEvoPath)
	ON_COMMAND(ID_updateLog, &CImgMatchView::OnUpdateLog)
	ON_EN_VSCROLL(IDC_log, &CImgMatchView::OnEnVscrollLog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_CLOSE()
	ON_COMMAND(ID_readModelTrain, &CImgMatchView::OnReadModelTrain)
	ON_COMMAND(ID_saveModelTrain, &CImgMatchView::OnSaveModelTrain)
	ON_COMMAND(ID_Quit, &CImgMatchView::OnQuit)
	ON_COMMAND(IDD_ResizeWindow, &CImgMatchView::OnResizeWindow)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()
#pragma endregion

//标记是否使用多线程加速(在imgPro.h中定义): #define b_multiThread 0/1
//支持图片最大大小(在imgPro.h中已定义) const int MAXSIZE = 1000;

CImgMatchView::CImgMatchView(): CFormView(CImgMatchView::IDD)
{
	m_bmpBackground.LoadResource(IDB_BITMAP1);
	ml.setML(Ch_Num,1,.75,Dlg.EvoCol);
	filePath1 = filePath2 = _T("");
	img1 = img2 = 0;
	log = "欢迎使用笔迹智能匹配系统，如有疑问请查看使用帮助（快捷键F2）\r\n\r\n快速读取模板和训练集路径请按Ctrl+R\r\n\r\n快速读取演化路径请按R\r\n";
	for(int i=0;i<log_num;i++)
		logg[i].Empty();
	train.p_num = train.n_num = 0;
	for(int j=0;j<10;j++)
		model.path[j].Empty();
	model.n = 0;
	test.num = 0;
	memset(test.times,0,sizeof(test.times));
	Evolution = 0;
	resetP_N = 1;
	matchlog.Empty();
	EvoPath.Empty();
	pStatues = 0;
	for(int i=0;i<100;++i)
		DragImgPath[i].Empty();
	DragEvoPath.Empty();
	DragModelTrainPath.Empty();
#ifdef _DEBUG
	//hLogFile = CreateFile("memleak.log", GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ,\
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//创建日志文件
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);//将warn级别的内容都输出到文件（注意dump的报告级别即为warning）
	_CrtSetReportFile(_CRT_WARN, hLogFile);//将日志文件设置为告警的输出文件
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );//无论程序在什么地方退出都会自动调用 _CrtDumpMemoryLeaks

#define new  new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
	//ofstream out("log",ios::out);
	//out.close();

	CreateDirectory("Log", NULL);	//创建保存日志文件的文件夹
	DeleteFile("Log\\detail-log.txt"); 
	DeleteFile("Log\\time-log.txt");
	DeleteFile("Log\\dbError-log.txt");
	DeleteFile("Log\\match-log.txt");
}

CImgMatchView::~CImgMatchView()
{
	DeleteFile("best.pth");
	DeleteFile("present.pth");
	DeleteFile("tmp.txt");
	DeleteFile("log");
}

void CImgMatchView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_log, log);
	DDV_MaxChars(pDX, log, 1000000000);
}

BOOL CImgMatchView::PreCreateWindow(CREATESTRUCT& cs)
{
	pstyle = &cs;
	return CFormView::PreCreateWindow(cs);
}

UINT first_help(void*pp){
	((CImgMatchView*)pp)->OnUseHelp();
	return 0;
}

void CImgMatchView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	CMainFrame *pFrame = (CMainFrame*)GetParent();
	pStatues = &pFrame->m_wndStatusBar;
	pStatues->SetPaneInfo(0,0,pStatues->GetPaneStyle(0),355);
	pStatues->SetPaneInfo(1,1,pStatues->GetPaneStyle(1),298);
	pStatues->SetPaneInfo(2,2,pStatues->GetPaneStyle(2),40);

	pMF = ((CMainFrame *)AfxGetMainWnd());
	pMF->m_wndStatusBar.GetItemRect(1,&myr);
	if(!pMF->m_bCreated)
	{
		pMF->m_progress.Create(WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, myr, &pMF->m_wndStatusBar,1);
		pMF->m_progress.SetStep(1);
		pMF->m_bCreated = 1;
	}
	DB_table myDB;
	if (!PathFileExists(myDB.m_dbName))
	{
		AfxBeginThread(first_help,this);
	}
	initDB();
	GetDlgItem(IDC_many_model)->SetFocus();
}
inline CString printDateTime(CString s="最近一次操作时间：") {
	char tmpbuf[128];
	char tmpbuf2[128];
	CString now = "",tmp,tmpName;
	time_t now_time=time(NULL);
	struct tm *newtime=localtime(&now_time);
	if(s=="time")	//可以保存当前全屏状态到文件
	{
		strftime(tmpbuf,128,"%Y%m%d",newtime);
		strftime(tmpbuf2,128,"%H%M%S",newtime);
		Screen(strcat(tmpbuf,tmpbuf2));
		return "";
	}
	strftime(tmpbuf,128,"%Y/%m/%d  ",newtime);
	strftime(tmpbuf2,128,"%H:%M:%S",newtime);
	now.Format(_T("%s%s%s"),s,tmpbuf,tmpbuf2);
	return now;
}

void CImgMatchView::updatelog(){
	InvalidateRect(&r,1);
	SetDlgItemText(IDC_log,log+printDateTime());
	CEdit* pEdit = ((CEdit*)GetDlgItem(IDC_log));
	pEdit->LineScroll(pEdit->GetLineCount());
}
#pragma region SimpleMatch
void CImgMatchView::OnShowObj1()
{
	showImg(img1,"对比图1","显示对比图1出错");
}

void CImgMatchView::OnShowObj2()
{
	showImg(img2,"对比图2","显示对比图2出错");
}

//清空操作历史记录
void CImgMatchView::OnClearLog()
{
	log.Empty();
	for(int i=0;i<log_num;i++)
		logg[i].Empty();
	UpdateData(false);
	updatelog();
}

void CImgMatchView::OnClearAll()
{
	LOG(">>重置所有\n\n")
	filePath1.Empty();
	filePath2.Empty();
	EvoPath.Empty();
	train.p_num = train.n_num = 0;
	releaseImg(&img1);
	releaseImg(&img2);
	model.n = 0;
	OnClearLog();
	
	train.p_num = 0;
	train.p_num = 0;
	test.num = 0;
	memset(test.times,0,sizeof(test.times));

	DeleteFile("p_n.txt");
	DeleteFile("test.txt");
	DeleteFile("best.pth");

	for(int i=log_num-1;i>0;i--)
		logg[i] = logg[i-1];
	logg[0].Format(_T("已重置所有配置\r\n-------------操作分割线-------------\r\n\r\n"));
	log = logg[log_num-1];
	for(int i=log_num-2;i>=0;i--)
		log += logg[i];
	UpdateData(false);
	updatelog();
}

void CImgMatchView::OnFileopen1()
{
	strcpy(info,"");
	char fileFormat[] = _T("JPEG (*.jpg)|*.jpg;*.jpeg|PNG (*.png)|*.png|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,0,0,OFN_FILEMUSTEXIST,fileFormat);
    if(dlg.DoModal()==IDOK)
	{
		CString _filePath1 = dlg.GetPathName();
		char* _file1 = (LPSTR)(LPCTSTR)_filePath1 ;
		IplImage* _img1 = loadImg(_file1);
		if(_img1){
			for(int i=log_num-1;i>0;i--)
				logg[i] = logg[i-1];
			logg[0].Format(_T("对比图1:\r\n%s-------------操作分割线-------------\r\n\r\n"),info);
			log = logg[log_num-1];
			for(int i=log_num-2;i>=0;i--)
				log += logg[i];
			UpdateData(false);
			updatelog();
			filePath1 = _filePath1;
			releaseImg(&img1);
			img1 = _img1;
		}
	}
}

void CImgMatchView::OnFileopen2()
{
	strcpy(info,"");
	char fileFormat[] = _T("JPEG (*.jpg)|*.jpg;*.jpeg|PNG (*.png)|*.png|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,0,0,OFN_FILEMUSTEXIST,fileFormat);
    if(dlg.DoModal()==IDOK)
	{
		CString _filePath2 = dlg.GetPathName();
		char* _file2 = (LPSTR)(LPCTSTR)_filePath2 ;
		IplImage* _img2 = loadImg(_file2);
		if(_img2){
			for(int i=log_num-1;i>0;i--)
				logg[i] = logg[i-1];
			logg[0].Format(_T("对比图2:\r\n%s-------------操作分割线-------------\r\n\r\n"),info);
			log = logg[log_num-1];
			for(int i=log_num-2;i>=0;i--)
				log += logg[i];
			UpdateData(false);
			updatelog();
			filePath2 = _filePath2;
			releaseImg(&img2);
			img2 = _img2;
		}
	}
}

void CImgMatchView::OnMatchAll()
{
	strcpy(info,"");
	if(filePath1 && filePath2 && filePath1[0] && filePath2[0]){
		t.getTargetInfo(filePath1,-2,0);
		t2.getTargetInfo(filePath2,-2,0);
		t.compare(t2,0,0,0,Dlg.ratio);
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),info);
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}else
		MessageBox("图像指针无效，请先载入有效图像后再使用该功能","匹配图像出错",MB_ICONERROR);
}

void CImgMatchView::OnSaveMatch()
{
	if(find_obj_match){
		char fileFormat[] = _T("JPEG (*.jpg)|*.jpg;*.jpeg|PNG (*.png)|*.png|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*||");
		CFileDialog dlg(false,"jpg",0,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,fileFormat);
		if(dlg.DoModal()==IDOK)
		{
			CString path = dlg.GetPathName();
			char *path_c = (LPSTR)(LPCTSTR)path ;
			saveImg(path_c,find_obj_match);
		}
	}else
		MessageBox("图像指针无效，请先载入有效图像并综合匹配后再使用该功能","保存图像出错",MB_ICONERROR);
}

void CImgMatchView::OnMatchSurf()
{
	strcpy(info,"");
	if(filePath1 && filePath2 && filePath1[0] && filePath2[0]){
		find_obj(filePath1,filePath2,0,1);
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),info);
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}else
		MessageBox("图像指针无效，请先载入有效图像后再使用该功能","surf角点检测出错",MB_ICONERROR);

}
#pragma endregion
void CImgMatchView::OnTrainP()
{
	LOG(">>载入正训练集\n")
	char fileFormat[] = _T("JPEG (*.jpg)|*.jpg;*.jpeg|PNG (*.png)|*.png|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,0,0,OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,fileFormat);
	int check=0;
	if(DragImgPath[0]=="")
		check = (dlg.DoModal()==IDOK);
	if(DragImgPath[0][0] || check)	//传入了文件路径列表 或者 读取了文件
	{
		int num = train.p_num;
		POSITION pos = dlg.GetStartPosition();
		int jj=0;
		bool loop = DragImgPath[0]=="" ? pos!=0 : DragImgPath[jj][0]!=0;
		while(loop){
			if(DragImgPath[0]!=""){
				train.p[train.p_num]=DragImgPath[jj++];
				loop = DragImgPath[jj][0]!=0;
			}else{
				train.p[train.p_num]=dlg.GetNextPathName(pos);
				loop = pos!=0;
			}
			LOG(train.p[train.p_num]);
			if(!(tmpimg = loadImg(train.p[train.p_num],0,0,1,0))){
				train.p_num--;
				LOG("\t载入失败，已跳过")
			}else if(tmpimg->width>MAXSIZE || tmpimg->height>MAXSIZE)
			{
				char exit_info[1000];
				sprintf(exit_info,("图像%s尺寸过大，请先处理为%d*%d以内再载入！\r\n\r\n已自动跳过该图像！"),train.p[train.p_num],MAXSIZE,MAXSIZE);
				MessageBox(exit_info,"获取图像信息失败",0);
				train.p_num--;
				LOG("\t尺寸过大，已跳过");
			}
			releaseImg(&tmpimg);
			train.p_num++;
			LOG("\n");
		}
		LOG("———正训练集读取完毕———\n");
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		if(train.p_num-num>0){
			tmp.Format(_T("已载入下列%d个正训练集:\r\n"),train.p_num-num);
			for(int i=num;i<train.p_num;i++){
				logg[0].Format(_T("%s%s\r\n"),tmp,train.p[i]);
				tmp = logg[0];
			}
			logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),tmp);
			Evolution = 0;
		}else
			logg[0].Format(_T("未成功载入正训练集，请重试!\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}
	LOG("<<结束载入正训练集———\n\n")
}

void CImgMatchView::OnTrainN()
{
	LOG(">>载入负训练集\n")
	char fileFormat[] = _T("JPEG (*.jpg)|*.jpg;*.jpeg|PNG (*.png)|*.png|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,0,0,OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,fileFormat);
	int check=0;
	if(DragImgPath[0]=="")
		check = (dlg.DoModal()==IDOK);
	if(DragImgPath[0][0] || check)	//传入了文件路径列表 或者 读取了文件
	{
		int num = train.n_num;
		POSITION pos = dlg.GetStartPosition();
		int jj=0;
		bool loop = DragImgPath[0]=="" ? pos!=0 : DragImgPath[jj][0]!=0;
		while(loop){
			if(DragImgPath[0]!=""){
				train.n[train.n_num]=DragImgPath[jj++];
				loop = DragImgPath[jj][0]!=0;
			}else{
				train.n[train.n_num]=dlg.GetNextPathName(pos);
				loop = pos!=0;
			}
			LOG(train.n[train.n_num]);
			if(!(tmpimg = loadImg(train.n[train.n_num],0,0,1,0))){
				train.n_num--;
				LOG("\t载入失败，已跳过")
			}else if(tmpimg->width>MAXSIZE || tmpimg->height>MAXSIZE)
			{
				char exit_info[1000];
				sprintf(exit_info,("图像%s尺寸过大，请先处理为%d*%d以内再载入！\r\n\r\n已自动跳过该图像！"),train.n[train.n_num],MAXSIZE,MAXSIZE);
				MessageBox(exit_info,"获取图像信息失败",0);
				train.n_num--;
				LOG("\t尺寸过大，已跳过")
			}
			releaseImg(&tmpimg);
			train.n_num++;
			LOG("\n")
		}
		LOG("———负训练集读取完毕———\n")
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		if(train.n_num-num>0){
			tmp.Format(_T("已载入下列%d个负训练集:\r\n"),train.n_num-num);
			for(int i=num;i<train.n_num;i++){
				logg[0].Format(_T("%s%s\r\n"),tmp,train.n[i]);
				tmp = logg[0];
			}
			logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),tmp);
			Evolution = 0;
		}else
			logg[0].Format(_T("未成功载入正训练集，请重试!\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}
	LOG("<<结束载入负训练集———\n\n")
}

void CImgMatchView::OnShowTrainP()
{
	for(int i=log_num-1;i>0;i--)
		logg[i] = logg[i-1];
	if(train.p_num>0){
		tmp.Format(_T("当前已载入下列正训练集:\r\n"));
		for(int i=0;i<train.p_num;i++){
			logg[0].Format(_T("%s%s\r\n"),tmp,train.p[i]);
			tmp = logg[0];
		}
		logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),tmp);
	}else
	logg[0].Format(_T("当前还未载入正训练集！\r\n-------------操作分割线-------------\r\n\r\n"));
	log = logg[log_num-1];
	for(int i=log_num-2;i>=0;i--)
		log += logg[i];
	UpdateData(false);
	updatelog();
}

void CImgMatchView::OnShowTrainN()
{
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		if(train.n_num>0){
			tmp.Format(_T("当前已载入下列负训练集:\r\n"));
			for(int i=0;i<train.n_num;i++){
				logg[0].Format(_T("%s%s\r\n"),tmp,train.n[i]);
				tmp = logg[0];
			}
			logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),tmp);
		}else
			logg[0].Format(_T("当前还未载入负训练集！\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
}

void CImgMatchView::OnClearP()
{
	LOG(">>重置正测试集\n\n")
	if(train.p_num!=0){
		train.p_num = 0;
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		logg[0].Format(_T("已清空当前已载入的正训练集\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}else
		OnShowTrainP();
	resetP_N = 1;
}

void CImgMatchView::OnClearN()
{
	LOG(">>重置负测试集\n\n")
	if(train.n_num!=0){
		train.n_num = 0;
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		logg[0].Format(_T("已清空当前已载入的负训练集\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}else
		OnShowTrainN();
	resetP_N = 1;
}

void CImgMatchView::OnClearTrain()
{
	LOG(">>重置训练集\n\n")
	if(train.p_num!=0){
		train.p_num = 0;
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		logg[0].Format(_T("已清空当前已载入的正训练集\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}else
		OnShowTrainP();
	if(train.n_num!=0){
		train.n_num = 0;
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		logg[0].Format(_T("已清空当前已载入的负训练集\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}else
		OnShowTrainN();
	resetP_N = 1;
}

void CImgMatchView::OnOpenModelMany()
{
	LOG(">>载入模板图\n");
	runing = 1;
	char fileFormat[] = _T("JPEG (*.jpg)|*.jpg;*.jpeg|PNG (*.png)|*.png|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,0,0,OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,fileFormat);
	int check=0;
	if(DragImgPath[0]=="")
		check = (dlg.DoModal()==IDOK);
    if(DragImgPath[0][0] || check)	//传入了文件路径列表 或者 读取了文件
	{
		try{
			model.n = 0;
			POSITION pos = dlg.GetStartPosition();
			int jj=0;
			bool loop = DragImgPath[0]=="" ? pos!=0 : DragImgPath[jj][0]!=0;
			while(loop){
				if(model.n==10){		//最多只允许10个模板
					MessageBox("最多只允许载入10个模板，已将前10个作为模板","温馨提示");
					break;
				}
				if(DragImgPath[0]!=""){
					model.path[model.n]=DragImgPath[jj++];
					loop = DragImgPath[jj][0]!=0;
				}else{
					model.path[model.n]=dlg.GetNextPathName(pos);
					loop = pos!=0;
				}
				LOG(model.path[model.n]);
				if(!(tmpimg = loadImg(model.path[model.n],0,0,1,0))){
					model.n--;
					LOG("\t载入失败，已跳过");
				}else if(tmpimg->width>MAXSIZE || tmpimg->height>MAXSIZE)
				{
					char exit_info[1000];
					sprintf(exit_info,("图像%s尺寸过大，请先处理为%d*%d以内再载入！\r\n\r\n\r\n已自动跳过该图像！"),model.path[model.n],MAXSIZE,MAXSIZE);
					MessageBox(exit_info,"获取图像信息失败",0);
					model.n--;
					LOG("\t尺寸过大，已跳过");
				}
				releaseImg(&tmpimg);
				model.n++;
				LOG("\n");
			}
			LOG("———模板图读取完毕———\n");
		}
		catch(...)
		{
			LOG("\nERROR: 读取模板图时出现异常\n\n");
			runing = 0;
			MessageBox("非常抱歉，在读取模板图时出现异常，请关闭后重试","运行出错",0);
			return;
		}
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		if(model.n>0){
			tmp.Format(_T("已载入下列%d个模板:\r\n"),model.n);
			for(int i=0;i<model.n;i++){
				logg[0].Format(_T("%s%s\r\n"),tmp,model.path[i]);
				tmp = logg[0];
			}
			logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),tmp);
		}else
			logg[0].Format(_T("未成功载入模板图，请重试!\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
		Evolution = 0;
		//清理以前载入的训练集和测试集
		train.p_num = 0;
		train.n_num = 0;
		test.num = 0;
		resetP_N = 1;
	}
	runing = 0;
	LOG("<<结束载入模板图———\n\n");
}

void CImgMatchView::OnShowModel()
{
	if(model.n>0){
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		tmp.Format(_T("当前已载入模板图:\r\n"));
		for(int i=0;i<model.n;i++){
			logg[0].Format("%s%s\r\n",tmp,model.path[i]);
			tmp = logg[0];
		}
		logg[0].Format("%s-------------操作分割线-------------\r\n\r\n",tmp);
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}else{
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		logg[0].Format(_T("当前还未载入模板图！\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}
}

void CImgMatchView::OnAddTest()
{
	LOG(">>增加测试集\n\n");
	runing = 1;
	strcpy(info,"");
	char fileFormat[] = _T("JPEG (*.jpg)|*.jpg;*.jpeg|PNG (*.png)|*.png|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE,0,0,OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,fileFormat);
	int check=0;
	if(DragImgPath[0]=="")		//如果没有拖入文件作为测试集则使用文件对话框
		check = (dlg.DoModal()==IDOK);
	if(DragImgPath[0][0] || check)	//传入了文件路径列表 或者 读取了文件
	{
		int num = test.num;
		POSITION pos = dlg.GetStartPosition();
		int jj=0;
		bool loop = DragImgPath[0]=="" ? pos!=0 : DragImgPath[jj][0]!=0;
		while(loop){
			if(DragImgPath[0]!=""){
				test.p[test.num]=DragImgPath[jj++];
				loop = DragImgPath[jj][0]!=0;
			}else{
				test.p[test.num]=dlg.GetNextPathName(pos);
				loop = pos!=0;
			}
			LOG(test.p[test.num]);
			if(!(tmpimg = loadImg(test.p[test.num],0,0,1,0))){
				test.num--;
				LOG("\t载入失败，已跳过");
			}else if(tmpimg->width>MAXSIZE || tmpimg->height>MAXSIZE)
			{
				char exit_info[1000];
				sprintf(exit_info,("图像%s尺寸过大，请先处理为%d*%d以内再载入！\r\n\r\n\r\n已自动跳过该图像！"),test.p[test.num],MAXSIZE,MAXSIZE);
				MessageBox(exit_info,"获取图像信息失败",0);
				test.num--;
				LOG("\t尺寸过大，已跳过")
			}
			releaseImg(&tmpimg);
			test.num++;
			LOG("\n")
		}
		LOG("———测试集读取完毕———\n");
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		if(test.num-num>0){
			tmp.Format(_T("已载入下列%d个测试集:\r\n"),test.num-num);
			for(int i=num;i<test.num;i++){
				logg[0].Format(_T("%s%s\r\n"),tmp,test.p[i]);
				tmp = logg[0];
			}
			logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),tmp);
			test.times[test.times[9]] = test.num-1;
			test.times[9]++;
		}else
			logg[0].Format(_T("未成功载入测试集，请重试!\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}
	runing = 0;
	LOG("<<结束增加测试集———\n\n");
	if(Evolution==1 && test.num>0)
		if(MessageBox("你已载入了测试集，是否立即匹配？","开始匹配提醒",MB_YESNO)==IDYES)
			OnStartMatch();
}

void CImgMatchView::OnShowTest()
{
	for(int i=log_num-1;i>0;i--)
		logg[i] = logg[i-1];
	if(test.num>0){
		tmp.Format(_T("当前已载入下列测试集:\r\n"));
		for(int i=0;i<test.num;i++){
			logg[0].Format(_T("%s%s\r\n"),tmp,test.p[i]);
			tmp = logg[0];
		}
		logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),tmp);
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
	}else{
		logg[0].Format(_T("当前还未载入测试集！\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
	}
	UpdateData(false);
	updatelog();
}

void CImgMatchView::OnClearTest()
{
	LOG(">>重置测试集\n\n")
	if(test.num>0){
		test.num = 0;
		memset(test.times,0,sizeof(test.times));
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		logg[0].Format(_T("已清空当前已载入的测试集\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
	}else
		OnShowTest();
}

UINT evoTrain(void* pp){
	long time1 = clock(),time2;
	CImgMatchView* pView = (CImgMatchView*)pp;
	MSG &msg = pView->msg;
	Target &t3 = pView->t3, &t4 = pView->t4;
	for(int i=0;i<pView->train.p_num;i++){		//正训练集
		if(pView->train.done_p[i])
			continue;
		pView->train.done_p[i]=1;
		double best = 0;
		int index = 0;
		for(int j=0;j<pView->model.n && pView->model.n>1;j++)
		{
			t3.getTargetInfo(pView->model.path[j]);
			t4.getTargetInfo(pView->train.p[i]);
			double re = t3.compare(t4,0,1,1,pView->ratio);
			if(best<re)
			{
				index = j;
				best = re;
			}
			time2 = clock();
			if (time2 - time1 > 50 && PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)){
				SendMessage(0,msg.message, msg.wParam, msg.lParam);
				time1 = time2;
				Sleep(5);
			}
			LOG("i="<<i<<" j="<<j<<">\n");
			pView->pMF->m_progress.StepIt();
		}
		t3.getTargetInfo(pView->model.path[index]);
		t4.getTargetInfo(pView->train.p[i]);
		t3.compare(t4,"p_n.txt",1,0,pView->ratio);
		pView->pMF->m_progress.StepIt();
		if(pView->model.n==1)
			pView->pMF->m_progress.StepIt();
	}
	for(int i=0;i<pView->train.n_num;i++){		//负训练集
		if(pView->train.done_n[i])
			continue;
		pView->train.done_n[i]=1;
		for(int j=0;j<pView->model.n;j++)
		{
			t3.getTargetInfo(pView->model.path[j]);
			t4.getTargetInfo(pView->train.n[i]);
			t3.compare(t4, "p_n.txt", 0, 0, pView->ratio);
			time2 = clock();
			if (time2 - time1 > 50 && PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)){
				SendMessage(0,msg.message, msg.wParam, msg.lParam);
				time1 = time2;
				Sleep(5);
			}
			LOG("i="<<i<<" j="<<j<<">\n");
			pView->pMF->m_progress.StepIt();
		}
	}
	pView->runing = 0;
	return 0;
}

void CImgMatchView::OnStartEvolution()
{
	LOG(">>开始演化训练\n");
	runing = 1;
	_CrtMemCheckpoint(&cc1);	//检查内存泄露
	if(model.n && train.p_num){// && train.n_num---舍弃对负训练集的要求，即可以不载入负训练集
		memcpy(ratio,Dlg.ratio,sizeof(Dlg.ratio));
		int nUpper=0;	//进度条最大值
		long time = clock(),time1 = time,time2=0;
		train.done_p[0]=train.done_n[0]=1;
		LOG(">>>>正在读取训练集...模板个数："<<model.n<<"  训练集个数 正："<<train.p_num<<" 负："<<train.n_num<<endl);
		CString s1="正在读取训练集并开始训练...",s2;
		pStatues->SetPaneText(0,s1);
		pStatues->SetPaneText(2,_T("读取中"));
		static int lastp = train.p_num, lastn = train.n_num;
#pragma region Training
		try{
		if(resetP_N){					//重置训练集了则重新演化
			LOG("———已启动重新生成演化结果———\n");
			DeleteFile("p_n.txt");
			resetP_N = 0;
			nUpper = train.p_num*(model.n+1)+train.n_num*model.n;
			memset(train.done_p,0,train.p_num);
			memset(train.done_n,0,train.n_num);
			LOG("———正在读取正训练集———\n");
			pMF->m_progress.SetRange(0,nUpper);
			if(b_multiThread)
				AfxBeginThread(evoTrain,this);
			for(int i=0;i<train.p_num;i++){		//正训练集
				if(train.done_p[i])
					continue;
				train.done_p[i]=1;
				double best = 0;
				int index = 0;
				for(int j=0;j<model.n && model.n>1;j++)
				{
					int ttt = clock();
					t.getTargetInfo(model.path[j]);
					//LOGT("getTargetInfo1:\t"<<clock()-ttt<<endl);ttt = clock();
					t2.getTargetInfo(train.p[i]);
					//LOGT("getTargetInfo2:\t"<<clock()-ttt<<endl);ttt = clock();
					double re = t.compare(t2,0,1,1,ratio);
					//LOGT("compare:\t"<<clock()-ttt<<endl);
					if(best<re)
					{
						index = j;
						best = re;
					}
					time2 = clock();
					if (time2 - time1 > 50 && PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)){
						SendMessage(msg.message, msg.wParam, msg.lParam);
						time1 = time2;
						Sleep(5);
					}
					if (PeekMessage(&msg,(HWND)NULL, 0, 0, PM_REMOVE))
						SendMessage(msg.message, msg.wParam, msg.lParam);
					LOG("i="<<i<<" j="<<j<<"\n");
					pMF->m_progress.StepIt();
					s2.Format(_T("%s 已进行 %.1lf %%"),s1,pMF->m_progress.GetPos()/0.01/nUpper);
					pStatues->SetPaneText(0,s2);
					s2.Format(_T("耗时：%.2f s"),(time2-time)/1000.0);
					pStatues->SetPaneText(3,s2);
				}
				t.getTargetInfo(model.path[index]);
				t2.getTargetInfo(train.p[i]);
				t.compare(t2,"p_n.txt",1,0,ratio);
				pMF->m_progress.StepIt();
				if(model.n==1)
					pMF->m_progress.StepIt();
				s2.Format(_T("%s 已进行 %.1lf %%"),s1,pMF->m_progress.GetPos()/0.01/nUpper);
				pStatues->SetPaneText(0,s2);
				s2.Format(_T("耗时：%.2f s"),(clock()-time)/1000.0);
				pStatues->SetPaneText(3,s2);
			}
			LOG("———正在读取负训练集———\n");
			for(int i=0;i<train.n_num;i++){		//负训练集
				if(train.done_n[i])
					continue;
				train.done_n[i]=1;
				for(int j=0;j<model.n;j++)
				{
					t.getTargetInfo(model.path[j]);
					t2.getTargetInfo(train.n[i]);
					t.compare(t2, "p_n.txt", 0, 0, ratio);
					time2 = clock();
					if (time2 - time1 > 50 && PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)){
						SendMessage(msg.message, msg.wParam, msg.lParam);
						time1 = time2;
						Sleep(5);
					}
					LOG("i="<<i<<" j="<<j<<"\n");
					pMF->m_progress.StepIt();
					s2.Format(_T("%s 已进行 %.1lf %%"),s1,pMF->m_progress.GetPos()/0.01/nUpper);
					pStatues->SetPaneText(0,s2);
					s2.Format(_T("耗时：%.2f s"),(time2-time)/1000.0);
					pStatues->SetPaneText(3,s2);
				}
			}
		}else if(lastp!=train.p_num || lastn != train.n_num){	//否则就是新增了训练集，直接加入新的数据进模板
			LOG("———已启动添加演化结果———\n———正在读取正训练集("<<train.p_num-lastp<<")———\n");
			memset(&train.done_p[lastp],0,train.p_num-lastp);
			memset(&train.done_n[lastn],0,train.n_num-lastn);
			nUpper = (train.p_num-lastp)*(model.n+1) + (train.n_num-lastn)*model.n;
			pMF->m_progress.SetRange(0,nUpper);
			if(b_multiThread)
				AfxBeginThread(evoTrain,this);
			for(int i=lastp;i<train.p_num;i++){		//增加 正训练集
				if(train.done_p[i])
					continue;
				train.done_p[i]=1;
				double best = 0;
				int index = 0;
				for(int j=0;j<model.n && model.n>1;j++)
				{
					t.getTargetInfo(model.path[j]);
					t2.getTargetInfo(train.p[i]);
					double re = t.compare(t2,0,1,1,ratio);
					if(best<re)
					{
						index = j;
						best = re;
					}
					time2 = clock();
					if (time2 - time1 > 50 && PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)){
						SendMessage(msg.message, msg.wParam, msg.lParam);
						time1 = time2;
						Sleep(5);
					}
					LOG("i="<<i<<" j="<<j<<"\n");
						pMF->m_progress.StepIt();
					s2.Format(_T("%s 已进行 %.1lf %%"),s1,pMF->m_progress.GetPos()/0.01/nUpper);
					pStatues->SetPaneText(0,s2);
					s2.Format(_T("耗时：%.2f s"),(time2-time)/1000.0);
					pStatues->SetPaneText(3,s2);
				}
				t.getTargetInfo(model.path[index]);
				t2.getTargetInfo(train.p[i]);
				t.compare(t2,"p_n.txt",1,0,ratio);
				pMF->m_progress.StepIt();
				if(model.n==1)
					pMF->m_progress.StepIt();
				s2.Format(_T("%s 已进行 %.1lf %%"),s1,pMF->m_progress.GetPos()/0.01/nUpper);
				pStatues->SetPaneText(0,s2);
				s2.Format(_T("耗时：%.2f s"),(time2-time)/1000.0);
				pStatues->SetPaneText(3,s2);
			}
			LOG("———正在读取负训练集("<<train.n_num-lastn<<")———\n");
			for(int i=lastn;i<train.n_num;i++){		//增加 负训练集
				if(train.done_n[i])
					continue;
				train.done_n[i]=1;
				for(int j=0;j<model.n;j++)
				{
					t.getTargetInfo(model.path[j]);
					t2.getTargetInfo(train.n[i]);
					t.compare(t2, "p_n.txt", 0, 0, ratio);
					time2 = clock();
					if (time2 - time1 > 50 && PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)){
						SendMessage(msg.message, msg.wParam, msg.lParam);
						time1 = time2;
						Sleep(5);
					}
					LOG("i="<<i<<" j="<<j<<"\n");
					pMF->m_progress.StepIt();
					s2.Format(_T("%s 已进行 %.1lf %%"),s1,pMF->m_progress.GetPos()/0.01/nUpper);
					pStatues->SetPaneText(0,s2);
					s2.Format(_T("耗时：%.2f s"),(time2-time)/1000.0);
					pStatues->SetPaneText(3,s2);
				}
			}
		}
		}catch(...)
		{
			LOG("读取训练集时出现异常\n\n");
			runing = 0;
			MessageBox("非常抱歉，在读取训练集时出现异常，请关闭后重试","运行出错",0);
			rename("Log","ErrorLog.txt");
			exit(0);
		}
#pragma endregion
		while(pMF->m_progress.GetPos()!=nUpper)
		{
			Sleep(5);
			if (PeekMessage(&msg,(HWND)NULL, 0, 0, PM_REMOVE))
				SendMessage(msg.message, msg.wParam, msg.lParam);
		}
		pMF->m_progress.SetPos(0);
		LOG("———训练集读取完毕———\n———开始演化过程———\n");
		_CrtMemCheckpoint(&cc2);
		if(_CrtMemDifference(&cc3, &cc1, &cc2))
			_CrtMemDumpStatistics(&cc3);

		pStatues->SetPaneText(0,_T("训练集已读取完毕,即将进行演化过程请耐心等待..."));
		pStatues->SetPaneText(2,_T("演化中"));
		int dataNum = ml.loadDataFromFile("p_n.txt",-1,1,1);
		ml.setWait_Col(Dlg.waitTime,Dlg.EvoCol);
		ml.setSave(1);	//保存此次演化后的路径到预定文件
		ml.setModel(Dlg.absolut);
		LOG("———进入演化:dataNum="<<dataNum<<"———\n");
		ml.startLearn(dataNum, pStatues, Dlg.permitError,Dlg.balance_ratio,(LPCTSTR)EvoPath);
		LOG("———演化完毕———\n");
		pStatues->SetPaneText(2,_T("完成"));
		Evolution = 1;
		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		logg[0].Format(_T("%s\r\n-------------操作分割线-------------\r\n\r\n"),ml_info);
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
		lastp = train.p_num, lastn = train.n_num;
	}else if(0==model.n)
		MessageBox("请先载入模板图","演化出错",MB_ICONERROR);
	else if(Evolution==1)
		MessageBox("当前未找到正训练集！\n\n你已经从文件成功读取了演化路径，可以直接载入测试图进行匹配","演化出错",MB_ICONERROR);
	else
		MessageBox("请先载入正训练集","演化出错",MB_ICONERROR);
	runing = 0;
	LOG("<<结束演化训练———\n\n");
}

UINT match(void* pp){
	MSG msg;
	long  time1 = clock(), time2;
	CImgMatchView* pView = (CImgMatchView*)pp;
	for(int i=0;i<pView->test.num;i++)
	{
		if(pView->test.done[i])
			continue;
		pView->test.done[i] = 1;
		if(pView->model.n==1)	//单模板
		{
			pView->t3.getTargetInfo(pView->model.path[0]);
			pView->t4.getTargetInfo(pView->test.p[i]);
			pView->t3.compare(pView->t4,"test.txt",-1,0,pView->ratio);
			pView->pMF->m_progress.StepIt();
			pView->pMF->m_progress.StepIt();
			time2 = clock();
			if (time2 - time1 > 50 && PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)){
				SendMessage(0, msg.message, msg.wParam, msg.lParam);
				time1 = time2;
				Sleep(5);
			}
		}else{			//多模板
			double best = 0;
			int index = 0;
			for(int j=0;j<pView->model.n;j++)
			{
				pView->t3.getTargetInfo(pView->model.path[j]);
				pView->t4.getTargetInfo(pView->test.p[i]);
				double re = pView->t3.compare(pView->t4,0,-1,1,pView->ratio);//注意此处如果演化完再调整ratio的话就会出现未预期的结果
				if(best<re)
				{
					index = j;
					best = re;
				}
				time2 = clock();
				if (time2 - time1 > 50 && PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)){
					SendMessage(0,msg.message, msg.wParam, msg.lParam);
					time1 = time2;
					Sleep(5);
				}
				pView->pMF->m_progress.StepIt();
			}
			pView->t3.getTargetInfo(pView->model.path[index]);
			pView->t4.getTargetInfo(pView->test.p[i]);
			pView->t3.compare(pView->t4,"test.txt",-1,0,pView->ratio);
			pView->pMF->m_progress.StepIt();
		}
	}
	pView->runing = 0;
	return 0;
}

void CImgMatchView::OnStartMatch()
{
	LOG(">>开始匹配\n");
	runing = 1;
	if(Evolution==1){
		if(test.num>0){
			long time = clock(),time1 = time,time2=0;
			memset(test.done,0,test.num);
			CString s1="正在读取测试集...",s2;
			pStatues->SetPaneText(0,_T(s1));
			pStatues->SetPaneText(2,_T("读取中"));
			DeleteFile("test.txt");
			try{
				if(b_multiThread)
					AfxBeginThread(match,this);
				int nUpper = test.num*(model.n+1);
				pMF->m_progress.SetRange(0,nUpper);
				for(int i=0;i<test.num;i++)
				{
					if(test.done[i])
						continue;
					test.done[i] = 1;
					if(model.n==1)	//单模板
					{
						t.getTargetInfo(model.path[0]);
						t2.getTargetInfo(test.p[i]);
						t.compare(t2,"test.txt",-1,0,ratio);
						pMF->m_progress.StepIt();
						pMF->m_progress.StepIt();
						s2.Format(_T("%s 已进行 %.1lf %%"),s1,pMF->m_progress.GetPos()/0.01/nUpper);
						pStatues->SetPaneText(0,s2);
						time2 = clock();
						s2.Format(_T("耗时：%.2g s"),(time2-time)/1000.0);
						pStatues->SetPaneText(3,s2);
						if (time2 - time1 > 50 && PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)){
							SendMessage(msg.message, msg.wParam, msg.lParam);
							time1 = time2;
							Sleep(5);
						}
					}else{			//多模板
						double best = 0;
						int index = 0;
						for(int j=0;j<model.n;j++)
						{
							t.getTargetInfo(model.path[j]);
							t2.getTargetInfo(test.p[i]);
							double re = t.compare(t2,0,-1,1,ratio);
							if(best<re)
							{
								index = j;
								best = re;
							}
							pMF->m_progress.StepIt();
							s2.Format(_T("%s 已进行 %.1lf %%"),s1,pMF->m_progress.GetPos()/0.01/nUpper);
							pStatues->SetPaneText(0, s2);
							time2 = clock();
							s2.Format(_T("耗时：%.2g s"),(time2-time)/1000.0);
							pStatues->SetPaneText(3, s2);
							if (time2 - time1 > 50 && PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)){
								SendMessage(msg.message, msg.wParam, msg.lParam);
								time1 = time2;
								Sleep(5);
							}
						}
						t.getTargetInfo(model.path[index]);
						t2.getTargetInfo(test.p[i]);
						t.compare(t2,"test.txt",-1,0,ratio);
						pMF->m_progress.StepIt();
						s2.Format(_T("%s 已进行 %.1lf %%"),s1,pMF->m_progress.GetPos()/0.01/nUpper);
						pStatues->SetPaneText(0,s2);
						s2.Format(_T("耗时：%.2g s"),(clock()-time)/1000.0);
						pStatues->SetPaneText(3,s2);
					}
				}
				while (pMF->m_progress.GetPos()!=nUpper)
				{
					Sleep(5);
					if (PeekMessage(&msg,(HWND)NULL, 0, 0, PM_REMOVE))
						SendMessage(msg.message, msg.wParam, msg.lParam);
				}
			}catch(...)
			{
				MessageBox("非常抱歉，在读取训练集时出现异常，请关闭后重试","运行出错",0);
				return;
			}
			LOG("———读取测试集完成———\n");
			ml.getTestOut("test.txt",-1,1);
			
			for(int i=log_num-1;i>0;i--)
				logg[i] = logg[i-1];
			tmp.Empty();
			int yes = 0;
			for(int i=0,j=0;i<test.num;i++){
				CString s;
				if(testOut[i]==0)
					s = _T("(不匹配)");
				else if(testOut[i]==1)
				{
					s = _T("(匹配)");
					yes ++ ;
				}
				if(j<test.times[9] && i==test.times[j])
				{
					Sleep(1);
					++j;
					logg[0].Format(_T("%s%s    \t%s\r\n\r\n"),tmp,test.p[i],s);
				}else
					logg[0].Format(_T("%s%s    \t%s\r\n"),tmp,test.p[i],s);
				tmp = logg[0];
			}
			s2.Format(_T("测试集的测试结果如下(匹配/总数 = %d/%d):\r\n%s"),yes,test.num,tmp);
			tmp = s2;
			//更改匹配日志
			CString tmp2;
			matchlog.Format(_T("%s\r\n"),printDateTime("本次操作时间："));
			tmp2.Format(_T("%s当前模板图:\r\n"),matchlog);
			for(int i=0;i<model.n;i++){
				matchlog.Format("%s%s\r\n",tmp2,model.path[i]);
				tmp2 = matchlog;
			}
			matchlog.Format(_T("%s\r\n%s\r\n\r\n"),tmp2,tmp);

			LOG("———匹配完成———\n"<<matchlog);
			s2.Format(_T("匹配完成 !  匹配/总数 = %d/%d(匹配率 %.3g%%)"),yes,test.num,100.0*yes/test.num);

			logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),tmp);
			log = logg[log_num-1];
			for(int i=log_num-2;i>=0;i--)
				log += logg[i];
			UpdateData(false);
			updatelog();
			test.num = 0;
			memset(test.times,0,sizeof(test.times));
			pStatues->SetPaneText(0,s2);
			pStatues->SetPaneText(2,_T("完成"));
			//pStatues->SetPaneText(3,_T(""));
			pMF->m_progress.SetPos(0);
		}else
			MessageBox("请先载入测试集后再进行匹配","匹配出错",MB_ICONERROR);
	}else
		MessageBox("请先演化完成后再进行匹配","匹配出错",MB_ICONERROR);
	runing = 0;
	LOG(">>结束匹配\n\n");
}

void CImgMatchView::OnUseHelp()
{
	CHelpDlg help;
	help.DoModal();
}

void CImgMatchView::OnTrainHelp()
{
	MessageBox(_T("训练集是指用来训练自动匹配标准的数据集。\n\n正训练集是和模版图相近的数据集，"
				  "与模板图能够匹配；\n\n负训练集是不同于模板图特征的数据集，不能够与模板图匹配"),
				  "什么是训练集",0);
}

void CImgMatchView::OnTestHelp()
{
	MessageBox(_T("测试集是指需要判定其与模板是否匹配的数据集。\n\n自动匹配完成后将给出匹配或者不匹配的提示")
		,"什么是测试集",0);
}

void CImgMatchView::OnAdvancedSet()
{
	LOG(">>高级设置\n");
	Dlg.DoModal();
	ml.wait = Dlg.waitTime*1000;
	ml.permitError = Dlg.permitError;
	LOG("———模板列数:"<<Dlg.EvoCol<<"   允许误差:"<<Dlg.permitError<<"%———\n");
	LOG(">>结束高级设置\n\n");
}

void CImgMatchView::OnSavematch()
{
	if(matchlog[0])
	{
		FILE *fp1 = fopen("Log\\match-log.txt","a+");
		fprintf(fp1,"%s",matchlog);
		fclose(fp1);
		MessageBox("匹配记录已保存到文件 Log\\match-log.txt 中！","保存成功",MB_ICONINFORMATION);
	}else
	{
		MessageBox("请先匹配之后再使用该功能","保存失败",MB_ICONERROR);
	}
}

void CImgMatchView::OnSaveEvoPath()
{
	if(PathFileExists("best.pth")){			//当前最好路径已保存
		char fileFormat[] = _T("演化路径文件 EvoPath (*.epath)|*.epath|All Files (*.*)|*.*||");
		CFileDialog dlg(false,"epath","Evo_path",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,fileFormat);
		if(dlg.DoModal()==IDOK)
		{
			ofstream file1(dlg.GetPathName().GetBuffer());
			//保存模板数据
			file1<<model.n<<endl;
			for(int i=0;i<model.n;++i)
				file1<<model.path[i]<<endl;
			//保存演化参数
			file1<<"\n"<<Dlg.absolut<<"\t"<<Dlg.balance_ratio<<"\t";
			for(int i=0;i<Dlg.ratio_num;i++)
				file1<<"\t"<<ratio[i];
			//保存演化信息(模板长度和路径)
			file1<<endl<<endl<<ml.getCol()<<endl;
			ifstream file2("best.pth");
			char ch;
			while(file2.get(ch) && ch!='\n' && ch!='\r')	//读取文本中的内容	
				file1.put(ch);		//写入内容到文件
			file1.close();
			file2.close();			//关闭文件流
			MessageBox("已保存演化后路径","保存演化路径成功",MB_ICONINFORMATION);
		}
	}else
		MessageBox("当前未发现可保存的路径，请先进行演化后再使用该功能","保存演化路径出错",MB_ICONERROR);
}

void CImgMatchView::OnReadEvoPath()
{
	LOG(">>读取演化路径\n");
	char fileFormat[] = _T("演化路径文件 EvoPath (*.epath)|*.epath|All Files (*.*)|*.*||");
	CFileDialog dlg(true, "epath", "Evo_path", OFN_FILEMUSTEXIST, fileFormat);
	int check = 0;
	if (DragEvoPath == "")
		check = (dlg.DoModal() == IDOK);
	if (DragEvoPath[0] || check)	//传入了文件路径 或者 读取了文件
	{
		CString EvoPath = DragEvoPath[0] ? DragEvoPath : dlg.GetPathName();
		char path[512];
		freopen((LPSTR)(LPCTSTR)EvoPath.GetBuffer(),"r",stdin);
		cin>>model.n;
		getchar();
		int flag = 1;
		for(int i=0;i<model.n;++i){
			gets(path);
			model.path[i] = path;
			if(!(tmpimg = loadImg(model.path[i],0,0,1,0)))
			{
				MessageBox("从路径中获取模板图失败，请确认路径有效且模板图存在","载入演化信息失败");
				releaseImg(&tmpimg);
				model.n = 0;
				break;
			}
			releaseImg(&tmpimg);
		}
		if(model.n>0){
			cin>>Dlg.absolut>>Dlg.balance_ratio;
			for(int i=0;i<Dlg.ratio_num;i++)
			{
				cin>>Dlg.ratio[i];
				ratio[i] = Dlg.ratio[i];
			}
			cin>>ml.getCol();
			Dlg.EvoCol = ml.getCol();
			ml.setWait_Col(Dlg.waitTime,Dlg.EvoCol);
			for(int i=0;i<ml.getChromlen();++i)
				cin>>ml.getBest().chrom[i];
			if(flag){
				Evolution = 1;
				//清理以前载入的训练集和测试集
				train.p_num = 0;
				train.n_num = 0;
				test.num = 0;
				resetP_N = 1;
				LOG("模板列数："<<Dlg.EvoCol<<"偏向性系数："<<Dlg.absolut<<"\n");
				for(int i=log_num-1;i>0;i--)
					logg[i] = logg[i-1];
				if(model.n>0){
					tmp.Format(_T("已成功载入演化路径信息，可载入测试集开始匹配！\r\n其中包含下列%d个模板:\r\n"),model.n);
					for(int i=0;i<model.n;i++){
						logg[0].Format(_T("%s%s\r\n"),tmp,model.path[i]);
						tmp = logg[0];
					}
					LOG(tmp);
					logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),tmp);
				}
				log = logg[log_num-1];
				for(int i=log_num-2;i>=0;i--)
					log += logg[i];
				UpdateData(false);
				updatelog();
			}
		}else
			flag = 0;
		if(flag==0)
			logg[0].Format(_T("未成功载入路径信息，请重试!\r\n-------------操作分割线-------------\r\n\r\n"));
		freopen("CON","r",stdin);
	}
	LOG(">>结束读取演化路径\n\n");
}

void CImgMatchView::OnUpdateLog()
{
	CUpdateLogDlg uplog;
	uplog.DoModal();
}

//窗口消息相关
BOOL CImgMatchView::OnEraseBkgnd(CDC* pDC) 
{
	int x,y;
	CRect rc;
	GetClientRect(&rc);
	x=0;y=0;
	while(y < rc.Height()) 
	{
		//rc表示对话框的大小尺寸
		while(x < rc.Width()) 
		{
			m_bmpBackground.DrawBMP(pDC, x, y); 
			x += m_bmpBackground.GetWidth(); //得到位图的宽度
		}
		x = 0;
		y += m_bmpBackground.GetHeight();  //得到位图的高度
	}
	return TRUE;
}

HBRUSH CImgMatchView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	static bool first = 1;
	if(first){
		GetDlgItem(IDC_log)->GetWindowRect(&r);//获取控件相对于屏幕的位置
		ScreenToClient(&r);//转化为对话框上的相对位置
		first = 0;
	}
	static int t = clock();
	if(!runing && clock()-t>500)
	{
		/*
		((CEdit*)(GetDlgItem(IDC_log)))->ShowWindow(SW_HIDE);
		((CEdit*)(GetDlgItem(IDC_log)))->SetWindowText(log);
		((CEdit*)(GetDlgItem(IDC_log)))->ShowWindow(SW_SHOW);*/
		InvalidateRect(&r,1);
		t = clock();
	}
    // 对特定的控件做修改
	if (nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetTextColor(RGB(0,0,255)); //设置字体颜色
        pDC->SetBkMode(TRANSPARENT);//设置背景为透明
		static HBRUSH B = (HBRUSH)GetStockObject(NULL_BRUSH);//获取画笔颜色混合后的画笔，完成透明
		return B; //返回画刷句柄
	}
	return CFormView::OnCtlColor(pDC,pWnd, nCtlColor);
}

void CImgMatchView::OnEnVscrollLog()
{
	RedrawWindow();
}

UINT warn(void*){
	MessageBox(0,"我正在忙着给你计算，请过一会儿再移动我吧^_^","当前请不要移动窗口",0);
	return 0;
}
void CImgMatchView::OnNcLButtonDown(UINT u,CPoint cp)
{
	if (runing)
	{
		AfxBeginThread(warn, 0);
		return;
	}
	CFormView::OnNcLButtonDown(u,cp);
	RedrawWindow();
}
//以上为窗口消息相关

void CImgMatchView::OnSaveModelTrain()
{
	if(model.n && train.p_num)			//当前已载入模板图和正训练集
	{
		char fileFormat[] = _T("路径文件 Path (*.path)|*.path|All Files (*.*)|*.*||");
		CFileDialog dlg(false,"path","path",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,fileFormat);
		if(dlg.DoModal()==IDOK)
		{
			ofstream file1(dlg.GetPathName());
			//保存模板个数，训练集个数（正、负）
			file1<<model.n<<"\t"<<train.p_num<<"\t"<<train.n_num<<endl;
			//保存模板路径
			for(int i=0;i<model.n;++i)
				file1<<model.path[i]<<endl;
			//保存训练集
			file1<<endl;
			for(int i=0;i<train.p_num;++i)
				file1<<train.p[i]<<endl;
			file1<<endl;
			for(int i=0;i<train.n_num;++i)
				file1<<train.n[i]<<endl;
			file1.close();
			MessageBox("已保存模板图和训练集路径","保存模板和训练集路径成功",MB_ICONINFORMATION);
		}
	}else
		MessageBox("当前还未完全载入模板图和训练集，请先载入完整后再使用该功能","保存模板和训练集出错",MB_ICONERROR);
}

void CImgMatchView::OnReadModelTrain()
{
	LOG(">>读取模板和训练集\n");
	char fileFormat[] = _T("路径文件 Path (*.path)|*.path|All Files (*.*)|*.*||");
	CFileDialog dlg(true, "path", "path", OFN_FILEMUSTEXIST, fileFormat);
	int check = 0;
	if (DragModelTrainPath == "")
		check = (dlg.DoModal() == IDOK);
	if (DragModelTrainPath[0] || check)	//传入了文件路径 或者 读取了文件
	{
		CString PathName = DragModelTrainPath[0] ? DragModelTrainPath : dlg.GetPathName();
		char path[512];
		int n[4];
		freopen((LPSTR)(LPCTSTR)PathName.GetBuffer(),"r",stdin);
		cin>>n[0]>>n[1]>>n[2];
		n[3] = n[0]+n[1]+n[2];
		if(n[0]>0 && n[1]>0)
		{
			for(int i=0;i<n[3];++i)
			{
				if(i==0 || i==n[0] || i==n[0]+n[1])	//读入空行
					getchar();
				gets(path);
				if(!(tmpimg = loadImg(path,0,0,1,0)))
				{
					MessageBox("从路径中获取模板和训练集失败，请确认该路径文件有效","载入模板图和训练集失败");
					LOG(">>中断读取模板和训练集\n\n");
					return;
				}
				releaseImg(&tmpimg);
			}
		}else
		{
			MessageBox("从路径中获取模板和训练集失败，请确认该路径文件内容正确","载入模板图和训练集失败");
			LOG(">>中断读取模板和训练集\n\n");
			return;
		}
		freopen("CON","r",stdin);
		model.n=n[0];
		train.p_num=n[1];
		train.n_num=n[2];
		freopen((LPSTR)(LPCTSTR)PathName.GetBuffer(),"r",stdin);
		gets(path); //必须使用gets函数而不是cin或者scanf，否则会出现遇到空格即停止读入
		//载入模板路径
		for(int i=0;i<model.n;++i)
		{
			gets(path);
			model.path[i] = path;
		}
		//载入训练集
		getchar();
		for(int i=0;i<train.p_num;++i)
		{
			gets(path);
			train.p[i] = path;
		}
		getchar();
		for(int i=0;i<train.n_num;++i)
		{
			gets(path);
			train.n[i] = path;
		}
		freopen("CON","r",stdin);

		for(int i=log_num-1;i>0;i--)
			logg[i] = logg[i-1];
		if(model.n>0){
			tmp.Format(_T("已成功载入模板图和训练集信息，可开始演化或新增训练集！\r\n其中包含下列%d个模板:\r\n"),model.n);
			for(int i=0;i<model.n;i++)
			{
				logg[0].Format(_T("%s%s\r\n"),tmp,model.path[i]);
				tmp = logg[0];
			}
			logg[0].Format("%s\r\n包含下列%d个正训练集:\r\n",tmp,train.p_num);
			for(int i=0;i<train.p_num;i++)
			{
				tmp.Format(_T("%s%s\r\n"),logg[0],train.p[i]);
				logg[0] = tmp;
			}
			if(train.n_num)
				tmp.Format("%s\r\n包含下列%d个负训练集:\r\n",logg[0],train.n_num);
			for(int i=0;i<train.n_num;i++)
			{
				logg[0].Format(_T("%s%s\r\n"),tmp,train.n[i]);
				tmp = logg[0];
			}
			LOG(tmp);
			logg[0].Format(_T("%s-------------操作分割线-------------\r\n\r\n"),tmp);
		}else
			logg[0].Format(_T("未成功载入路径信息，请稍后重试!\r\n-------------操作分割线-------------\r\n\r\n"));
		log = logg[log_num-1];
		for(int i=log_num-2;i>=0;i--)
			log += logg[i];
		UpdateData(false);
		updatelog();
		test.num = 0;
		resetP_N = 1;
	}
	LOG(">>结束读取模板和训练集\n\n");
}

void CImgMatchView::OnQuit()
{
	if(::MessageBox(0,"你是否要退出该软件？","退出提醒",1)==1)
		PostQuitMessage(0);
}

void CImgMatchView::OnResizeWindow()
{
	//LONG style = ::GetWindowLong(this->GetSafeHwnd(),GWL_STYLE);
	pstyle->style |= WS_THICKFRAME;//使窗口能用鼠标改变大小
	//::SetWindowLong(this->GetSafeHwnd(),GWL_STYLE,pstyle->style);
	/*
	static int enable = 0;
	LONG style = ::GetWindowLong(*this,GWL_STYLE);
	if(!enable)
		style |=  WS_THICKFRAME;//使窗口能用鼠标改变大小
	else
		style &= ~WS_THICKFRAME;
	int a = ::SetWindowLong(*this,GWL_STYLE,style);
	CString ss;ss.Format("%d",a);
	MessageBox(ss);
	enable = !enable;
	*/
}

void CImgMatchView::OnDropFiles(HDROP hDropInfo)
{
	if (runing)
		return;
	int  nFileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 256);   //获取拖入的文件数量
	char filepath[256]="";
	int i=0,j=0;
	bool otherFiles = 0;
	SetForegroundWindow();		//设置窗口置顶显示
	for (i = 0; i<nFileCount; i++){
		DragQueryFile(hDropInfo,i,filepath,256);	// 获取拖放第i个文件的完整文件名
		if(GetFileAttributes(filepath)!=FILE_ATTRIBUTE_DIRECTORY	//是一个有效文件
			&& ((!strcmp(PathFindExtension(filepath),".jpg") || !strcmp(PathFindExtension(filepath),".bmp")
			|| !strcmp(PathFindExtension(filepath),".png"))))		//是一幅格式支持的图像文件
		{
			DragImgPath[j++] = filepath;
		}
		else if (GetFileAttributes(filepath) != FILE_ATTRIBUTE_DIRECTORY
			&& !strcmp(PathFindExtension(filepath), ".epath")){
			DragEvoPath = filepath;
			OnReadEvoPath();
		}
		else if (GetFileAttributes(filepath) != FILE_ATTRIBUTE_DIRECTORY
			&& !strcmp(PathFindExtension(filepath), ".path")){
			DragModelTrainPath = filepath;
			OnReadModelTrain();
		}
		else{
			otherFiles = 1;
			continue;
		}
	}
	if(j>0){
		CChooseType ct;
		if (ct.DoModal() == IDOK){
			switch (ct.m_choosed)
			{
			case 0:OnOpenModelMany(); break;
			case 1:OnTrainP(); break;
			case 2:OnTrainN(); break;
			case 3:OnAddTest(); break;
			default:break;
			}
		}
		for(int i=0;i<j;++i)
			DragImgPath[i].Empty();
	}
	if (DragEvoPath[0])
		DragEvoPath.Empty();
	if (DragModelTrainPath[0])
		DragModelTrainPath.Empty();
	if (otherFiles)
		MessageBox("仅支持拖入jpg,bmp,png格式的图像文件和.path/.epath格式的路径文件，其他文件已自动跳过","筛选提示");
	DragFinish(hDropInfo);   // 注意这个不能少，用于释放Windows 为处理文件拖放而分配的内存
}

